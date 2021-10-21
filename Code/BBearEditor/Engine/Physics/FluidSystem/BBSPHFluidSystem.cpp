#include "BBSPHFluidSystem.h"
#include "BBSPHParticleSystem.h"
#include "BBSPHGridContainer.h"
#include "BBSPHParticleNeighborTable.h"
#include "Utils/BBUtils.h"
#include "BBSPHFluidRenderer.h"
#include "Geometry/BBMarchingCubeMesh.h"
#include "Math/BBMatrix.h"
#include "Math/BBMath.h"


BBSPHFluidSystem::BBSPHFluidSystem(const QVector3D &position)
    : BBGameObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_pParticleSystem = new BBSPHParticleSystem();
    m_pGridContainer = new BBSPHGridContainer();
    m_pParticleNeighborTable = new BBSPHParticleNeighborTable();

    m_fParticleRadius = 0.0f;
    m_fParticleMass = 0.0003f;
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

    m_fDeltaTime = BB_CONSTANT_UPDATE_RATE / 10000.0f;
    m_pFluidRenderer = new BBSPHFluidRenderer(position);

    m_pMCMesh = nullptr;
    m_pDensityField = nullptr;
    m_fDensityThreshold = 0.5f;

    m_bAnisotropic = false;

    m_fPCISPHDensityErrorFactor = 0.0f;
    m_bPredictCorrection = true;
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

    m_pGridContainer->insertParticles(m_pParticleSystem);
    m_pParticleNeighborTable->reset(m_pParticleSystem->getSize());

    // PCISPH
    computeGradient();
    computePCISPHDensityErrorFactor();
}

void BBSPHFluidSystem::render(BBCamera *pCamera)
{
    m_pGridContainer->insertParticles(m_pParticleSystem);
    m_pParticleNeighborTable->reset(m_pParticleSystem->getSize());

    if (m_bPredictCorrection)
    {
        computeNeighborTable();
        // 1. Calculate the neighbor information of each particle and record it in the neighbor table ---- computeGradient() in init()
        // 2. Calculate the acceleration caused by all forces except pressure (viscosity, gravity, collision)
        computePCISPHAcceleration();
        // 3. Correction loops
        predictPCISPHCorrection();
        // 4. The corrected pressure is used to calculate the particle velocity and position
        computePCISPHPositionAndVelocity();
    }
    else
    {
        computeDensityAndPressure();
        computeAcceleration();
        computePositionAndVelocity();
    }

    if (m_bAnisotropic)
        computeAnisotropicKernel();

    computeImplicitField(m_pFieldSize, m_WallBoxMin, 0.25f * m_pGridContainer->getGridDelta());
    m_pMCMesh->createMCMesh(m_pDensityField);

    m_pFluidRenderer->render(pCamera);
//    m_pMCMesh->render(pCamera);

    if (m_bAnisotropic)
    {
        // restore original position
        for (int i = 0; i < m_OldPositions.size(); i++)
        {
            BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);
            pParticle->m_Position = m_OldPositions[i];
        }
    }
}

void BBSPHFluidSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pFluidRenderer->setPosition(position, bUpdateLocalTransform);
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

void BBSPHFluidSystem::computeNeighborTable()
{
    // h ^ 2
    float h2 = m_fSmoothRadius * m_fSmoothRadius;
    // Traverse all particles
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pCurrentParticle = m_pParticleSystem->getParticle(i);
        int pGridCell[8];
        m_pGridContainer->findCells(pCurrentParticle->m_Position, m_fSmoothRadius / m_fUnitScale, pGridCell);
        m_pParticleNeighborTable->setCurrentParticle(i);
        float sum = 0.0;
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
                if (pCurrentParticle == pNeighborParticle)
                {
                    sum += pow(h2, 3.0);
                }
                else
                {
                    QVector3D pi_pj = (pCurrentParticle->m_Position - pNeighborParticle->m_Position) * m_fUnitScale;
                    float r2 = pi_pj.lengthSquared();
                    if (r2 < h2)
                    {
                        // Insert pNeighborParticle into the NeighborTable of pCurrentParticle
                        sum += pow(h2 - r2, 3.0);
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
        pCurrentParticle->m_fKernel = sum * m_fKernelPoly6;
    }
}

/**
 * @brief BBSPHFluidSystem::computeDensityAndPressure                 Pressure, density, and neighbor
 */
void BBSPHFluidSystem::computeDensityAndPressure()
{
    // h ^ 2
    float h2 = m_fSmoothRadius * m_fSmoothRadius;
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

            acceleration += (P * ri_rj + V * (pNeighborParticle->m_EvalVelocity - pCurrentParticle->m_EvalVelocity)) * D;
        }

        acceleration += m_Gravity;
        pCurrentParticle->m_Acceleration = acceleration;

        computeBoundaryForce(pCurrentParticle);
    }
}

