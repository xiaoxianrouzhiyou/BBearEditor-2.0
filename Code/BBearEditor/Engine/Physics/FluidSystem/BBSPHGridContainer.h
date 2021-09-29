#ifndef BBSPHGRIDCONTAINER_H
#define BBSPHGRIDCONTAINER_H


#include <QVector3D>
#include <vector>

class BBSPHParticleSystem;

class BBSPHGridContainer
{
public:
    BBSPHGridContainer();

    void init(const QVector3D &min, const QVector3D &max, float fUnitScale, float fGridCellSize);
    int getGridData(int nGridIndex);
    int getGridCellIndex(const QVector3D &p);
    void insertParticles(BBSPHParticleSystem *pParticleSystem);
    int findCell(const QVector3D &p);
    void findCells(const QVector3D &p, float radius, int *pGridCell);

private:
    // Stores particles in the grid
    std::vector<int> m_GridData;
    QVector3D m_GridMin;
    QVector3D m_GridMax;
    // n * m * l
    QVector3D m_GridResolution;
    QVector3D m_GridSize;
    QVector3D m_GridDelta;
    // The size of one cell of the grid (usually twice the smooth kernel radius)
    float m_fGridCellSize;
};

#endif // BBSPHGRIDCONTAINER_H
