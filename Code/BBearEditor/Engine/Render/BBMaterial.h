#ifndef BBMATERIAL_H
#define BBMATERIAL_H


#include "BBBaseRenderComponent.h"
#include "BBRenderState.h"


class BBCamera;
class BBAttribute;
class BBUniformUpdater;
class BBMaterialProperty;
class BBRenderPass;

class BBMaterial
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath);

    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, const float *pVector4);

    inline BBRenderPass* getBaseRenderPass() const { return m_pBaseRenderPass; }
    inline BBRenderPass* getAdditiveRenderPass() const { return m_pAdditiveRenderPass; }

private:
    BBRenderPass *m_pBaseRenderPass;
    BBRenderPass *m_pAdditiveRenderPass;
};

#endif // BBMATERIAL_H