void BBSPHFluidSystem::computeBoundaryForce(BBSPHParticle *pParticle)
{
    float fSpeedLimiting2 = m_fSpeedLimiting * m_fSpeedLimiting;

    QVector3D acceleration = pParticle->m_Acceleration;
    float fAcceleration2 = acceleration.lengthSquared();
    // Limit speed
    if (fAcceleration2 > fSpeedLimiting2)
        acceleration *= m_fSpeedLimiting / sqrtf(fAcceleration2);

    // When particle is in the edge
    float diff;
    float threshold = 1.0f;
    diff = threshold - (pParticle->m_Position.z() - m_WallBoxMin.z());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(0, 0, 1);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    diff = threshold - (m_WallBoxMax.z() - pParticle->m_Position.z());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(0, 0, -1);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    diff = threshold - (pParticle->m_Position.x() - m_WallBoxMin.x());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(1, 0, 0);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    diff = threshold - (m_WallBoxMax.x() - pParticle->m_Position.x());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(-1, 0, 0);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    diff = threshold - (pParticle->m_Position.y() - m_WallBoxMin.y());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(0, 1, 0);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    diff = threshold - (m_WallBoxMax.y() - pParticle->m_Position.y());
    diff *= m_fUnitScale;
    if (diff > 0.0f)
    {
        QVector3D normal(0, -1, 0);
        float coefficient = m_fBoundingStiffness * diff - m_fBoundingDamping * QVector3D::dotProduct(normal, pParticle->m_EvalVelocity);
        acceleration += coefficient * normal;
    }

    pParticle->m_Acceleration = acceleration;
}

void BBSPHFluidSystem::handleCollision(BBSPHParticle* pParticle)
{
    static float fDamping = 0.1f;
    for (int i = 0; i < 3; i++)
    {
        if (pParticle->m_PredictedPosition[i] < m_WallBoxMin[i])
        {
            pParticle->m_PredictedPosition[i] = m_WallBoxMin[i];
            pParticle->m_Velocity[i] *= fDamping;
        }
        if (pParticle->m_PredictedPosition[i] > m_WallBoxMax[i])
        {
            pParticle->m_PredictedPosition[i] = m_WallBoxMax[i];
            pParticle->m_Velocity[i] *= fDamping;
        }
    }
}

void BBSPHFluidSystem::computePositionAndVelocity()
{
    // Traverse all particles
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);
        // v(t+1/2) = v(t-1/2) + a(t)dt
        QVector3D v = pParticle->m_Velocity + pParticle->m_Acceleration * m_fDeltaTime;
        // v(t+1) = [v(t-1/2) + v(t+1/2)] * 0.5
        pParticle->m_EvalVelocity = (v + pParticle->m_Velocity) * 0.5f;
        pParticle->m_Velocity = v;
        // p(t+1) = p(t) + v(t+1/2)dt
        pParticle->m_Position += v * m_fDeltaTime / m_fUnitScale;
    }
}

