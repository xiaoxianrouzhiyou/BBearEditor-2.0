#ifndef BBSPHPARTICLENEIGHBORTABLE_H
#define BBSPHPARTICLENEIGHBORTABLE_H


#define MAX_NEIGHBOR_COUNT 80

struct BBNeighborDataBufferCode
{
    unsigned int m_nNeighborDataBufferSize;
    unsigned int m_nNeighborDataBufferOffset;
};

class BBSPHParticleNeighborTable
{
public:
    BBSPHParticleNeighborTable();
    virtual ~BBSPHParticleNeighborTable();

    void addNeighborParticle(unsigned int nNeighborIndex, float fNeighborDistance);
    // current -> m_pNeighborDataBuffer; record code
    void recordCurrentNeighbor();

private:
    void extendNeighborDataBuffer(unsigned int nNeedSize);

    unsigned int m_nParticleCount;
    unsigned int m_nParticleCapacity;

    unsigned char *m_pNeighborDataBuffer;
    unsigned int m_nNeighborDataBufferSize;
    unsigned int m_nNeighborDataBufferOffset;

    // Guide how to get data from m_pNeighborDataBuffer
    BBNeighborDataBufferCode *m_pNeighborDataBufferCode;

    // The data of current particle
    unsigned int m_nCurrentParticleIndex;
    int m_nCurrentNeighborCount;
    unsigned int m_CurrentNeighborIndexes[MAX_NEIGHBOR_COUNT];
    float m_CurrentNeighborDistances[MAX_NEIGHBOR_COUNT];
};

#endif // BBSPHPARTICLENEIGHBORTABLE_H
