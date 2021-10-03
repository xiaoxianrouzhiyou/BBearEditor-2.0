#include "BBSPHGridContainer.h"
#include "BBSPHParticleSystem.h"
#include "Utils/BBUtils.h"


BBSPHGridContainer::BBSPHGridContainer()
{

}

void BBSPHGridContainer::init(const QVector3D &min, const QVector3D &max, float fUnitScale, float fGridCellSize, unsigned int *pFieldSize)
{
    m_GridMin = min;
    m_GridMax = max;
    m_GridSize = m_GridMax - m_GridMin;
    m_fGridCellSize = fGridCellSize / fUnitScale;

    m_GridResolution.setX(ceil(m_GridSize.x() / m_fGridCellSize));
    m_GridResolution.setY(ceil(m_GridSize.y() / m_fGridCellSize));
    m_GridResolution.setZ(ceil(m_GridSize.z() / m_fGridCellSize));

    // Resize the grid to a multiple of the cell size
    m_GridSize = m_GridResolution * m_fGridCellSize;

    m_GridDelta = m_GridResolution / m_GridSize;

    m_GridData.resize((int)(m_GridResolution.x() * m_GridResolution.y() * m_GridResolution.z()));

    pFieldSize[0] = m_GridResolution.x() * 8;
    pFieldSize[1] = m_GridResolution.y() * 8;
    pFieldSize[2] = m_GridResolution.z() * 8;
}

int BBSPHGridContainer::getGridData(int nGridIndex)
{
    if (nGridIndex < 0 | nGridIndex >= m_GridData.size())
    {
        return -1;
    }
    else
    {
        return m_GridData[nGridIndex];
    }
}

int BBSPHGridContainer::getGridCellIndex(const QVector3D &p)
{
    // Rounding
    int gx = (int)((p.x() - m_GridMin.x()) * m_GridDelta.x());
    int gy = (int)((p.y() - m_GridMin.y()) * m_GridDelta.y());
    int gz = (int)((p.z() - m_GridMin.z()) * m_GridDelta.z());
    return (gz * m_GridResolution.y() + gy) * m_GridResolution.x() + gx;
}

void BBSPHGridContainer::insertParticles(BBSPHParticleSystem *pParticleSystem)
{
    // reset
    std::fill(m_GridData.begin(), m_GridData.end(), -1);

    BBSPHParticle *pParticle = pParticleSystem->getParticle(0);
    for (unsigned int n = 0; n < pParticleSystem->getSize(); n++, pParticle++)
    {
        int nGridCellIndex = getGridCellIndex(pParticle->m_Position);
        // Within each grid, particles form a linked list
        // m_GridData[nGridCellIndex] is header
        if (nGridCellIndex >= 0 && nGridCellIndex < m_GridData.size())
        {
            // Insert header
            pParticle->m_nNextIndex = m_GridData[nGridCellIndex];
            m_GridData[nGridCellIndex] = n;
        }
        else
        {
            pParticle->m_nNextIndex = -1;
        }
    }
}

int BBSPHGridContainer::findCell(const QVector3D &p)
{
    int nGridCellIndex = getGridCellIndex(p);
    if (nGridCellIndex < 0 || nGridCellIndex >= m_GridData.size())
    {
        return -1;
    }
    else
    {
        return nGridCellIndex;
    }
}

void BBSPHGridContainer::findCells(const QVector3D &p, float radius, int *pGridCell)
{
    // Because smooth kernel radius * 2 == grid size, there are 8 grid around it
    for (int i = 0; i < 8; i++)
    {
        pGridCell[i] = -1;
    }
    // Smooth kernel min pos to which the particle belongs
    int minX = (p.x() - radius - m_GridMin.x()) * m_GridDelta.x();
    int minY = (p.y() - radius - m_GridMin.y()) * m_GridDelta.y();
    int minZ = (p.z() - radius - m_GridMin.z()) * m_GridDelta.z();
    if (minX < 0)
        minX = 0;
    if (minY < 0)
        minY = 0;
    if (minZ < 0)
        minZ = 0;

    pGridCell[0] = (minZ * m_GridResolution.y() + minY) * m_GridResolution.x() + minX;
    pGridCell[1] = pGridCell[0] + 1;
    pGridCell[2] = pGridCell[0] + m_GridResolution.x();
    pGridCell[3] = pGridCell[2] + 1;
    if (minZ + 1 < m_GridResolution.z())
    {
        pGridCell[4] = pGridCell[0] + m_GridResolution.y() * m_GridResolution.x();
        pGridCell[5] = pGridCell[4] + 1;
        pGridCell[6] = pGridCell[4] + m_GridResolution.x();
        pGridCell[7] = pGridCell[6] + 1;
    }
    if (minX + 1 >= m_GridResolution.x())
    {
        pGridCell[1] = -1;
        pGridCell[3] = -1;
        pGridCell[5] = -1;
        pGridCell[7] = -1;
    }
    if (minY + 1 >= m_GridResolution.y())
    {
        pGridCell[2] = -1;
        pGridCell[3] = -1;
        pGridCell[6] = -1;
        pGridCell[7] = -1;
    }
}
