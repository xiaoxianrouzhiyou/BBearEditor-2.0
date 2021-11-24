#include "BBProcedureMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/BBRenderPass.h"


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
    m_pVBO = new BBVertexBufferObject(6);

    m_pVBO->setPosition(0, 1.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(1, 0.0f, 0.0f, 0.0f);

    m_pVBO->setPosition(2, 0.0f, 1.0f, 0.0f);
    m_pVBO->setPosition(3, 0.0f, 0.0f, 0.0f);

    m_pVBO->setPosition(4, 0.0f, 0.0f, 1.0f);
    m_pVBO->setPosition(5, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Red);
    }
    for (int i = 2; i < 4; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Green);
    }
    for (int i = 4; i < 6; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Blue);
    }


    BBMesh::init(userData, pOutBoundingBox);
}

void BBProcedureMesh::load(const QString &userData, QList<QVector4D> &outPositions)
{
    outPositions = m_pVBO->getPositions();
}
