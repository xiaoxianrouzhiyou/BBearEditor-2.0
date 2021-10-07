#include "BBSPHFluidSystem.h"
#include "BBSPHParticleSystem.h"
#include "BBSPHGridContainer.h"
#include "BBSPHParticleNeighborTable.h"
#include "Utils/BBUtils.h"
#include "BBSPHFluidRenderer.h"
#include "Geometry/BBMarchingCubeMesh.h"


BBSPHFluidSystem::BBSPHFluidSystem(const QVector3D &position)
    : BBGameObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_pParticleSystem = new BBSPHParticleSystem();
    m_pGridContainer = new BBSPHGridContainer();
    m_pParticleNeighborTable = new BBSPHParticleNeighborTable();

    m_fParticleRadius = 0.0f;
    m_fParticleMass = 0.0005f;
    m_fUnitScale = 0.02f;
    m_fViscosity = 0.2f;
    m_fStaticDensity = 1000.0f;
    m_fSmoothRadius = 0.01f;
    m_fGasConstantK = 1.0f;
    m_fBoundingStiffness = 10000.0f;
    m_fBoundingDamping = 256;
    m_fSpeedLimiting = 200;
    
    m_fKernelPoly6 = 315.0f / (64.0f * 3.141592f * pow(m_fSmoothRadius, 9));
    m_fKernelSpiky = -45.0f / (3.141592f * pow(m_fSmoothRadius, 6));
    m_fKernelViscosity = 45.0f / (3.141592f * pow(m_fSmoothRadius, 6));

    m_pFluidRenderer = new BBSPHFluidRenderer(position);

    m_pMCMesh = nullptr;
    m_pDensityField = nullptr;
    m_fDensityThreshold = 0.5f;
}

BBSPHFluidSystem::~BBSPHFluidSystem()
{
    BB_SAFE_DELETE(m_pParticleSystem);
    BB_SAFE_DELETE(m_pGridContainer);
    BB_SAFE_DELETE(m_pParticleNeighborTable);
    BB_SAFE_DELETE(m_pFluidRenderer);
    BB_SAFE_DELETE(m_pMCMesh);
    BB_SAFE_DELETE_ARRAY(m_pDensityField);
}

void BBSPHFluidSystem::init(unsigned int nMaxParticleCount,
                            const QVector3D &wallBoxMin, const QVector3D &wallBoxMax,
                            const QVector3D &originalFluidBoxMin, const QVector3D &originalFluidBoxMax,
                            const QVector3D &gravity)
{
    m_pParticleSystem->reset(nMaxParticleCount);
    m_WallBoxMin = wallBoxMin;
    m_WallBoxMax = wallBoxMax;
    m_Gravity = gravity;
    m_fParticleRadius = pow(m_fParticleMass / m_fStaticDensity, 1.0f / 3.0f);
    initFluidVolume(originalFluidBoxMin, originalFluidBoxMax, m_fParticleRadius / m_fUnitScale);
    m_pGridContainer->init(wallBoxMin, wallBoxMax, m_fUnitScale, m_fSmoothRadius * 2.0f, m_pFieldSize);

    m_pFluidRenderer->init(m_pParticleSystem);


    // Marching cubes
    m_pMCMesh = new BBMarchingCubeMesh();
    m_pDensityField = new float[(m_pFieldSize[0] + 1) * (m_pFieldSize[1] + 1) * (m_pFieldSize[2] + 1)];
    m_pMCMesh->init(m_pFieldSize, 0.25f * m_pGridContainer->getGridDelta(), m_WallBoxMin, m_fDensityThreshold);
}

void BBSPHFluidSystem::render(BBCamera *pCamera)
{
    m_pGridContainer->insertParticles(m_pParticleSystem);

    computeImplicitField(m_pFieldSize, m_WallBoxMin, 0.125f * m_pGridContainer->getGridDelta());
    m_pMCMesh->createMCMesh(m_pDensityField);

    computeDensityAndPressure();
    computeAcceleration();
    update();
    m_pFluidRenderer->render(pCamera);
}

void BBSPHFluidSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pFluidRenderer->setPosition(position, bUpdateLocalTransform);
}

/**
 * @brief BBSPHFluidSystem::computeDensityAndPressure                 Pressure, density, and neighbor
 */