void BBSPHFluidSystem::computePositionAndVelocityWithGravity()
{
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle* pParticle = m_pParticleSystem->getParticle(i);
        pParticle->m_Acceleration += m_Gravity;
        pParticle->m_Velocity += pParticle->m_Acceleration * m_fDeltaTime;
        pParticle->m_Position += pParticle->m_Velocity * m_fDeltaTime / m_fUnitScale;
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

            if (m_bAnisotropic)
            {
                QMatrix3x3 G = m_G[nNeighborParticleIndex];
                QVector3D rij = pos - pNeighborParticle->m_Position;
                // Transform to ellipse
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        rij[i] += G(i, j) * rij[j];
                    }
                }
                float r = rij.length() * m_fUnitScale;
                float q = h2 - r * r;
                if (q > 0)
                {
                    float determinant = G(0, 0) * G(1, 1) * G(2, 2) + G(0, 1) * G(1, 2) * G(2, 0) + G(0, 2) * G(1, 0) * G(2, 1)
                                      - G(0, 0) * G(1, 2) * G(2, 1) - G(0, 1) * G(1, 0) * G(2, 2) - G(0, 2) * G(1, 1) * G(2, 0);
                    c += determinant * m_fParticleMass * m_fKernelPoly6 * q * q * q;
                }
            }
            else
            {
                // isotropic
                float r = pos.distanceToPoint(pNeighborParticle->m_Position) * m_fUnitScale;
                float q = h2 - r * r;
                if (q > 0)
                {
                    c += m_fParticleMass * m_fKernelPoly6 * q * q * q;
                }
            }

            nNeighborParticleIndex = pNeighborParticle->m_nNextIndex;
        }
    }

    return c;
}

void BBSPHFluidSystem::computeAnisotropicKernel()
{
    if (m_pParticleSystem->getSize() == 0)
        return;

    float h0 = m_fSmoothRadius;
    float h = 2.0 * h0;

    m_G.resize(m_pParticleSystem->getSize());

    // Laplace smoothing coefficient
    float lambda = 0.9f;
    // New particle position by smoothing
    QVector3D positions[m_pParticleSystem->getSize()];
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        QVector3D pos0 = m_pParticleSystem->getParticle(i)->m_Position;
        int pGridCell[64];
        m_pGridContainer->findTwoCells(pos0, h / m_fUnitScale, pGridCell);

        QVector3D posw(0, 0, 0);
        float sumw = 0.0f;

        for (int cell = 0; cell < 64; cell++)
        {
            if (pGridCell[cell] < 0)
                continue;
            int nNeighborParticleIndex = m_pGridContainer->getGridData(pGridCell[cell]);
            while (nNeighborParticleIndex != -1)
            {
                BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborParticleIndex);

                QVector3D pos1 = pNeighborParticle->m_Position;
                float r = pos0.distanceToPoint(pos1) * m_fUnitScale;
                if (r < h)
                {
                    float q = 1 - r / h;
                    float wij = q * q * q;
                    posw += pos1 * wij;
                    sumw += wij;
                }

                nNeighborParticleIndex = pNeighborParticle->m_nNextIndex;
            }
        }

        positions[i] = posw / sumw;
        positions[i] = (1 - lambda) * m_pParticleSystem->getParticle(i)->m_Position + lambda * positions[i];
    }

    m_OldPositions.resize(m_pParticleSystem->getSize());
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);
        m_OldPositions[i] = pParticle->m_Position;
        pParticle->m_Position = positions[i];
    }
    m_pGridContainer->insertParticles(m_pParticleSystem);

    // compute covariance matrix
    for (unsigned int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        QVector3D xi = positions[i];
        QVector3D xiw(0, 0, 0);
        float sumw = 0.0f;
        int pGridCell[64];
        m_pGridContainer->findTwoCells(xi, h / m_fUnitScale, pGridCell);
        for (int cell = 0; cell < 64; cell++)
        {
            if (pGridCell[cell] < 0)
                continue;
            int nNeighborParticleIndex = m_pGridContainer->getGridData(pGridCell[cell]);
            while (nNeighborParticleIndex != -1)
            {
                QVector3D xj = positions[nNeighborParticleIndex];
                float r = xi.distanceToPoint(xj) * m_fUnitScale;
                if (r < h)
                {
                    float q = 1 - r / h;
                    float wij = q * q * q;
                    xiw += xj * wij;
                    sumw += wij;
                }

                nNeighborParticleIndex = m_pParticleSystem->getParticle(nNeighborParticleIndex)->m_nNextIndex;
            }
        }
        xiw /= sumw;

        QMatrix3x3 c;
        c.fill(0.0f);
        sumw = 0.0f;
        // The number of neighbor particles
        int n = 0;
        for (int cell = 0; cell < 64; cell++)
        {
            if (pGridCell[cell] < 0)
                continue;
            int nNeighborParticleIndex = m_pGridContainer->getGridData(pGridCell[cell]);
            while (nNeighborParticleIndex != -1)
            {
                QVector3D xj = positions[nNeighborParticleIndex];
                float r = xi.distanceToPoint(xj) * m_fUnitScale;
                if (r < h)
                {
                    float q = 1 - r / h;
                    float wij = q * q * q;
                    QVector3D dxj = xj - xiw;

                    for (int k = 0; k < 3; k++)
                    {
                        c(0, k) += wij * dxj[0] * dxj[k];
                        c(1, k) += wij * dxj[1] * dxj[k];
                        c(2, k) += wij * dxj[2] * dxj[k];
                    }
                    n++;
                    sumw += wij;
                }

                nNeighborParticleIndex = m_pParticleSystem->getParticle(nNeighborParticleIndex)->m_nNextIndex;
            }
        }
        c /= sumw;

        // SVD
        float w[3], u[9], v[9];
        for (int j = 0; j < 3; j++)
        {
            u[j * 3 + 0] = c(0, j);
            u[j * 3 + 1] = c(1, j);
            u[j * 3 + 2] = c(2, j);
        }
        BBMatrix::SVDdecomposition(w, u, v, 1.0e-10);

        // Eigenvalue Σ
        QVector3D eigenvalue(w[0], w[1], w[2]);
        // C = R Σ R^T
        // Eigenvector (rotation matrix R)
        QMatrix3x3 R;
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                R(j, k) = u[k * 3 + j];
            }
        }

        // threshold
        int ne = 25;
        float ks = 1400;
        float kn = 0.5f;
        float kr = 4.0f;
        if (n > ne)
        {
            // When the maximum variance on main axis ([0]) is greater than kr times the minimum variance on the other axis
            // The ratio between any two eigenvalues is less than kr
            eigenvalue[1] = max(eigenvalue[1], eigenvalue[0] / kr);
            eigenvalue[2] = max(eigenvalue[2], eigenvalue[0] / kr);
            // The matrix norm of ksC is approximately equal to 1
            // The matrix norm reflects the linear mapping of one vector to another, and the scale of the "length" of the vector
            // So that the volume of particles with complete neighborhoods remains constant.
            eigenvalue *= ks;
        }
        else
        {
            // When there are fewer particles nearby
            // Reset to spherical to prevent poor particle deformation for almost isolated particles
            eigenvalue = QVector3D(1, 1, 1) * kn;
        }

        // Kernel Matrix G = R Σ^-1 R^T
        QMatrix3x3 sigma;
        sigma(0, 0) = 1.0f / eigenvalue.x(), sigma(0, 1) = 0.0f,                  sigma(0, 2) = 0.0f;
        sigma(1, 0) = 0.0f,                  sigma(1, 1) = 1.0f / eigenvalue.y(), sigma(1, 2) = 0.0f;
        sigma(2, 0) = 0.0f,                  sigma(2, 1) = 0.0f,                  sigma(2, 2) = 1.0f / eigenvalue.z();
        QMatrix3x3 G = R * sigma * R.transposed();

        float limiting = -1.0e10;
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (G(k, j) > limiting)
                    limiting = G(k, j);
            }
        }
        G /= limiting;

        m_G[i] = G;
    }
}

