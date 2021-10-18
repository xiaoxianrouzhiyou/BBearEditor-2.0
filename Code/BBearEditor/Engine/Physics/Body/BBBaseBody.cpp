#include "BBBaseBody.h"
#include "../Constraint/BBBaseConstraint.h"
#include "Utils/BBUtils.h"


BBBaseBody::BBBaseBody(int nParticleCount, float fMass)
{
    m_nParticleCount = nParticleCount;
    m_fParticleMass = fMass;
    m_fDamping = 1.0f;

    m_pPositions = new QVector3D[nParticleCount];
    m_pPredictedPositions = new QVector3D[nParticleCount];
    m_pVelocities = new QVector3D[nParticleCount];
}

BBBaseBody::~BBBaseBody()
{
    BB_SAFE_DELETE_ARRAY(m_pPositions);
    BB_SAFE_DELETE_ARRAY(m_pPredictedPositions);
    BB_SAFE_DELETE_ARRAY(m_pVelocities);
}

void BBBaseBody::dampenVelocities(float fDeltaTime)
{
    for (int i = 0; i < m_nParticleCount; i++)
    {
        m_pVelocities[i] *= 1 - m_fDamping * fDeltaTime;
    }
}

void BBBaseBody::predictPositions(float fDeltaTime)
{
    for (int i = 0; i < m_nParticleCount; i++)
    {
        m_pPredictedPositions[i] = m_pPositions[i] + m_pVelocities[i] * fDeltaTime;
    }
}

void BBBaseBody::projectConstraints(float fDeltaTime)
{
    for (int i = 0; i < m_Constraints.size(); i++)
    {
        m_Constraints[i]->doConstraint(fDeltaTime);
    }
}

void BBBaseBody::updateVelocities(float fDeltaTime, float fStopThreshold2)
{
    for (int i = 0; i < m_nParticleCount; i++)
    {
        QVector3D d = m_pPredictedPositions[i] - m_pPositions[i];
        m_pVelocities[i] = d / fDeltaTime;
        if (m_pVelocities[i].lengthSquared() < fStopThreshold2)
        {
            m_pVelocities[i] = QVector3D(0, 0, 0);
        }
    }
}

void BBBaseBody::updatePositions()
{
    for (int i = 0; i < m_nParticleCount; i++)
    {
        m_pPositions[i] = m_pPredictedPositions[i];
    }
}
