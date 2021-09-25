#include "BBSPHParticleSystem.h"
#include "Utils/BBUtils.h"


BBSPHParticleSystem::BBSPHParticleSystem()
{
    m_pParticleBuffer = nullptr;
    m_nParticleCount = 0;
    m_nBufferCapacity = 0;
}

BBSPHParticleSystem::~BBSPHParticleSystem()
{
    BB_SAFE_DELETE_ARRAY(m_pParticleBuffer);
}