void BBSPHFluidSystem::computeGradient()
{
    // Σ▽wij▽wij & Σ▽wij
    computeNeighborTable();
    float h = m_fSmoothRadius;
    float h2 = h * h;
    for (int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);

        int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(i);
        for (unsigned int j = 0; j < nNeighborCount; j++)
        {
            unsigned int nNeighborIndex = -1;
            float r = 0.0f;
            m_pParticleNeighborTable->getNeighborInfo(i, j, nNeighborIndex, r);
            BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborIndex);

            QVector3D pi_pj = (pParticle->m_Position - pNeighborParticle->m_Position) * m_fUnitScale;
            float r2 = pi_pj.lengthSquared();
            if (r2 < h2)
            {
                float r = sqrtf(r2);
                QVector3D gradient = (pParticle->m_Position - pNeighborParticle->m_Position) * m_fKernelSpiky / r * (h - r) * (h - r);
                pParticle->m_SumGradient += gradient;
                pNeighborParticle->m_SumGradient -= gradient;
                pParticle->m_SumGradient2 += QVector3D::dotProduct(gradient, gradient);
                pNeighborParticle->m_SumGradient2 += QVector3D::dotProduct(-gradient, -gradient);
            }
        }
    }
}

/**
 * @brief BBSPHFluidSystem::computePCISPHDensityErrorFactor         When the number of neighbor particles is insufficient, it will lead to calculation errors
 *                                                                  So when initializing the fluid, that is, when the fluid particles are filled with neighbor particles, the coefficient is calculated
 */
