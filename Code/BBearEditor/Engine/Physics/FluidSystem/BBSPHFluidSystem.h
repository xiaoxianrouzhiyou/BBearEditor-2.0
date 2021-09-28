#ifndef BBSPHFLUIDSYSTEM_H
#define BBSPHFLUIDSYSTEM_H


#include <QVector3D>

class BBSPHParticleSystem;
class BBSPHGridContainer;
class BBSPHParticleNeighborTable;

class BBSPHFluidSystem
{
public:
    BBSPHFluidSystem();
    ~BBSPHFluidSystem();

    void init(unsigned int nMaxParticleCount,
              const QVector3D &wallBoxMin, const QVector3D &wallBoxMax,
              const QVector3D &originalFluidBoxMin, const QVector3D &originalFluidBoxMax,
              const QVector3D &gravity);

private:
    void computePressure();
    void computeForce();
    void initFluidVolume(const QVector3D &fluidBoxMin, const QVector3D &fluidBoxMax, float fSpacing);

    void update();

    BBSPHParticleSystem *m_pParticleSystem;
    BBSPHGridContainer *m_pGridContainer;
    BBSPHParticleNeighborTable *m_pParticleNeighborTable;

    // SPH smooth kernel



};

#endif // BBSPHFLUIDSYSTEM_H
