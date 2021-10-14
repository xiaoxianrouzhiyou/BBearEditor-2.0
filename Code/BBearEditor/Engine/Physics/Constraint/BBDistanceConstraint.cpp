#include "BBDistanceConstraint.h"
#include "../Body/BBBaseBody.h"


BBDistanceConstraint::BBDistanceConstraint(BBBaseBody *pBody, int nParticleIndex1, int nParticleIndex2, float fElasticModulus)
    : BBBaseConstraint(pBody)
{
    m_nParticleIndex1 = nParticleIndex1;
    m_nParticleIndex2 = nParticleIndex2;
    m_fElasticModulus = fElasticModulus;
    m_fOriginLength = pBody->getParticlePosition(nParticleIndex1).distanceToPoint(pBody->getParticlePosition(nParticleIndex2));
}

void BBDistanceConstraint::doConstraint(float fDeltaTime)
{
    QVector3D position1 = m_pBody->getParticlePredictedPosition(m_nParticleIndex1);
    QVector3D position2 = m_pBody->getParticlePredictedPosition(m_nParticleIndex2);
    QVector3D n = position2 - position1;
    float d = n.length();
    n.normalize();
    QVector3D correction = m_fElasticModulus * n * (d - m_fOriginLength) * fDeltaTime;
    m_pBody->setParticlePredictedPosition(m_nParticleIndex1, position1 + correction);
    m_pBody->setParticlePredictedPosition(m_nParticleIndex2, position2 - correction);
}
