#include "BBSprite2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBTexture.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/BBCamera.h"


BBSprite2D::BBSprite2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject2D(x, y, nWidth, nHeight)
{

}

BBSprite2D::~BBSprite2D()
{

}

void BBSprite2D::init()
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

    BBRenderableObject2D::init();
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 1.0f, 0.0f, 0.0f, 0.0f);
    BBTexture texture;
    QString texturePath = BB_PATH_RESOURCE_ICON(empty2.png);
    m_pCurrentMaterial->setSampler2D(LOCATION_TEXTURE(0), texture.createTexture2D(texturePath), texturePath);

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 4);
    appendDrawCall(pDrawCall);
}