void BBSPHFluidSystem::computePCISPHDensityErrorFactor()
{
    int nMaxNeighborCount = 0;
    int nMaxNeighborCountCurrentParticleIndex = 0;
    for (int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(i);
        if (nNeighborCount > nMaxNeighborCount)
        {
            nMaxNeighborCount = nNeighborCount;
            nMaxNeighborCountCurrentParticleIndex = i;
        }
    }

    BBSPHParticle *pMaxParticle = m_pParticleSystem->getParticle(nMaxNeighborCountCurrentParticleIndex);
    // According to formula
    float volume = m_fParticleMass / m_fStaticDensity;
    float factor = 2.0f * volume * volume * m_fDeltaTime * m_fDeltaTime;
    float divisor = -QVector3D::dotProduct(pMaxParticle->m_SumGradient, pMaxParticle->m_SumGradient) - pMaxParticle->m_SumGradient2;
    divisor *= factor;
    m_fPCISPHDensityErrorFactor = -1.0f / divisor;
}

/**
 * @brief BBSPHFluidSystem::computePCISPHAcceleration                   not including pressure
 */
void BBSPHFluidSystem::computePCISPHAcceleration()
{
    float h = m_fSmoothRadius;
    for (int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);

        pParticle->m_Acceleration = QVector3D(0, 0, 0);
        float volume = m_fParticleMass / m_fStaticDensity;

        int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(i);
        for (unsigned int j = 0; j < nNeighborCount; j++)
        {
            unsigned int nNeighborIndex = -1;
            float r = 0.0f;
            m_pParticleNeighborTable->getNeighborInfo(i, j, nNeighborIndex, r);
            BBSPHParticle *pNeighborParticle = m_pParticleSystem->getParticle(nNeighborIndex);

            float V = m_fKernelViscosity * m_fViscosity * (h - r) * volume * volume;
            pParticle->m_Acceleration -= (pParticle->m_Velocity - pNeighborParticle->m_Velocity) * V / m_fParticleMass;
        }

        computeBoundaryForce(pParticle);

        pParticle->m_Acceleration += m_Gravity;

        // init
        pParticle->m_fCorrectPressure = 0.0f;
        pParticle->m_CorrectPressureForce = QVector3D(0, 0, 0);
    }
}

void BBSPHFluidSystem::predictPCISPHCorrection()
{
    static int nMinLoops = 3;
    static int nMaxLoops = 100;
    static float fAllowedMaxDensityError = 1.0f;

    int nIteration = 0;
    bool bEndLoop = false;  

    while ((nIteration < nMinLoops) || ((nIteration < nMaxLoops) && !bEndLoop))
    {
        for (int i = 0; i < m_pParticleSystem->getSize(); i++)
        {
            BBSPHParticle *pParticle = m_pParticleSystem->getParticle(i);
            predictPCISPHPositionAndVelocity(pParticle);
        }

        float fMaxError = 0.0f;
        for (int i = 0; i < m_pParticleSystem->getSize(); i++)
        {
            float fError = predictPCISPHDensityAndPressure(i);
            fMaxError = max(fMaxError, fError);
        }

        // Less than the density error threshold, break
        if (max(0.1f * fMaxError - 100.0f, 0.0f) < fAllowedMaxDensityError)
        {
            bEndLoop = true;
        }

        for (int i = 0; i < m_pParticleSystem->getSize(); i++)
        {
            computePCISPHCorrectivePressureForce(i);
        }

        nIteration++;
    }
}

void BBSPHFluidSystem::predictPCISPHPositionAndVelocity(BBSPHParticle *pParticle)
{
    // v' = v + t * a
    // a = F / m
    // v' = v + t * F / m
    // v' = v + t * F * V / m
    // v' = v + t * F * 1 / density
    QVector3D F = m_fParticleMass * pParticle->m_Acceleration + pParticle->m_CorrectPressureForce;
    QVector3D predictedVelocity = pParticle->m_Velocity + m_fDeltaTime * F / m_fParticleMass;
    pParticle->m_PredictedPosition = pParticle->m_Position + m_fDeltaTime * predictedVelocity;

    // Collision
    handleCollision(pParticle);
}

