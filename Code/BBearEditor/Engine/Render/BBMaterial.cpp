#include "BBMaterial.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "BBShader.h"


BBMaterial::BBMaterial()
{
    m_pBaseRenderPass = nullptr;
    m_pAdditiveRenderPass = nullptr;
    m_pDeferredRenderPass = nullptr;
}

BBMaterial::~BBMaterial()
{
    BB_SAFE_DELETE(m_pBaseRenderPass);
    BB_SAFE_DELETE(m_pAdditiveRenderPass);
    BB_SAFE_DELETE(m_pDeferredRenderPass);
}

void BBMaterial::init(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath)
{
    m_pBaseRenderPass = new BBRenderPass;
    m_pBaseRenderPass->setShader(BBShader::loadShader(shaderName, vShaderPath, fShaderPath));
    m_pDeferredRenderPass = new BBRenderPass;
    m_pDeferredRenderPass->setShader(BBShader::loadShader("DefferedPosition",
                                                          BB_PATH_RESOURCE_SHADER(DefferedPosition.vert),
                                                          BB_PATH_RESOURCE_SHADER(DefferedPosition.frag)));
}

void BBMaterial::initMultiPass(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath)
{
    init(shaderName, vShaderPath, fShaderPath);
    m_pAdditiveRenderPass = new BBRenderPass;
    m_pAdditiveRenderPass->setShader(BBShader::loadShader(shaderName, vShaderPath, fShaderPath));
}

void BBMaterial::setBlendState(bool bEnable)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setBlendState(bEnable);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setBlendState(bEnable);
    }
    if (m_pDeferredRenderPass != nullptr)
    {
        m_pDeferredRenderPass->setBlendState(bEnable);
    }
}

void BBMaterial::setZTestState(bool bEnable)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setZTestState(bEnable);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setZTestState(bEnable);
    }
    if (m_pDeferredRenderPass != nullptr)
    {
        m_pDeferredRenderPass->setZTestState(bEnable);
    }
}

void BBMaterial::setMatrix4(const std::string &uniformName, const float *pMatrix4)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setMatrix4(uniformName, pMatrix4);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setMatrix4(uniformName, pMatrix4);
    }
    if (m_pDeferredRenderPass != nullptr)
    {
        m_pDeferredRenderPass->setMatrix4(uniformName, pMatrix4);
    }
}

void BBMaterial::setVector4(const std::string &uniformName, const float *pVector4)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setVector4(uniformName, pVector4);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setVector4(uniformName, pVector4);
    }
    if (m_pDeferredRenderPass != nullptr)
    {
        m_pDeferredRenderPass->setVector4(uniformName, pVector4);
    }
}

void BBMaterial::setSampler2D(const std::string &uniformName, GLuint textureName)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setSampler2D(uniformName, textureName);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setSampler2D(uniformName, textureName);
    }
    if (m_pDeferredRenderPass != nullptr)
    {
        m_pDeferredRenderPass->setSampler2D(uniformName, textureName);
    }
}


