#ifndef BBMATERIAL_H
#define BBMATERIAL_H


#include "BBBaseRenderComponent.h"
#include "BBRenderState.h"


class BBCamera;
class BBAttribute;
class BBUniformUpdater;
class BBMaterialProperty;
class BBRenderPass;

class BBMaterial : protected BBBaseRenderComponent
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const QString &vShaderPath, const QString &fShaderPath,
              const unsigned short *pIndexes = 0, int nIndexCount = 0);
    void bind(BBCamera *pCamera);

    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, const float *pVector4);

    void bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount);

private:
    BBRenderPass *m_pBaseRenderPass;
    BBRenderPass *m_pAdditiveRenderPass;
};

#endif // BBMATERIAL_H
