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

private:
    int m_nMeshUnitPointNum;
};


// Catmull-Clark
class BBCatmullClarkMeshSubdivision : public BBMeshSubdivision
{
public:
    BBCatmullClarkMeshSubdivision(const BBMeshSubdivisionMeshType &eType = Quadrangle);
};


#endif // BBMESHSUBDIVISION_H