float BBSPHFluidSystem::predictPCISPHDensityAndPressure(int nParticleIndex)
{
    float h2 = m_fSmoothRadius * m_fSmoothRadius;

    BBSPHParticle* pParticle = m_pParticleSystem->getParticle(nParticleIndex);

    pParticle->m_fPredictedDensity = pParticle->m_fKernel * m_fParticleMass;

    int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(nParticleIndex);
    for (unsigned int j = 0; j < nNeighborCount; j++)
    {
        unsigned int nNeighborIndex = -1;
        float r = 0.0f;
        m_pParticleNeighborTable->getNeighborInfo(nParticleIndex, j, nNeighborIndex, r);
        BBSPHParticle* pNeighborParticle = m_pParticleSystem->getParticle(nNeighborIndex);

        // Use the predicted position to recalculate the distance and kernel
        QVector3D pi_pj = (pParticle->m_PredictedPosition - pNeighborParticle->m_PredictedPosition) * m_fUnitScale;
        float r2 = pi_pj.lengthSquared();
        if (r2 < h2)
        {
            pParticle->m_fPredictedDensity += m_fKernelPoly6 * pow(h2 - r2, 3.0) * m_fParticleMass;
        }
    }

    // Consider only positive errors
    pParticle->m_fDensityError = max(pParticle->m_fPredictedDensity - m_fStaticDensity, 0.0f);
    // Correct positive pressure
    pParticle->m_fCorrectPressure += max(pParticle->m_fDensityError * m_fPCISPHDensityErrorFactor, 0.0f);

    return pParticle->m_fPredictedDensity;
}

void BBSPHFluidSystem::computePCISPHCorrectivePressureForce(int nParticleIndex)
{
    float h = m_fSmoothRadius;
    float h2 = m_fSmoothRadius * m_fSmoothRadius;
    float fStaticDensity2 = m_fStaticDensity * m_fStaticDensity;

    BBSPHParticle* pParticle = m_pParticleSystem->getParticle(nParticleIndex);
    pParticle->m_CorrectPressureForce = QVector3D(0, 0, 0);

    int nNeighborCount = m_pParticleNeighborTable->getNeighborCount(nParticleIndex);
    for (unsigned int j = 0; j < nNeighborCount; j++)
    {
        unsigned int nNeighborIndex = -1;
        float r = 0.0f;
        m_pParticleNeighborTable->getNeighborInfo(nParticleIndex, j, nNeighborIndex, r);
        BBSPHParticle* pNeighborParticle = m_pParticleSystem->getParticle(nNeighborIndex);

        QVector3D pi_pj = (pParticle->m_Position - pNeighborParticle->m_Position) * m_fUnitScale;
        float r2 = pi_pj.lengthSquared();
        if (r2 < h2)
        {
            r = sqrtf(r2);
            QVector3D gradient = pi_pj * m_fKernelSpiky / r * (h - r) * (h - r);
            pParticle->m_CorrectPressureForce -= m_fParticleMass * m_fParticleMass * gradient
                * (pParticle->m_fCorrectPressure + pNeighborParticle->m_fCorrectPressure) / fStaticDensity2;
        }
    }
}

void BBSPHFluidSystem::computePCISPHPositionAndVelocity()
{
    for (int i = 0; i < m_pParticleSystem->getSize(); i++)
    {
        BBSPHParticle* pParticle = m_pParticleSystem->getParticle(i);
        pParticle->m_Acceleration += pParticle->m_CorrectPressureForce / m_fParticleMass;

        // v(t+1/2) = v(t-1/2) + a(t)dt
        QVector3D v = pParticle->m_Velocity + pParticle->m_Acceleration * m_fDeltaTime;
        // v(t+1) = [v(t-1/2) + v(t+1/2)] * 0.5
        pParticle->m_EvalVelocity = (v + pParticle->m_Velocity) * 0.5f;
        pParticle->m_Velocity = v;
        // p(t+1) = p(t) + v(t+1/2)dt
        pParticle->m_Position += v * m_fDeltaTime / m_fUnitScale;
    }
}
