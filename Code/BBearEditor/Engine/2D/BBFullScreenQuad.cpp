#include "BBFullScreenQuad.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/BBRenderPass.h"
#include "Render/BBTexture.h"
#include "Scene/BBScene.h"
#include "Scene/BBSceneManager.h"
#include "Render/Light/BBLight.h"
#include "Render/Light/BBPointLight.h"
#include "Render/BBCamera.h"


/**
 * @brief BBFullScreenQuad::BBFullScreenQuad
 */
BBFullScreenQuad::BBFullScreenQuad()
    : BBFullScreenQuad(1.0f, 0.0f, 0.0f)
{

}

BBFullScreenQuad::BBFullScreenQuad(float fScale, float fOffsetX, float fOffsetY)
    : BBRenderableObject()
{
    m_fScale = fScale;
    m_fOffsetX = fOffsetX;
    m_fOffsetY = fOffsetY;
}

void BBFullScreenQuad::init()
{
    m_pVBO = new BBVertexBufferObject(4);
    m_pVBO->setPosition(0, -1.0f * m_fScale + m_fOffsetX, -1.0f * m_fScale + m_fOffsetY, 0.0f);
    m_pVBO->setPosition(1, 1.0f * m_fScale + m_fOffsetX, -1.0f * m_fScale + m_fOffsetY, 0.0f);
    m_pVBO->setPosition(2, -1.0f * m_fScale + m_fOffsetX, 1.0f * m_fScale + m_fOffsetY, 0.0f);
    m_pVBO->setPosition(3, 1.0f * m_fScale + m_fOffsetX, 1.0f * m_fScale + m_fOffsetY, 0.0f);
    m_pVBO->setTexcoord(0, (m_pVBO->getPosition(0).x() + 1.0f) / 2.0f, (m_pVBO->getPosition(0).y() + 1.0f) / 2.0f);
    m_pVBO->setTexcoord(1, (m_pVBO->getPosition(1).x() + 1.0f) / 2.0f, (m_pVBO->getPosition(1).y() + 1.0f) / 2.0f);
    m_pVBO->setTexcoord(2, (m_pVBO->getPosition(2).x() + 1.0f) / 2.0f, (m_pVBO->getPosition(2).y() + 1.0f) / 2.0f);
    m_pVBO->setTexcoord(3, (m_pVBO->getPosition(3).x() + 1.0f) / 2.0f, (m_pVBO->getPosition(3).y() + 1.0f) / 2.0f);

    m_pCurrentMaterial->init("fullscreenquad",
                             BB_PATH_RESOURCE_SHADER(fullscreenquad.vert),
                             BB_PATH_RESOURCE_SHADER(fullscreenquad.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendFunc(GL_ONE, GL_ONE);
    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);
    closeLight();

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLE_STRIP, 0, 4);
    appendDrawCall(pDrawCall);
}

void BBFullScreenQuad::render(BBCamera *pCamera)
{
    QList<BBGameObject*> lights = BBSceneManager::getScene()->getLights();
    QList<BBGameObject*> culledLights;
    for (int i = 0; i < lights.count(); i++)
    {
        if (lights[i]->getClassName() == BB_CLASSNAME_POINT_LIGHT)
        {
//            if (!((BBPointLight*)lights[i])->cull(pCamera, m_AABB))
//            {
//                culledLights.append(lights[i]);
//            }
            // without culling
            culledLights.append(lights[i]);
        }
        else
        {
            culledLights.append(lights[i]);
        }
    }

    m_pDrawCalls->onePassRendering(pCamera, culledLights);
}

//void BBFullScreenQuad::setAABB(float fWidth, float fHeight)
//{
//    m_AABB = QRectF(m_pVBO->getPosition(2).x() * fWidth / 2.0f,
//                    m_pVBO->getPosition(2).y() * fHeight / 2.0f,
//                    (m_pVBO->getPosition(1).x() - m_pVBO->getPosition(0).x()) * fWidth / 2.0f,
//                    (m_pVBO->getPosition(2).y() - m_pVBO->getPosition(0).y()) * fHeight / 2.0f);
//}


/**
 * @brief BBTiledFullScreenQuad::BBTiledFullScreenQuad
 */
BBTiledFullScreenQuad::BBTiledFullScreenQuad()
    : BBGameObject()
{
    m_nQuadCount = 4;
    m_pFullScreenQuad[0] = new BBFullScreenQuad(0.5f, -0.5f, -0.5f);
    m_pFullScreenQuad[1] = new BBFullScreenQuad(0.5f, 0.5f, -0.5f);
    m_pFullScreenQuad[2] = new BBFullScreenQuad(0.5f, -0.5f, 0.5f);
    m_pFullScreenQuad[3] = new BBFullScreenQuad(0.5f, 0.5f, 0.5f);
}

BBTiledFullScreenQuad::~BBTiledFullScreenQuad()
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        BB_SAFE_DELETE(m_pFullScreenQuad[i]);
    }
}

void BBTiledFullScreenQuad::init()
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        m_pFullScreenQuad[i]->init();
    }
}

void BBTiledFullScreenQuad::render(BBCamera *pCamera)
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        m_pFullScreenQuad[i]->render(pCamera);
    }
    // test bottom left
    QList<BBGameObject*> lights = BBSceneManager::getScene()->getLights();
    for (int i = 0; i < lights.count(); i++)
    {
        if (lights[i]->getClassName() == BB_CLASSNAME_POINT_LIGHT)
        {
            qDebug() << pCamera->isFrustumContainPoint(0, 0, 0, lights[i]->getPosition());
        }
    }
}

//void BBTiledFullScreenQuad::setTiledAABB(float fWidth, float fHeight)
//{
//    for (int i = 0; i < m_nQuadCount; i++)
//    {
//        m_pFullScreenQuad[i]->setAABB(fWidth, fHeight);
//    }
//}

void BBTiledFullScreenQuad::setTexture(const std::string &uniformName, GLuint textureName)
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        m_pFullScreenQuad[i]->setTexture(uniformName, textureName);
    }
}

void BBTiledFullScreenQuad::openLight()
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        m_pFullScreenQuad[i]->openLight();
    }
}

void BBTiledFullScreenQuad::closeLight()
{
    for (int i = 0; i < m_nQuadCount; i++)
    {
        m_pFullScreenQuad[i]->closeLight();
    }
}
