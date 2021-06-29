#ifndef BBMATERIAL_H
#define BBMATERIAL_H


#include "BBBaseRenderComponent.h"
#include "BBRenderState.h"
#include <QPixmap>


class BBCamera;
class BBAttribute;
class BBUniformUpdater;
class BBMaterialProperty;
class BBRenderPass;
class BBShader;

class BBMaterial
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath);
    void initMultiPass(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath);

    void setBlendState(bool bEnable);
    void setZTestState(bool bEnable);

    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, const float *pVector4);
    void setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath = "");

    void setOverviewMap(const QPixmap &pix);
    QPixmap getOverviewMap() { return m_OverviewMap; }

    void getEditableProperties(QList<std::string> &outNames, QList<BBMaterialProperty*> &outProperties);

    inline BBRenderPass* getBaseRenderPass() const { return m_pBaseRenderPass; }
    inline BBRenderPass* getAdditiveRenderPass() const { return m_pAdditiveRenderPass; }
    BBShader* getShader();

private:
    BBRenderPass *m_pBaseRenderPass;
    BBRenderPass *m_pAdditiveRenderPass;

    QPixmap m_OverviewMap;
};

#endif // BBMATERIAL_H
