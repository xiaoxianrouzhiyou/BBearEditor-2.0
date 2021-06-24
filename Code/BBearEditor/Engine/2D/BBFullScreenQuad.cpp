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

    m_pMaterial->init("fullscreenquad",
                      BB_PATH_RESOURCE_SHADER(fullscreenquad.vert),
                      BB_PATH_RESOURCE_SHADER(texture.frag));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLE_STRIP, 0, 4);
    appendDrawCall(pDrawCall);
}

void BBFullScreenQuad::setTexture(GLuint textureName)
{
    m_pMaterial->getBaseRenderPass()->setSampler2D(NAME_TEXTURE, textureName);
}
