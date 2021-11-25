#include "BBProcedureMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/BBRenderPass.h"
#include "Geometry/BBBoundingBox.h"


BBProcedureMesh::BBProcedureMesh()
    : BBProcedureMesh(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBProcedureMesh::BBProcedureMesh(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBMesh(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBProcedureMesh::init(const QString &userData, BBBoundingBox3D *&pOutBoundingBox)
{
    init0();


    QList<QVector4D> positions;
    load(userData, positions);
    m_pVBO->computeTangent(m_pIndexes, m_nIndexCount);
    m_pVBO->computeSmoothNormal();

    // create bounding box
    pOutBoundingBox = new BBAABBBoundingBox3D(m_Position.x(), m_Position.y(), m_Position.z(),
                                              m_Rotation.x(), m_Rotation.y(), m_Rotation.z(),
                                              m_Scale.x(), m_Scale.y(), m_Scale.z(),
                                              positions);
    pOutBoundingBox->init();

    m_pCurrentMaterial->init("base", BB_PATH_RESOURCE_SHADER(base.vert), BB_PATH_RESOURCE_SHADER(base.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setPolygonMode(GL_LINE);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_QUADS, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBProcedureMesh::load(const QString &userData, QList<QVector4D> &outPositions)
{
    outPositions = m_pVBO->getPositions();
}

/**
 * @brief BBProcedureMesh::init0                        A plane consists of quadrilateral primitive
 */
void BBProcedureMesh::init0()
{
    m_pVBO = new BBVertexBufferObject(121);

    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            int nIndex = (i + 5) * 11 + (j + 5);
            m_pVBO->setPosition(nIndex, 1.0f * j, 0.0f, 1.0f * i);
            m_pVBO->setColor(nIndex, BBConstant::m_White);
            m_pVBO->setNormal(nIndex, 0.0f, 1.0f, 0.0f);
        }
    }

    m_nIndexCount = 400;
    m_pIndexes = new unsigned short[m_nIndexCount];
    unsigned short *pCurrent = m_pIndexes;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int nIndex = i * 11 + j;
            *(pCurrent++) = nIndex;
            *(pCurrent++) = nIndex + 11;
            *(pCurrent++) = nIndex + 12;
            *(pCurrent++) = nIndex + 1;
        }
    }
}
