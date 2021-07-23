#ifndef BBSHADER_H
#define BBSHADER_H


#include "BBBaseRenderComponent.h"
#include "BBMaterialProperty.h"

class BBAttribute;
class BBUniformUpdater;

class BBShader : public BBBaseRenderComponent
{
public:
    BBShader();
    virtual ~BBShader();

    static BBShader* loadShader(const char *name, const QString &vShaderPath, const QString &fShaderPath);
    void init(const QString &vShaderPath, const QString &fShaderPath);
    inline void setShaderName(const char *name) { strcpy(m_ShaderName, name); }
    inline void setVShaderPath(const QString &path) { m_VShaderPath = path; }
    inline void setFShaderPath(const QString &path) { m_FShaderPath = path; }

    inline char* getShaderName() { return m_ShaderName; }
    inline QString getVShaderPath() { return m_VShaderPath; }
    inline QString getFShaderPath() { return m_FShaderPath; }

    inline GLuint getProgram() const { return m_Program; }
    void activeAttributes();
    inline BBUniformUpdater* getUniforms() { return m_pUniforms; }

private:
    void initAttributes();
    void initUniforms();
    BBUniformUpdater* initUniformFloat(GLint location, const char *pUniformName);
    BBUniformUpdater* initUniformMatrix4(GLint location, const char *pUniformName);
    BBUniformUpdater* initUniformVector4(GLint location, const char *pUniformName);
    BBUniformUpdater* initUniformSampler2D(GLint location, const char *pUniformName);
    void appendUniformUpdater(BBUniformUpdater *pUniformUpdater);

    GLuint compileShader(GLenum shaderType, const char *shaderCode);
    GLuint createProgram(GLuint vShader, GLuint fShader);

    static QMap<std::string, BBShader*> m_CachedShaders;
    GLuint m_Program;
    char m_ShaderName[64];
    QString m_VShaderPath;
    QString m_FShaderPath;
    BBAttribute *m_pAttributes;
    BBUniformUpdater *m_pUniforms;
    QMap<std::string, BBMaterialProperty*> m_Properties;
};


#endif // BBSHADER_H