void BBSPHFluidSystem::computeDensityAndPressure()
{
    // h ^ 2
    float h2 = m_fSmoothRadius * m_fSmoothRadius;
    m_pParticleNeighborTable->reset(m_pParticleSystem->getSize());
    // Traverse all particles
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pCurrentParticle = m_pParticleSystem->getParticle(i);
        int pGridCell[8];
        m_pGridContainer->findCells(pCurrentParticle->m_Position, m_fSmoothRadius / m_fUnitScale, pGridCell);
        m_pParticleNeighborTable->setCurrentParticle(i);
        // According to SPH formula
        float fDensity = 0.001f;
        for (int cell = 0; cell < 8; cell++)
        {
            if (pGridCell[cell] == -1)
            {
                // edge
                continue;
            }
            // The head of index list of particles that are in the current grid
            int nNeighborParticleIndex = m_pGridContainer->getGridData(pGridCell[cell]);
            // Traverse the list
            while (nNeighborParticleIndex != -1)
            {
                BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborParticleIndex);
                if (pCurrentParticle != pNeighborParticle)
                {
                    QVector3D pi_pj = pCurrentParticle->m_Position - pNeighborParticle->m_Position;
                    pi_pj *= m_fUnitScale;
                    float r2 = pi_pj.lengthSquared();
                    if (r2 < h2)
                    {
                        fDensity += pow(h2 - r2, 3.0f);
                        // Insert pNeighborParticle into the NeighborTable of pCurrentParticle
                        if (!m_pParticleNeighborTable->addNeighborParticle(nNeighborParticleIndex, sqrtf(r2)))
                        {
                            goto NEIGHBOR_FULL;
                        }
                    }
                }

                nNeighborParticleIndex = pNeighborParticle->m_nNextIndex;
            }
        }

        NEIGHBOR_FULL:
        m_pParticleNeighborTable->recordCurrentNeighbor();
        // According to SPH formula
        pCurrentParticle->m_fDensity = m_fKernelPoly6 * m_fParticleMass * fDensity;
        pCurrentParticle->m_fPressure = (pCurrentParticle->m_fDensity - m_fStaticDensity) * m_fBoundingStiffness;
        pCurrentParticle->m_fDensity = 1.0f / pCurrentParticle->m_fDensity;
    }
}

void BBSPHFluidSystem::computeAcceleration()
{
    // Traverse all particles
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pCurrentParticle = m_pParticleSystem->getParticle(i);
        QVector3D acceleration(0, 0, 0);
        int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(i);
        for (unsigned int j = 0; j < nNeighborCount; j++)
        {
            unsigned int nNeighborIndex = -1;
            float r = 0.0f;
            m_pParticleNeighborTable->getNeighborInfo(i, j, nNeighborIndex, r);
            BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborIndex);
            // According to SPH formula
            QVector3D ri_rj = pCurrentParticle->m_Position - pNeighborParticle->m_Position;
            ri_rj *= m_fUnitScale;
            float h_r = m_fSmoothRadius - r;
            float P = -0.5f * h_r * m_fKernelSpiky * (pCurrentParticle->m_fPressure + pNeighborParticle->m_fPressure) / r;
            float D = h_r * pCurrentParticle->m_fDensity * pNeighborParticle->m_fDensity;
            float V = m_fKernelViscosity * m_fViscosity;

            acceleration += (P * ri_rj + V * (pNeighborParticle->m_FinalVelocity - pCurrentParticle->m_FinalVelocity)) * D;
        }
        pCurrentParticle->m_Acceleration = acceleration;
    }
}

void BBSPHFluidSystem::initFluidVolume(const QVector3D &fluidBoxMin, const QVector3D &fluidBoxMax, float fSpacing)
{
    for (float z = fluidBoxMin.z(); z < fluidBoxMax.z(); z += fSpacing)
    {
        for (float y = fluidBoxMin.y(); y < fluidBoxMax.y(); y += fSpacing)
        {
            for (float x = fluidBoxMin.x(); x < fluidBoxMax.x(); x += fSpacing)
            {
                m_pParticleSystem->addParticle(x, y, z);
            }
        }
    }
}

