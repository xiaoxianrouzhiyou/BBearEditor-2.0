#include "BBSPHGridContainer.h"
#include "BBSPHParticleSystem.h"


BBSPHGridContainer::BBSPHGridContainer()
{

}

void BBSPHGridContainer::init(const QVector3D &min, const QVector3D &max, float fUnitScale, float fGridCellSize)
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
    QVector3D grid = (p - m_GridMin) * m_GridDelta;
    return (grid.z() * m_GridResolution.y() + grid.y()) * m_GridResolution.x() + grid.x();
}

void BBSPHGridContainer::insertParticles(BBSPHParticleSystem *pParticleSystem)
{
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
    QVector3D min = p - QVector3D(radius, radius, radius);
    min -= m_GridMin;
    min *= m_GridDelta;
    if (min.x() < 0)
        min.setX(0);
    if (min.y() < 0)
        min.setY(0);
    if (min.z() < 0)
        min.setZ(0);

    pGridCell[0] = (min.z() * m_GridResolution.y() + min.y()) * m_GridResolution.x() + min.x();
    pGridCell[1] = pGridCell[0] + 1;
    pGridCell[2] = pGridCell[0] + m_GridResolution.x();
    pGridCell[3] = pGridCell[2] + 1;
    if (min.z() + 1 < m_GridResolution.z())
    {
        pGridCell[4] = pGridCell[0] + m_GridResolution.y() * m_GridResolution.x();
        pGridCell[5] = pGridCell[4] + 1;
        pGridCell[6] = pGridCell[4] + m_GridResolution.x();
        pGridCell[7] = pGridCell[6] + 1;
    }
    if (min.x() + 1 >= m_GridResolution.x())
    {
        pGridCell[1] = -1;
        pGridCell[3] = -1;
        pGridCell[5] = -1;
        pGridCell[7] = -1;
    }
    if (min.y() + 1 >= m_GridResolution.y())
    {
        pGridCell[2] = -1;
        pGridCell[3] = -1;
        pGridCell[6] = -1;
        pGridCell[7] = -1;
    }
}
