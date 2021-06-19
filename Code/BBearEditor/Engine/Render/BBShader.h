#ifndef BBSHADER_H
#define BBSHADER_H


#include "BBBaseRenderComponent.h"

class BBAttribute;
class BBUniformUpdater;
class BBMaterialProperty;

class BBShader : public BBBaseRenderComponent
{
public:
    BBShader();
    virtual ~BBShader();

    static BBShader* loadShader(const char *name, const QString &vShaderPath, const QString &fShaderPath);
    void init(const QString &vShaderPath, const QString &fShaderPath);

    inline GLuint getProgram() const { return m_Program; }
    void activeAttributes();
    inline BBUniformUpdater* getUniforms() { return m_pUniforms; }

private:
    void initAttributes();
    void initUniforms();
    BBUniformUpdater* initUniformMatrix4(GLint location, const char *pUniformName);
    BBUniformUpdater* initUniformVector4(GLint location, const char *pUniformName);
    void appendUniformUpdater(BBUniformUpdater *pUniformUpdater);

    GLuint compileShader(GLenum shaderType, const char *shaderCode);
    GLuint createProgram(GLuint vShader, GLuint fShader);

    static QMap<std::string, BBShader*> m_CachedShaders;
    GLuint m_Program;

    BBAttribute *m_pAttributes;
    BBUniformUpdater *m_pUniforms;
    QMap<std::string, BBMaterialProperty*> m_Properties;
};


#endif // BBSHADER_H
