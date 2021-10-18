#include "BBPinConstraint.h"
#include "../Body/BBBaseBody.h"


BBPinConstraint::BBPinConstraint(BBBaseBody *pBody, int nParticleIndex, const QVector3D &fixedPosition)
    : BBBaseConstraint(pBody)
{
    m_nParticleIndex = nParticleIndex;
    m_FixedPosition = fixedPosition;
}

void BBPinConstraint::doConstraint(float fDeltaTime)
{
    Q_UNUSED(fDeltaTime);
    m_pBody->setParticlePosition(m_nParticleIndex, m_FixedPosition);
    m_pBody->setParticlePredictedPosition(m_nParticleIndex, m_FixedPosition);
}
