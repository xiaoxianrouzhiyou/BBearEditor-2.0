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
class BBDrawCall;

class BBMaterial
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath = "");
    void initMultiPass(const char *shaderName, const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath = "");
    void bindDrawCallInstance(BBDrawCall *pDrawCall) { m_pDrawCallInstance = pDrawCall; }

public:
    void setBlendState(bool bEnable);
    void setSRCBlendFunc(unsigned int src);
    void setDSTBlendFunc(unsigned int dst);
    void setBlendFunc(unsigned int src, unsigned int dst);
    void setZTestState(bool bEnable);
    void setZFunc(unsigned int func);
    void setZMask(bool bEnable);
    void setStencilMask(bool bEnable);

    bool getBlendState();
    unsigned int getSRCBlendFunc();
    unsigned int getDSTBlendFunc();

public:
    void setFloat(const std::string &uniformName, const float fValue);
    void setFloatArray(const std::string &uniformName, const float *pFloatArray, int nArrayCount);
    void setMatrix4(const std::string &uniformName, const float *pMatrix4);
    void setVector4(const std::string &uniformName, float x, float y, float z, float w);
    void setVector4(const std::string &uniformName, const float *pVector4);
    void setVector4Array(const std::string &uniformName, const float *pVector4Array, int nArrayCount);
    void setSampler2D(const std::string &uniformName, GLuint textureName, const QString &resourcePath = "");
    void setSamplerCube(const std::string &uniformName, GLuint textureName);
    void setSamplerCube(const std::string &uniformName, GLuint textureName, const QString resourcePaths[]);

    BBMaterial* clone();

    void getEditableProperties(QList<std::string> &outNames, QList<BBMaterialProperty*> &outProperties);

    void setBaseRenderPass(BBRenderPass *pRenderPass) { m_pBaseRenderPass = pRenderPass; }
    inline BBRenderPass* getBaseRenderPass() const { return m_pBaseRenderPass; }
    void setAdditiveRenderPass(BBRenderPass *pRenderPass) { m_pAdditiveRenderPass = pRenderPass; }
    inline BBRenderPass* getAdditiveRenderPass() const { return m_pAdditiveRenderPass; }
    BBShader* getShader();
    bool isWriteFBO();

private:
    BBRenderPass *m_pBaseRenderPass;
    BBRenderPass *m_pAdditiveRenderPass;
    BBDrawCall *m_pDrawCallInstance;
};

#endif // BBMATERIAL_H
