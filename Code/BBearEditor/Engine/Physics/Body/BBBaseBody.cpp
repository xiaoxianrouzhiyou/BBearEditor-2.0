#include "BBBaseBody.h"
#include "../Constraint/BBBaseConstraint.h"


BBBaseBody::BBBaseBody(int nParticleCount, float fMass)
{
    m_nParticleCount = nParticleCount;
    m_fParticleMass = fMass;
    m_fDamping = 1.0f;
}

void BBBaseBody::projectConstraints(float fDeltaTime)
{
    for (int i = 0; i < m_Constraints.size(); i++)
    {
        m_Constraints[i]->doConstraint(fDeltaTime);
    }
}
