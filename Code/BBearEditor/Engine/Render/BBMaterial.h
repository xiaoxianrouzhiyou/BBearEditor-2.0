#ifndef BBMATERIAL_H
#define BBMATERIAL_H


#include "BBearGL.h"
#include <QOpenGLShaderProgram>

class BBMaterial : protected QOpenGLFunctions
{
public:
    BBMaterial();
    ~BBMaterial();

    void init(const QString &vShaderPath, const QString &fShaderPath);
    void bind(const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix);

    GLuint compileShader(GLenum shaderType, const char *shaderCode);
    GLuint createProgram(GLuint vShader, GLuint fShader);

private:
    GLuint m_Program;

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
