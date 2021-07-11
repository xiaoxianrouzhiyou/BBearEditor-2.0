#include "BBSprite2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBTexture.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/BBCamera.h"


BBSprite2D::BBSprite2D()
    : BBSprite2D(0, 0, 100, 100)
{

}

BBSprite2D::BBSprite2D(int nCenterX, int nCenterY, int nWidth, int nHeight)
    : BBRenderableObject2D(nCenterX, nCenterY, nWidth, nHeight)
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

    m_pCurrentMaterial->init("UI",
                             BB_PATH_RESOURCE_SHADER(UI.vert),
                             BB_PATH_RESOURCE_SHADER(UI.frag));

    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 0.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);

    BBRenderableObject2D::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 4);
    appendDrawCall(pDrawCall);

//    m_pBoundingBox2D->init();
}

void BBSprite2D::bindCanvas(BBCanvas *pCanvas)
{

}
