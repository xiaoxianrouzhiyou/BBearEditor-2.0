#include "BBMeshSubdivision.h"
#include "Utils/BBUtils.h"
#include "3D/Mesh/BBProcedureMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"


/**
 * @brief BBMeshSubdivision::BBMeshSubdivision
 * @param eType
 */
BBMeshSubdivision::BBMeshSubdivision(const BBMeshSubdivisionMeshType &eType)
{
    if (eType == Triangle)
        m_nMeshUnitPointNum = 3;
    else if (eType == Quadrangle)
        m_nMeshUnitPointNum = 4;
}


/**
 * @brief BBCatmullClarkMeshSubdivision::BBCatmullClarkMeshSubdivision
 * @param eType
 */
BBCatmullClarkMeshSubdivision::BBCatmullClarkMeshSubdivision(BBProcedureMesh *pMesh, const BBMeshSubdivisionMeshType &eType)
    : BBMeshSubdivision(eType)
{
    m_InputPositions = pMesh->getVBO()->getPositions();
    m_pInputVertexIndexes = pMesh->getVertexIndexes();
    m_nInputIndexCount = pMesh->getIndexCount();

    QList<QVector3D> facePoints = getFacePoints();
}

/**
 * @brief BBCatmullClarkMeshSubdivision::getFacePoints                              a face point is the average of all the points of the face
 */
QList<QVector3D> BBCatmullClarkMeshSubdivision::getFacePoints()
{
    QList<QVector3D> facePoints;

    // For the moment, consider that primitive consists of 4 vertices
    for (int i = 0; i < m_nInputIndexCount; i += m_nMeshUnitPointNum)
    {
        // each face
        unsigned short index0 = m_pInputVertexIndexes[i + 0];
        unsigned short index1 = m_pInputVertexIndexes[i + 1];
        unsigned short index2 = m_pInputVertexIndexes[i + 2];
        unsigned short index3 = m_pInputVertexIndexes[i + 3];

        QVector3D point0(m_InputPositions[index0]);
        QVector3D point1(m_InputPositions[index1]);
        QVector3D point2(m_InputPositions[index2]);
        QVector3D point3(m_InputPositions[index3]);

        QVector3D facePoint = (point0 + point1 + point2 + point3) / m_nMeshUnitPointNum;
        facePoints.append(facePoint);
    }

    return facePoints;
}

void BBCatmullClarkMeshSubdivision::generateEdgeFaceList()
{

}
