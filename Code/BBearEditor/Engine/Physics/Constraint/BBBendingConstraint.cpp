#include "BBBendingConstraint.h"
#include "../Body/BBBaseBody.h"


BBBendingConstraint::BBBendingConstraint(BBBaseBody *pBody, int nParticleIndex1, int nParticleIndex2, int nParticleIndex3, float fElasticModulus)
    : BBBaseConstraint(pBody)
{
    m_nParticleIndex1 = nParticleIndex1;
    m_nParticleIndex2 = nParticleIndex2;
    m_nParticleIndex3 = nParticleIndex3;
    m_fElasticModulus = fElasticModulus;
    QVector3D center = (pBody->getParticlePosition(nParticleIndex1) + pBody->getParticlePosition(nParticleIndex2) + pBody->getParticlePosition(nParticleIndex3)) / 3.0f;
    m_fOriginLength = pBody->getParticlePosition(nParticleIndex3).distanceToPoint(center);
}

void BBBendingConstraint::doConstraint(float fDeltaTime)
{
    QVector3D position1 = m_pBody->getParticlePredictedPosition(m_nParticleIndex1);
    QVector3D position2 = m_pBody->getParticlePredictedPosition(m_nParticleIndex2);
    QVector3D position3 = m_pBody->getParticlePredictedPosition(m_nParticleIndex3);
    QVector3D center = (position1 + position2 + position3) / 3.0f;
    QVector3D dir = position3 - center;
    float d = dir.length();
    float diff = 1.0f - m_fOriginLength / d;
    QVector3D force = diff * dir;
    QVector3D f = m_fElasticModulus / 2.0f * force * fDeltaTime;
    m_pBody->setParticlePredictedPosition(m_nParticleIndex1, position1 + f);
    m_pBody->setParticlePredictedPosition(m_nParticleIndex2, position2 + f);
    f = -2.0f * f;
    m_pBody->setParticlePredictedPosition(m_nParticleIndex3, position3 + f);
}
