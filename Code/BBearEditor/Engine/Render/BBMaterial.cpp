#include "BBMaterial.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "BBShader.h"


BBMaterial::BBMaterial()
{
    m_pBaseRenderPass = new BBRenderPass;
    m_pAdditiveRenderPass = nullptr;
}

BBMaterial::~BBMaterial()
{
    BB_SAFE_DELETE(m_pBaseRenderPass);
    BB_SAFE_DELETE(m_pAdditiveRenderPass);
}

void BBMaterial::init(const QString &vShaderPath, const QString &fShaderPath,
                      const unsigned short *pIndexes, int nIndexCount)
{
    m_pBaseRenderPass->setShader(BBShader::loadShader("test", vShaderPath, fShaderPath));
}

void BBMaterial::bind(BBCamera *pCamera)
{
    m_pBaseRenderPass->bind(pCamera);
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

void BBMaterial::bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount)
{
//    m_pElementBufferObject->set(pIndexes, nIndexCount);
}


