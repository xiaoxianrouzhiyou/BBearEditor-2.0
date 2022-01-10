#ifndef BBMESHSUBDIVISION_H
#define BBMESHSUBDIVISION_H


#include <QVector3D>
#include <QList>

class BBProcedureMesh;

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

protected:
    int m_nMeshUnitPointNum;
};


// Catmull-Clark
class BBCatmullClarkMeshSubdivision : public BBMeshSubdivision
{
public:
    BBCatmullClarkMeshSubdivision(BBProcedureMesh *pMesh, const BBMeshSubdivisionMeshType &eType = Quadrangle);

private:
    QList<QVector3D> getFacePoints();
    void generateEdgeFaceList();

private:
    QList<QVector4D> m_InputPositions;
    unsigned short *m_pInputVertexIndexes;
    int m_nInputIndexCount;
};


#endif // BBMESHSUBDIVISION_H
