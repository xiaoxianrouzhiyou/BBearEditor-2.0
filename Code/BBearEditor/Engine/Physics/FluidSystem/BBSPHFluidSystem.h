#ifndef BBSPHFLUIDSYSTEM_H
#define BBSPHFLUIDSYSTEM_H


#include "Base/BBGameObject.h"

class BBSPHParticleSystem;
class BBSPHGridContainer;
class BBSPHParticleNeighborTable;
class BBSPHFluidRenderer;

class BBSPHFluidSystem : public BBGameObject
{
public:
    BBSPHFluidSystem(const QVector3D &position = QVector3D(0, 0, 0));
    ~BBSPHFluidSystem();

    void init(unsigned int nMaxParticleCount,
              const QVector3D &wallBoxMin, const QVector3D &wallBoxMax,
              const QVector3D &originalFluidBoxMin, const QVector3D &originalFluidBoxMax,
              const QVector3D &gravity = QVector3D(0, -9.8f, 0));
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;

private:
    void computeDensityAndPressure();
    void computeAcceleration();
    void initFluidVolume(const QVector3D &fluidBoxMin, const QVector3D &fluidBoxMax, float fSpacing);

    void update();

    BBSPHParticleSystem *m_pParticleSystem;
    BBSPHGridContainer *m_pGridContainer;
    BBSPHParticleNeighborTable *m_pParticleNeighborTable;

    // SPH smooth kernel
    float m_fKernelPoly6;
    float m_fKernelSpiky;
    float m_fKernelViscosity;

    float m_fParticleRadius;
    float m_fParticleMass;
    float m_fUnitScale;
    float m_fViscosity;
    float m_fStaticDensity;
    float m_fSmoothRadius;
    float m_fGasConstantK;
    float m_fBoundingStiffness;
    float m_fBoundingDamping;
    float m_fSpeedLimiting;
    
    QVector3D m_Gravity;

    QVector3D m_Size;
    QVector3D m_WallBoxMin;
    QVector3D m_WallBoxMax;

    BBSPHFluidRenderer *m_pFluidRenderer;
};

#endif // BBSPHFLUIDSYSTEM_H
