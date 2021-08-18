#include "BBClipArea2D.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBDrawCall.h"
#include "Scene/BBRendererManager.h"


BBClipArea2D::BBClipArea2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject2D(x, y, nWidth, nHeight)
{

}

void BBClipArea2D::init()
{
    m_pVBO = new BBVertexBufferObject(4);
    m_pVBO->setPosition(0, 1.0f, 1.0f, 0);
    m_pVBO->setPosition(1, -1.0f, 1.0f, 0);
    m_pVBO->setPosition(2, -1.0f, -1.0f, 0);
    m_pVBO->setPosition(3, 1.0f, -1.0f, 0);
    m_pVBO->setTexcoord(0, 1.0f, 1.0f);
    m_pVBO->setTexcoord(1, 0.0f, 1.0f);
    m_pVBO->setTexcoord(2, 0.0f, 0.0f);
    m_pVBO->setTexcoord(3, 1.0f, 0.0f);
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setColor(i, 1.0f, 1.0f, 1.0f);
    }

    m_pCurrentMaterial = BBRendererManager::createStencilUIMaterial();
    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 4);
    appendDrawCall(pDrawCall);
}
