#ifndef BBRENDERPASS_H
#define BBRENDERPASS_H


#include "BBBaseRenderComponent.h"
#include "BBRenderState.h"

class BBShader;
class BBCamera;
class BBCanvas;
class BBUniformUpdater;
class BBMaterialProperty;

class BBRenderPass : public BBBaseRenderComponent
{
public:
    BBRenderPass();
    ~BBRenderPass();

    void setShader(BBShader *pShader);
    BBShader* getShader() { return m_pShader; }

    void setRenderState(const BBRenderState &renderState) { m_RenderState = renderState; }

    void bind(void *ptr);
    void unbind();

    void setupStencilBuffer();
    void restoreStencilBuffer();

public:
    void setBlendState(bool bEnable);
    void setSRCBlendFunc(unsigned int src);
    void setDSTBlendFunc(unsigned int dst);
    void setBlendFunc(unsigned int src, unsigned int dst);
    void setZTestState(bool bEnable);
    void setZFunc(unsigned int func);
    void setZMask(bool bEnable);
    void setStencilMask(bool bEnable);
    void setUseStencil(bool bEnable);
    void setPolygonMode(unsigned int face, unsigned int mode);
    void setLineWidth(float fWidth);
    void setCullState(bool bEnable);
    void setCullFace(int face);
    void setPointSpriteState(bool bEnable);
    void setProgramPointSizeState(bool bEnable);

    bool getBlendState() { return m_RenderState.m_bBlend; }
    unsigned int getSRCBlendFunc() { return m_RenderState.m_SRCBlendFunc; }
    unsigned int getDSTBlendFunc() { return m_RenderState.m_DSTBlendFunc; }

public:
    void setFloat(const std::string &uniformName, const float fValue);
    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, const float *pVector4);
    void setArrayVector4(const std::string &uniformName, const float *pArrayVector4, int nArrayCount);
    void setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath = "");
    void setSamplerCube(const std::string &uniformName, GLuint textureName);
    void setSamplerCube(const std::string &uniformName, GLuint textureName, const QString resourcePaths[]);

    BBRenderPass* clone();

    void getEditableProperties(QList<std::string> &outNames, QList<BBMaterialProperty*> &outProperties);

private:
    void appendUniformUpdater(BBUniformUpdater *pUniformUpdater);
    // 智能指针
    BBShader *m_pShader;

    BBUniformUpdater *m_pUniforms;
    QMap<std::string, BBMaterialProperty*> m_Properties;

    BBRenderState m_RenderState;
};

#endif // BBRENDERPASS_H
