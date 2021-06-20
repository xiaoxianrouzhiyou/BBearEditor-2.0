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

void BBRenderPass::bind(BBCamera *pCamera)
{
    glUseProgram(m_pShader->getProgram());

    BBGlobalRenderState::updateBlendState(m_RenderState.m_bBlend);
    BBGlobalRenderState::updateBlendFunc(m_RenderState.m_SRCBlendFunc, m_RenderState.m_DSTBlendFunc);

    m_pShader->activeAttributes();

    BBUniformUpdater *pUniformUpdater = m_pUniforms;
    while (pUniformUpdater != nullptr)
    {
        pUniformUpdater->updateUniform(pUniformUpdater->getLocation(), pCamera, pUniformUpdater->getTargetProperty());
        pUniformUpdater = pUniformUpdater->next<BBUniformUpdater>();
    }
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
