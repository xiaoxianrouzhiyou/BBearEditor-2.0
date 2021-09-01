#ifndef BBSHADER_H
#define BBSHADER_H


#include "BBBaseShader.h"

class BBAttribute;
class BBUniformUpdater;

class BBShader : public BBBaseShader
{
public:
    BBShader();
    virtual ~BBShader();

    static BBShader* loadShader(const char *name, const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath = "");
    void init(const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath = "");

    inline void setShaderName(const char *name) { strcpy(m_ShaderName, name); }
    inline void setVShaderPath(const QString &path) { m_VShaderPath = path; }
    inline void setFShaderPath(const QString &path) { m_FShaderPath = path; }

    inline char* getShaderName() { return m_ShaderName; }
    inline QString getVShaderPath() { return m_VShaderPath; }
    inline QString getFShaderPath() { return m_FShaderPath; }

private:
    GLuint createProgram(GLuint vShader, GLuint fShader, GLuint gShader);

    static QMap<std::string, BBShader*> m_CachedShaders;
    char m_ShaderName[64];
    QString m_VShaderPath;
    QString m_FShaderPath;
};


#endif // BBSHADER_H
