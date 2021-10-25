#ifndef BBSPHFLUIDSYSTEM_H
#define BBSPHFLUIDSYSTEM_H


#include "Base/BBGameObject.h"

class BBSPHParticle;
class BBSPHParticleSystem;
class BBSPHGridContainer;
class BBSPHParticleNeighborTable;
class BBSPHFluidRenderer;
class BBMarchingCubeMesh;

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

    void reset();

public:
    inline BBSPHFluidRenderer* getFluidRenderer() { return m_pFluidRenderer; }

private:
    void initFluidVolume(const QVector3D &fluidBoxMin, const QVector3D &fluidBoxMax, float fSpacing);

private:
    void computeNeighborTable();
    void computeDensityAndPressure();
    void computeAcceleration();
    void computeBoundaryForce(BBSPHParticle *pParticle);
    void handleCollision(BBSPHParticle* pParticle);
    void computePositionAndVelocity();
    void computePositionAndVelocityWithGravity();

private:
    void computeImplicitField(unsigned int pNum[3], const QVector3D &minPos, const QVector3D &unitWidth);
    // Density field
    float computeColorField(const QVector3D &pos);

private:
    // Anisotropic surface
    void computeAnisotropicKernel();

    std::vector<QVector3D> m_OldPositions;
    bool m_bAnisotropic;
    // Transform Matrixs
    std::vector<QMatrix3x3> m_G;

private:
    // Paper: Predictive-Corrective Incompressible SPH
    void computeGradient();
    void computePCISPHDensityErrorFactor();

    void computePCISPHAcceleration();

    void predictPCISPHCorrection();
    void predictPCISPHPositionAndVelocity(BBSPHParticle *pParticle);
    float predictPCISPHDensityAndPressure(int nParticleIndex);
    void computePCISPHCorrectivePressureForce(int nParticleIndex);
    void computePCISPHPositionAndVelocity();

    float m_fPCISPHDensityErrorFactor;
    bool m_bPredictCorrection;

private:
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
    QVector3D m_OriginalFluidBoxMin;
    QVector3D m_OriginalFluidBoxMax;
    float m_fDeltaTime;

    BBSPHFluidRenderer *m_pFluidRenderer;

    unsigned int m_pFieldSize[3];
    BBMarchingCubeMesh *m_pMCMesh;
    float *m_pDensityField;
    float m_fDensityThreshold;
};

#endif // BBSPHFLUIDSYSTEM_H
