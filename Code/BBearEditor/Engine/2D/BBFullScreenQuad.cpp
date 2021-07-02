#include "BBFullScreenQuad.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/BBRenderPass.h"
#include "Render/BBTexture.h"


BBFullScreenQuad::BBFullScreenQuad()
    : BBRenderableObject()
{

}

void BBFullScreenQuad::init()
{
    m_pVBO = new BBVertexBufferObject(4);
    m_pVBO->setPosition(0, -1.0f, -1.0f, 0.0f);
    m_pVBO->setPosition(1, 1.0f, -1.0f, 0.0f);
    m_pVBO->setPosition(2, -1.0f, 1.0f, 0.0f);
    m_pVBO->setPosition(3, 1.0f, 1.0f, 0.0f);
    m_pVBO->setTexcoord(0, 0.0f, 0.0f);
    m_pVBO->setTexcoord(1, 1.0f, 0.0f);
    m_pVBO->setTexcoord(2, 0.0f, 1.0f);
    m_pVBO->setTexcoord(3, 1.0f, 1.0f);

    m_pCurrentMaterial->init("fullscreenquad",
                             BB_PATH_RESOURCE_SHADER(fullscreenquad.vert),
                             BB_PATH_RESOURCE_SHADER(fullscreenquad.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setZMask(false);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendFunc(GL_ONE, GL_ONE);
    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    m_pCurrentMaterial->setVector4(NAME_LIGHT_POSITION, pLightPosition);
    m_pCurrentMaterial->setVector4(NAME_LIGHT_COLOR, pLightColor);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLE_STRIP, 0, 4);
    appendDrawCall(pDrawCall);
}

void BBFullScreenQuad::render(BBCamera *pCamera)
{
    m_pDrawCalls->onePassRendering(pCamera);
}

void BBFullScreenQuad::setTexture(const std::string &uniformName, GLuint textureName)
{
    m_pCurrentMaterial->getBaseRenderPass()->setSampler2D(uniformName, textureName);
}
