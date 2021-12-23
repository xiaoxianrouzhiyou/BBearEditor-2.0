#ifndef BBMESHSUBDIVISION_H
#define BBMESHSUBDIVISION_H


#include <QVector3D>

enum BBMeshSubdivisionMeshType
{
    Triangle,
    Quadrangle,
    TriangleAndQuadrangle
};

class BBMeshSubdivision
{
public:
    BBMeshSubdivision(const BBMeshSubdivisionMeshType &eType);

    /* Catmull-Clark */

private:
    int m_nMeshUnitPointNum;
};

#endif // BBMESHSUBDIVISION_H
