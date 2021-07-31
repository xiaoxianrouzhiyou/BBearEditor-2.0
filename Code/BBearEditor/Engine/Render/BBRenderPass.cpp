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
    BBGlobalRenderState::updatePolygonMode(m_RenderState.m_DrawFace, m_RenderState.m_PolygonMode);
    BBGlobalRenderState::updateLineWidth(m_RenderState.m_fLineWidth);
    BBGlobalRenderState::updateCullState(m_RenderState.m_bCull);
    BBGlobalRenderState::updateCullFace(m_RenderState.m_CullFace);

    m_pShader->activeAttributes();

    BBUniformUpdater *pUniformUpdater = m_pUniforms;
    while (pUniformUpdater != nullptr)
    {
        pUniformUpdater->updateUniform(pUniformUpdater->getLocation(), ptr, pUniformUpdater->getTargetProperty());
        pUniformUpdater = pUniformUpdater->next<BBUniformUpdater>();
    }
}

void BBRenderPass::unbind()
{
    glUseProgram(0);
}

void BBRenderPass::setupStencilBuffer()
{
    if (m_RenderState.m_bWriteStencil)
    {
        BBGlobalRenderState::clearStencil(0);
        glClear(GL_STENCIL_BUFFER_BIT);
        // pixels are written in the stencil
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glColorMask(false, false, false, false);
    }
    else if (m_RenderState.m_bUseStencil)
    {
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void BBRenderPass::restoreStencilBuffer()
{
    if (m_RenderState.m_bWriteStencil)
    {
        glDisable(GL_STENCIL_TEST);
        glColorMask(true, true, true, true);
    }
    else if (m_RenderState.m_bUseStencil)
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void BBRenderPass::setBlendState(bool bEnable)
{
    m_RenderState.m_bBlend = bEnable;
}

void BBRenderPass::setSRCBlendFunc(unsigned int src)
{
    m_RenderState.m_SRCBlendFunc = src;
}

void BBRenderPass::setDSTBlendFunc(unsigned int dst)
{
    m_RenderState.m_DSTBlendFunc = dst;
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

void BBRenderPass::setStencilMask(bool bEnable)
{
    m_RenderState.m_bWriteStencil = bEnable;
}

void BBRenderPass::setUseStencil(bool bEnable)
{
    m_RenderState.m_bUseStencil = bEnable;
}

void BBRenderPass::setPolygonMode(unsigned int face, unsigned int mode)
{
    m_RenderState.m_DrawFace = face;
    m_RenderState.m_PolygonMode = mode;
}

void BBRenderPass::setLineWidth(float fWidth)
{
    m_RenderState.m_fLineWidth = fWidth;
}

void BBRenderPass::setCullState(bool bEnable)
{
    m_RenderState.m_bCull = bEnable;
}

void BBRenderPass::setCullFace(int face)
{
    m_RenderState.m_CullFace = face;
}

void BBRenderPass::setFloat(const std::string &uniformName, const float fValue)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBFloatMaterialProperty*)it.value())->setPropertyValue(fValue);
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

void BBRenderPass::setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBSampler2DMaterialProperty*)it.value())->setTextureName(textureName, resourcePath);
    }
}

void BBRenderPass::setSamplerCube(const std::string &uniformName, GLuint textureName, const QString resourcePaths[])
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBSamplerCubeMaterialProperty*)it.value())->setTextureName(textureName, resourcePaths);
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
                && strcmp(uniformName.data(), LOCATION_VIEWMATRIX) != 0
                && strcmp(uniformName.data(), LOCATION_PROJECTIONMATRIX) != 0
                && strcmp(uniformName.data(), LOCATION_LIGHT_POSITION) != 0
                && strcmp(uniformName.data(), LOCATION_LIGHT_COLOR) != 0)
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
