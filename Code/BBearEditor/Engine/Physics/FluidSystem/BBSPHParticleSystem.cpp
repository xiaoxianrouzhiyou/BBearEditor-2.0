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

void BBSPHParticleSystem::reset(unsigned int nCapacity)
{
    m_nBufferCapacity = nCapacity;
    if (m_pParticleBuffer)
    {
        BB_SAFE_DELETE(m_pParticleBuffer);
    }
    if (m_nBufferCapacity > 0)
    {
        m_pParticleBuffer = new BBSPHParticle[m_nBufferCapacity];
    }
    m_nParticleCount = 0;
}

BBSPHParticle* BBSPHParticleSystem::addParticle()
{
    if (m_nParticleCount >= m_nBufferCapacity)
    {
        // Extend space
        if (m_nBufferCapacity * 2 > MAX_PARTICLE)
        {
            // When the upper limit is exceeded, the last value is returned
            return m_pParticleBuffer + m_nParticleCount - 1;
        }
        m_nBufferCapacity *= 2;
        BBSPHParticle *pNewData = new BBSPHParticle[m_nBufferCapacity];
        memcpy(pNewData, m_pParticleBuffer, m_nParticleCount * sizeof(BBSPHParticle));
        BB_SAFE_DELETE_ARRAY(m_pParticleBuffer);
        m_pParticleBuffer = pNewData;
    }
    // insert new particle
    BBSPHParticle *pParticle = m_pParticleBuffer + m_nParticleCount;
    m_nParticleCount++;
    pParticle->m_Position = QVector3D(0, 0, 0);
    // The next of the last particle points to the header
    pParticle->m_nNextIndex = 0;
    pParticle->m_Velocity = QVector3D(0, 0, 0);
    pParticle->m_FinalVelocity = QVector3D(0, 0, 0);
    pParticle->m_fPressure = 0.0f;
    pParticle->m_fDensity = 0.0f;
    pParticle->m_Acceleration = QVector3D(0, 0, 0);

    return pParticle;
}

BBSPHParticle* BBSPHParticleSystem::getParticle(unsigned int nIndex)
{
    return m_pParticleBuffer + nIndex;
}