void BBSPHFluidSystem::update()
{
    float fDeltaTime = BB_CONSTANT_UPDATE_RATE / 10000.0f;
    float fSpeedLimiting2 = m_fSpeedLimiting * m_fSpeedLimiting;
    // Traverse all particles
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pCurrentParticle = m_pParticleSystem->getParticle(i);
        QVector3D acceleration = pCurrentParticle->m_Acceleration;
        float fAcceleration2 = acceleration.lengthSquared();
        // Limit speed
        if (fAcceleration2 > fSpeedLimiting2)
            acceleration *= m_fSpeedLimiting / sqrtf(fAcceleration2);

        // When particle is in the edge
        float diff;
        float threshold = 1.0f;
        diff = threshold - (pCurrentParticle->m_Position.z() - m_WallBoxMin.z());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(0, 0, 1);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        diff = threshold - (m_WallBoxMax.z() - pCurrentParticle->m_Position.z());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(0, 0, -1);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        diff = threshold - (pCurrentParticle->m_Position.x() - m_WallBoxMin.x());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(1, 0, 0);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        diff = threshold - (m_WallBoxMax.x() - pCurrentParticle->m_Position.x());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(-1, 0, 0);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        diff = threshold - (pCurrentParticle->m_Position.y() - m_WallBoxMin.y());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(0, 1, 0);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        diff = threshold - (m_WallBoxMax.y() - pCurrentParticle->m_Position.y());
        diff *= m_fUnitScale;
        if (diff > 0.0f)
        {
            QVector3D normal(0, -1, 0);
            float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pCurrentParticle->m_FinalVelocity);
            acceleration += coefficient * normal;
        }

        acceleration += m_Gravity;
        // v(t+1/2) = v(t-1/2) + a(t)dt
        QVector3D v = pCurrentParticle->m_Velocity + acceleration * fDeltaTime;
        // v(t+1) = [v(t-1/2) + v(t+1/2)] * 0.5
        pCurrentParticle->m_FinalVelocity = (v + pCurrentParticle->m_Velocity) * 0.5f;
        pCurrentParticle->m_Velocity = v;
        // p(t+1) = p(t) + v(t+1/2)dt
        pCurrentParticle->m_Position += v * fDeltaTime / m_fUnitScale;
    }
}

/**
 * @brief BBSPHFluidSystem::computeImplicitField                    Calculating implicit function values from particle
 * @param pNum                                                      Num
 * @param minPos
 * @param unitWidth
 * @param pOutField                                                 Array
 */
void BBSPHFluidSystem::computeImplicitField(unsigned int pNum[3], const QVector3D &minPos, const QVector3D &unitWidth)
{
    unsigned int nSlice0 = pNum[0] + 1;
    unsigned int nSlice1 = nSlice0 * (pNum[1] + 1);

    for (int k = 0; k < pNum[2]; k++)
    {
        for (int j = 0; j < pNum[1]; j++)
        {
            for (int i = 0; i < pNum[0]; i++)
            {
                QVector3D pos = minPos + QVector3D(i, j, k) * unitWidth;
                m_pDensityField[k * nSlice1 + j * nSlice0 + i] = computeColorField(pos);
            }
        }
    }
}

float BBSPHFluidSystem::computeColorField(const QVector3D &pos)
{
    // Density field
    float c = 0.0f;

    if (pos.x() < m_WallBoxMin.x())
        return c;
    if (pos.x() > m_WallBoxMax.x())
        return c;
    if (pos.y() < m_WallBoxMin.y())
        return c;
    if (pos.y() > m_WallBoxMax.y())
        return c;
    if (pos.z() < m_WallBoxMin.z())
        return c;
    if (pos.z() > m_WallBoxMax.z())
        return c;

    float h = m_fSmoothRadius;
    float h2 = h * h;

    int pGridCell[8];
    m_pGridContainer->findCells(pos, h / m_fUnitScale, pGridCell);
    // isotropic
    for (int cell = 0; cell < 8; cell++)
    {
        if (pGridCell[cell] < 0)
        {
            continue;
        }
        int nNeighborParticleIndex = m_pGridContainer->getGridData(pGridCell[cell]);
        // Traverse the list
        while (nNeighborParticleIndex != -1)
        {
            BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborParticleIndex);

            float r = pos.distanceToPoint(pNeighborParticle->m_Position) * m_fUnitScale;
            float q = h2 - r * r;
            if (q > 0)
            {
                c += m_fParticleMass * m_fKernelPoly6 * q * q * q;
            }

            nNeighborParticleIndex = pNeighborParticle->m_nNextIndex;
        }
    }

    return c;
}
