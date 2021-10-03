#ifndef BBMARCHINGCUBEMESH_H
#define BBMARCHINGCUBEMESH_H


#include "Base/BBRenderableObject.h"


struct BBMCScalarField
{
    // size of x y z
    unsigned int m_nNum[3];
    // width of each grid
    QVector3D m_UnitWidth;
    // min pos
    QVector3D m_Min;
};

struct BBMCVertex
{
    unsigned int m_nID;
    QVector3D m_Position;
};

struct BBMCTriangle
{
    unsigned int m_VertexID[3];
};

// A list of isosurface vertices (with edge index as key and isosurface points as value)
typedef std::map<unsigned int, BBMCVertex> BBMCEVMap;

typedef std::vector<BBMCTriangle> BBMCTriangleVector;

class BBMarchingCubeMesh : public BBRenderableObject
{
public:
    BBMarchingCubeMesh();
    ~BBMarchingCubeMesh();

    bool init(float *pField, unsigned int *pNum, const QVector3D &unitWidth, const QVector3D &min, float fThreshold);

private:
    void generateIsoSurface();
    void generateEdgeVertexMap(unsigned int x, unsigned int y, unsigned int z, unsigned int nEdge);
    BBMCVertex computeIntersection(unsigned int x, unsigned int y, unsigned int z, unsigned int nEdge);
    unsigned int getEdgeID(unsigned int x, unsigned int y, unsigned int z, unsigned int nEdge);
    unsigned int getVertexID(unsigned int x, unsigned int y, unsigned int z);
    void generateVBOAndEBO();

    void deleteSurface();

private:
    static unsigned int m_EdgeTable[256];
    static int m_TriangleTable[256][16];

    // Scalar field info
    BBMCScalarField m_Grid;
    const float *m_pScalarField;
    float m_fIsoLevel;
    BBMCEVMap m_EVMap;
    BBMCTriangleVector m_TriangleVector;
    bool m_bValidSurface;
};

#endif // BBMARCHINGCUBEMESH_H
