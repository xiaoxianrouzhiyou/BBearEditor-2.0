#ifndef BBMATERIAL_H
#define BBMATERIAL_H


#include "BBBaseRenderComponent.h"
#include "BBAttribute.h"
#include "BBUniformUpdater.h"

class BBCamera;

class BBMaterial : protected BBBaseRenderComponent
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const QString &vShaderPath, const QString &fShaderPath,
              const unsigned short *pIndexes = 0, int nIndexCount = 0);
    void bind(BBCamera *pCamera);
    void bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount);

    GLuint compileShader(GLenum shaderType, const char *shaderCode);
    GLuint createProgram(GLuint vShader, GLuint fShader);

    inline BBUniformUpdater* getUniforms() { return m_pUniforms; }

private:
    void initAttributes();
    void initUniforms();

    GLuint m_Program;

    BBAttribute *m_pAttributes;
    BBUniformUpdater *m_pUniforms;

    GLuint m_PositionLocation;
    GLuint m_ColorLocation;
    GLuint m_TexcoordLocation;
    GLuint m_NormalLocation;

    GLuint m_ProjectionMatrixLocation;
    GLuint m_ViewMatrixLocation;
    GLuint m_ModelMatrixLocation;
    GLuint m_ITModelMatrixLocation;
};

#endif // BBMATERIAL_H
