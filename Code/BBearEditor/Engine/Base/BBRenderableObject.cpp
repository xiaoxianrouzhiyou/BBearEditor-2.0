#include "BBRenderableObject.h"
#include "Render/BBDrawCall.h"
#include "Render/BBMaterial.h"
#include "Utils/BBUtils.h"
#include "Render/BBCamera.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBElementBufferObject.h"
#include "Render/BBGLBuffers.h"
#include "Render/BBUniformUpdater.h"
#include "Render/BBRenderPass.h"


BBRenderableObject::BBRenderableObject()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBRenderableObject::BBRenderableObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBRenderableObject(position.x(), position.y(), position.z(),
                         rotation.x(), rotation.y(), rotation.z(),
                            scale.x(),    scale.y(),    scale.z())
{

}

BBRenderableObject::BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_pDrawCalls = NULL;
    m_bVisible = true;
    m_Materials.append(new BBMaterial());
    m_pCurrentMaterial = m_Materials[0];
    m_pVBO = NULL;
    m_pEBO = NULL;
    m_pIndexes = NULL;
    m_nIndexCount = 0;
    m_nVertexCount = 0;
    m_DefaultColor = QVector3D(1.0f, 1.0f, 1.0f);
}

BBRenderableObject::~BBRenderableObject()
{
    qDeleteAll(m_Materials);
    BB_SAFE_DELETE(m_pVBO);
    BB_SAFE_DELETE(m_pEBO);
    BB_SAFE_DELETE_ARRAY(m_pIndexes);
}

void BBRenderableObject::init()
{
    m_pVBO->submitData();
    if (m_nIndexCount > 0)
    {
        m_pEBO = new BBElementBufferObject(m_nIndexCount);
        m_pEBO->submitData(m_pIndexes, m_nIndexCount);
    }
}

void BBRenderableObject::render(BBCamera *pCamera)
{
    render(m_ModelMatrix, pCamera);
}

void BBRenderableObject::render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera)
{
    if (m_bVisible)
    {
        m_pCurrentMaterial->setMatrix4(NAME_MODELMATRIX, modelMatrix.data());
        m_pDrawCalls->draw(pCamera);
    }
}

void BBRenderableObject::setCurrentMaterial(int nIndex)
{
    m_pCurrentMaterial = m_Materials[nIndex];
}

void BBRenderableObject::appendDrawCall(BBDrawCall *pDrawCall)
{
    if (m_pDrawCalls == nullptr)
    {
        m_pDrawCalls = pDrawCall;
    }
    else
    {
        m_pDrawCalls->pushBack(pDrawCall);
    }
}
