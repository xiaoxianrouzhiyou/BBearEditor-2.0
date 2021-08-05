#include "BBMaterial.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "BBShader.h"
#include "BBDrawCall.h"


BBMaterial::BBMaterial()
{
    m_pBaseRenderPass = nullptr;
    m_pAdditiveRenderPass = nullptr;
    m_pDrawCallInstance = nullptr;
}

BBMaterial::~BBMaterial()
{
    BB_SAFE_DELETE(m_pBaseRenderPass);
    BB_SAFE_DELETE(m_pAdditiveRenderPass);
}

void BBMaterial::init(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath)
{
    m_pBaseRenderPass = new BBRenderPass();
    m_pBaseRenderPass->setShader(BBShader::loadShader(shaderName, vShaderPath, fShaderPath));
}

void BBMaterial::initMultiPass(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath)
{
    init(shaderName, vShaderPath, fShaderPath);
    m_pAdditiveRenderPass = new BBRenderPass();
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
    if (m_pDrawCallInstance)
    {
        m_pDrawCallInstance->updateMaterialBlendState(bEnable);
    }
}

void BBMaterial::setSRCBlendFunc(unsigned int src)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setSRCBlendFunc(src);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setSRCBlendFunc(src);
    }
}

void BBMaterial::setDSTBlendFunc(unsigned int dst)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setDSTBlendFunc(dst);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setDSTBlendFunc(dst);
    }
}

void BBMaterial::setBlendFunc(unsigned int src, unsigned int dst)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setBlendFunc(src, dst);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setBlendFunc(src, dst);
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
}

void BBMaterial::setZFunc(unsigned int func)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setZFunc(func);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setZFunc(func);
    }
}

void BBMaterial::setZMask(bool bEnable)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setZMask(bEnable);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setZMask(bEnable);
    }
}

void BBMaterial::setStencilMask(bool bEnable)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setStencilMask(bEnable);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setStencilMask(bEnable);
    }
}

bool BBMaterial::getBlendState()
{
    return m_pBaseRenderPass->getBlendState();
}

unsigned int BBMaterial::getSRCBlendFunc()
{
    return m_pBaseRenderPass->getSRCBlendFunc();
}

unsigned int BBMaterial::getDSTBlendFunc()
{
    return m_pBaseRenderPass->getDSTBlendFunc();
}

void BBMaterial::setFloat(const std::string &uniformName, const float fValue)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setFloat(uniformName, fValue);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setFloat(uniformName, fValue);
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
}

void BBMaterial::setVector4(const std::string &uniformName, float x, float y, float z, float w)
{
    float *value = new float[4] {x, y, z, w};
    setVector4(uniformName, value);
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
}

void BBMaterial::setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setSampler2D(uniformName, textureName, resourcePath);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setSampler2D(uniformName, textureName, resourcePath);
    }
}

void BBMaterial::setSamplerCube(const std::string &uniformName, GLuint textureName, const QString resourcePaths[])
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->setSamplerCube(uniformName, textureName, resourcePaths);
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        m_pAdditiveRenderPass->setSamplerCube(uniformName, textureName, resourcePaths);
    }
}

BBMaterial* BBMaterial::clone()
{
    BBMaterial *pRet = new BBMaterial();
    if (m_pBaseRenderPass != nullptr)
    {
        pRet->setBaseRenderPass(m_pBaseRenderPass->clone());
    }
    if (m_pAdditiveRenderPass != nullptr)
    {
        pRet->setAdditiveRenderPass(m_pAdditiveRenderPass->clone());
    }
    return pRet;
}

void BBMaterial::getEditableProperties(QList<std::string> &outNames, QList<BBMaterialProperty*> &outProperties)
{
    if (m_pBaseRenderPass != nullptr)
    {
        m_pBaseRenderPass->getEditableProperties(outNames, outProperties);
    }
}

BBShader* BBMaterial::getShader()
{
    if (m_pBaseRenderPass != nullptr)
    {
        return m_pBaseRenderPass->getShader();
    }
    return nullptr;
}

bool BBMaterial::isWriteFBO()
{
    return getShader()->isWriteFBO();
}
