#ifndef BBRENDERPASS_H
#define BBRENDERPASS_H


#include "BBBaseRenderComponent.h"
#include "BBRenderState.h"

class BBShader;
class BBCamera;
class BBUniformUpdater;
class BBMaterialProperty;

class BBRenderPass : public BBBaseRenderComponent
{
public:
    BBRenderPass();
    ~BBRenderPass();

    void setShader(BBShader *pShader);

    void bind(BBCamera *pCamera);

    void setZTestState(bool bEnable);

    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, const float *pVector4);
    void setSampler2D(const std::string &uniformName, GLuint textureName);

private:
    void appendUniformUpdater(BBUniformUpdater *pUniformUpdater);
    // 智能指针
    BBShader *m_pShader;

    BBUniformUpdater *m_pUniforms;
    QMap<std::string, BBMaterialProperty*> m_Properties;

    BBRenderState m_RenderState;
};

#endif // BBRENDERPASS_H
