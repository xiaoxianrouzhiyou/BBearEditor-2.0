#include "BBSPHParticleNeighborTable.h"
#include <string.h>
#include "Utils/BBUtils.h"

BBSPHParticleNeighborTable::BBSPHParticleNeighborTable()
{
    m_nParticleCount = 0;
    m_nParticleCapacity = 0;
    m_pNeighborDataBuffer = nullptr;
    m_nNeighborDataBufferSize = 0;
    m_nNeighborDataBufferOffset = 0;
    m_pNeighborDataBufferCode = nullptr;
    m_nCurrentParticleIndex = 0;
    m_nCurrentNeighborCount = 0;
}

BBSPHParticleNeighborTable::~BBSPHParticleNeighborTable()
{
    BB_SAFE_DELETE_ARRAY(m_pNeighborDataBuffer);
    BB_SAFE_DELETE_ARRAY(m_pNeighborDataBufferCode);
}

void BBSPHParticleNeighborTable::reset(unsigned int nParticleCount)
{
    int nSize = sizeof(BBNeighborDataBufferCode);
    if (nParticleCount > m_nParticleCapacity)
    {
        // extend space
        if (m_pNeighborDataBufferCode)
        {
            BB_SAFE_DELETE_ARRAY(m_pNeighborDataBufferCode);
        }
        m_pNeighborDataBufferCode = new BBNeighborDataBufferCode[nSize * nParticleCount];
        m_nParticleCapacity = nParticleCount;
    }
    m_nParticleCount = nParticleCount;
    memset(m_pNeighborDataBufferCode, 0, nSize * m_nParticleCapacity);
    m_nNeighborDataBufferOffset = 0;
}

void BBSPHParticleNeighborTable::setCurrentParticle(unsigned int nParticleIndex)
{
    m_nCurrentParticleIndex = nParticleIndex;
    m_nCurrentNeighborCount = 0;
}

bool BBSPHParticleNeighborTable::addNeighborParticle(unsigned int nNeighborIndex, float fNeighborDistance)
{
    if (m_nCurrentNeighborCount >= MAX_NEIGHBOR_COUNT)
    {
        return false;
    }
    m_CurrentNeighborIndexes[m_nCurrentNeighborCount] = nNeighborIndex;
    m_CurrentNeighborDistances[m_nCurrentNeighborCount] = fNeighborDistance;
    m_nCurrentNeighborCount++;
    return true;
}

void BBSPHParticleNeighborTable::recordCurrentNeighbor()
{
    if (m_nCurrentNeighborCount == 0)
        return;

    unsigned int nNeighborIndexSize = m_nCurrentNeighborCount * sizeof(unsigned int);
    unsigned int nNeighborDistanceSize = m_nCurrentNeighborCount * sizeof(float);

    if (m_nNeighborDataBufferOffset + nNeighborIndexSize + nNeighborDistanceSize > m_nNeighborDataBufferSize)
    {
        extendNeighborDataBuffer(m_nNeighborDataBufferOffset + nNeighborIndexSize + nNeighborDistanceSize);
    }

    // record code
    m_pNeighborDataBufferCode[m_nCurrentParticleIndex].m_nCurrentNeighborCount = m_nCurrentNeighborCount;
    m_pNeighborDataBufferCode[m_nCurrentParticleIndex].m_nNeighborDataBufferOffset = m_nNeighborDataBufferOffset;

    // Copy the information of the current to the data buffer
    memcpy(m_pNeighborDataBuffer + m_nNeighborDataBufferOffset, m_CurrentNeighborIndexes, nNeighborIndexSize);
    m_nNeighborDataBufferOffset += nNeighborIndexSize;
    memcpy(m_pNeighborDataBuffer + m_nNeighborDataBufferOffset, m_CurrentNeighborDistances, nNeighborDistanceSize);
    m_nNeighborDataBufferOffset += nNeighborDistanceSize;
}

int BBSPHParticleNeighborTable::getNeighborCount(unsigned int nCurrentParticleIndex)
{
    return m_pNeighborDataBufferCode[nCurrentParticleIndex].m_nCurrentNeighborCount;
}

void BBSPHParticleNeighborTable::getNeighborInfo(unsigned int nCurrentParticleIndexInBuffer,
                                                 unsigned int nNeighborParticleIndexInNeighborTable,
                                                 unsigned int &nNeighborParticleIndexInBuffer,
                                                 float &fNeighborParticleDistance)
{
    BBNeighborDataBufferCode code = m_pNeighborDataBufferCode[nCurrentParticleIndexInBuffer];
    unsigned int *pNeighborIndex = (unsigned int*)(m_pNeighborDataBuffer + code.m_nNeighborDataBufferOffset);
    float *pNeighborDistance = (float*)(m_pNeighborDataBuffer + code.m_nNeighborDataBufferOffset + sizeof(unsigned int) * code.m_nCurrentNeighborCount);

    nNeighborParticleIndexInBuffer = pNeighborIndex[nNeighborParticleIndexInNeighborTable];
    fNeighborParticleDistance = pNeighborDistance[nNeighborParticleIndexInNeighborTable];
}

void BBSPHParticleNeighborTable::extendNeighborDataBuffer(unsigned int nNeedSize)
{
    unsigned int nNewSize = m_nNeighborDataBufferSize > 0 ? m_nNeighborDataBufferSize : 1;
    while (nNewSize < nNeedSize)
    {
        nNewSize *= 2;
    }
    unsigned char *pNewBuffer = new unsigned char[nNewSize];
    if (m_pNeighborDataBuffer)
    {
        memcpy(pNewBuffer, m_pNeighborDataBuffer, m_nNeighborDataBufferSize);
        BB_SAFE_DELETE_ARRAY(m_pNeighborDataBuffer);
    }
    m_pNeighborDataBuffer = pNewBuffer;
    m_nNeighborDataBufferSize = nNewSize;
}
