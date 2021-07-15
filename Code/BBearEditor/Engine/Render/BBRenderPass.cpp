#include "BBRenderPass.h"
#include "BBMaterialProperty.h"
#include "BBCamera.h"
#include "BBShader.h"
#include "BBUniformUpdater.h"


BBRenderPass::BBRenderPass()
    : BBBaseRenderComponent()
{
    m_pShader = NULL;
}

BBRenderPass::~BBRenderPass()
{
    BB_SAFE_DELETE(m_pShader);
}

void BBRenderPass::setShader(BBShader *pShader)
{
    m_pShader = pShader;
    m_pUniforms = nullptr;

    BBUniformUpdater *pCurrentUniformUpdater = m_pShader->getUniforms();
    while (pCurrentUniformUpdater != nullptr)
    {
        BBUniformUpdater *pUniformUpdater = pCurrentUniformUpdater->clone();
        appendUniformUpdater(pUniformUpdater);
        if (pUniformUpdater->getTargetProperty() != nullptr)
        {
            m_Properties.insert(pUniformUpdater->getTargetProperty()->getName(), pUniformUpdater->getTargetProperty());
        }
        pCurrentUniformUpdater = pCurrentUniformUpdater->next<BBUniformUpdater>();
    }
}

void BBRenderPass::bind(void *ptr)
{
    glUseProgram(m_pShader->getProgram());

    BBGlobalRenderState::updateZTestState(m_RenderState.m_bZTest);
    BBGlobalRenderState::updateZFunc(m_RenderState.m_ZTestFunc);
    BBGlobalRenderState::updateBlendState(m_RenderState.m_bBlend);
    BBGlobalRenderState::updateBlendFunc(m_RenderState.m_SRCBlendFunc, m_RenderState.m_DSTBlendFunc);
    BBGlobalRenderState::updateZMask(m_RenderState.m_bWriteZ);
    BBGlobalRenderState::updateLineWidth(m_RenderState.m_fLineWidth);

    m_pShader->activeAttributes();

    BBUniformUpdater *pUniformUpdater = m_pUniforms;
    int nSlotIndex = 0;
    while (pUniformUpdater != nullptr)
    {
        if (pUniformUpdater->getUpdateUniformFunc() == &BBUniformUpdater::updateSampler2D)
        {
            glActiveTexture(GL_TEXTURE0 + nSlotIndex);
            BBSampler2DMaterialProperty *pProperty = (BBSampler2DMaterialProperty*) pUniformUpdater->getTargetProperty();
            glBindTexture(GL_TEXTURE_2D, pProperty->getTextureName());
            glUniform1i(pUniformUpdater->getLocation(), nSlotIndex);
            nSlotIndex++;
        }
        else
        {
            pUniformUpdater->updateUniform(pUniformUpdater->getLocation(), ptr, pUniformUpdater->getTargetProperty());
        }
        pUniformUpdater = pUniformUpdater->next<BBUniformUpdater>();
    }
}

void BBRenderPass::unbind()
{
    glUseProgram(0);
}

void BBRenderPass::setBlendState(bool bEnable)
{
    m_RenderState.m_bBlend = bEnable;
}

void BBRenderPass::setBlendFunc(unsigned int src, unsigned int dst)
{
    m_RenderState.m_SRCBlendFunc = src;
    m_RenderState.m_DSTBlendFunc = dst;
}

void BBRenderPass::setZTestState(bool bEnable)
{
    m_RenderState.m_bZTest = bEnable;
}

void BBRenderPass::setZFunc(unsigned int func)
{
    m_RenderState.m_ZTestFunc = func;
}

void BBRenderPass::setZMask(bool bEnable)
{
    m_RenderState.m_bWriteZ = bEnable;
}

void BBRenderPass::setLineWidth(float fWidth)
{
    m_RenderState.m_fLineWidth = fWidth;
}

void BBRenderPass::setMatrix4(const std::string &uniformName, const float *pMatrix4)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBMatrix4MaterialProperty*)it.value())->setPropertyValue(pMatrix4);
    }
}

void BBRenderPass::setVector4(const std::string &uniformName, const float *pVector4)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBVector4MaterialProperty*)it.value())->setPropertyValue(pVector4);
    }
}

void BBRenderPass::setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBSampler2DMaterialProperty*)it.value())->setTextureName(textureName, resourcePath);
    }
}

BBRenderPass* BBRenderPass::clone()
{
    BBRenderPass *pRet = new BBRenderPass();
    pRet->setShader(m_pShader);
    pRet->setRenderState(m_RenderState);
    return pRet;
}

void BBRenderPass::getEditableProperties(QList<std::string> &outNames, QList<BBMaterialProperty*> &outProperties)
{
    for (QMap<std::string, BBMaterialProperty*>::Iterator it = m_Properties.begin(); it != m_Properties.end(); it++)
    {
        std::string uniformName = it.key();
        // some properties cannot be outputted
        if (strcmp(uniformName.data(), LOCATION_MODELMATRIX) != 0
                || strcmp(uniformName.data(), LOCATION_VIEWMATRIX) != 0
                || strcmp(uniformName.data(), LOCATION_PROJECTIONMATRIX) != 0
                || strcmp(uniformName.data(), LOCATION_LIGHT_POSITION) != 0
                || strcmp(uniformName.data(), LOCATION_LIGHT_COLOR) != 0)
        {
            outNames.append(uniformName);
            BBMaterialProperty* pMaterialProperty = it.value();
            outProperties.append(pMaterialProperty);
        }
    }
}

void BBRenderPass::appendUniformUpdater(BBUniformUpdater *pUniformUpdater)
{
    if (m_pUniforms == nullptr)
    {
        m_pUniforms = pUniformUpdater;
    }
    else
    {
        m_pUniforms->pushBack(pUniformUpdater);
    }
}
