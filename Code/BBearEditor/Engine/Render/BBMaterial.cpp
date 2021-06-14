#include "BBMaterial.h"


BBMaterial::BBMaterial()
{

}

BBMaterial::~BBMaterial()
{

}

void BBMaterial::init(const QString &vShaderPath, const QString &fShaderPath)
{
    const char *vCode = NULL;
    const char *fCode = NULL;
    do
    {
        initializeOpenGLFunctions();

        int nFileSize;
        // path.toLatin1().data(); will cause Chinese garbled
        vCode = BBUtils::loadFileContent(vShaderPath.toStdString().c_str(), nFileSize);
        fCode = BBUtils::loadFileContent(fShaderPath.toStdString().c_str(), nFileSize);

        GLuint vShader = compileShader(GL_VERTEX_SHADER, vCode);
        BB_PROCESS_ERROR(vShader);
        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fCode);
        BB_PROCESS_ERROR(fShader);
        m_Program = createProgram(vShader, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (m_Program != 0)
        {
            m_PositionLocation = glGetAttribLocation(m_Program, NAME_POSITION);
            m_ColorLocation = glGetAttribLocation(m_Program, NAME_COLOR);
            m_TexcoordLocation = glGetAttribLocation(m_Program, NAME_TEXCOORD);
            m_NormalLocation = glGetAttribLocation(m_Program, NAME_NORMAL);
            m_ProjectionMatrixLocation = glGetUniformLocation(m_Program, NAME_PROJECTIONMATRIX);
            m_ViewMatrixLocation = glGetUniformLocation(m_Program, NAME_VIEWMATRIX);
            m_ModelMatrixLocation = glGetUniformLocation(m_Program, NAME_MODELMATRIX);
            m_ITModelMatrixLocation = glGetUniformLocation(m_Program, NAME_ITMODELMATRIX);
        }
    } while(0);

    BB_SAFE_DELETE(vCode);
    BB_SAFE_DELETE(fCode);
}

void BBMaterial::bind(const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix)
{
    glUseProgram(m_Program);
    if (m_ModelMatrixLocation > 0)
    {
        glUniformMatrix4fv(m_ModelMatrixLocation, 1, GL_FALSE, modelMatrix.data());
    }
    if (m_ITModelMatrixLocation > 0)
    {
        QMatrix4x4 ITModelMatrix = modelMatrix.transposed().inverted();
        glUniformMatrix4fv(m_ITModelMatrixLocation, 1, GL_FALSE, ITModelMatrix.data());
    }
    if (m_ViewMatrixLocation > 0)
    {
        glUniformMatrix4fv(m_ViewMatrixLocation, 1, GL_FALSE, viewMatrix.data());
    }
    if (m_ProjectionMatrixLocation > 0)
    {
        glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, projectionMatrix.data());
    }
    if (m_PositionLocation > 0)
    {
        glEnableVertexAttribArray(m_PositionLocation);
        glVertexAttribPointer(m_PositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    }
    if (m_ColorLocation > 0)
    {
        glEnableVertexAttribArray(m_ColorLocation);
        glVertexAttribPointer(m_ColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 4));
    }
    if (m_TexcoordLocation > 0)
    {
        glEnableVertexAttribArray(m_TexcoordLocation);
        glVertexAttribPointer(m_TexcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 8));
    }
    if (m_NormalLocation > 0)
    {
        glEnableVertexAttribArray(m_NormalLocation);
        glVertexAttribPointer(m_NormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 10));
    }
}

GLuint BBMaterial::compileShader(GLenum shaderType, const char *shaderCode)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        char szLog[1024] = {0};
        GLsizei logLength = 0;
        glGetShaderInfoLog(shader, 1024, &logLength, szLog);
        qDebug() << "compileShader fail, log:" << szLog;
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

GLuint BBMaterial::createProgram(GLuint vShader, GLuint fShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glDetachShader(program, vShader);
    glDetachShader(program, fShader);
    GLint nResult;
    glGetProgramiv(program, GL_LINK_STATUS, &nResult);
    if (nResult == GL_FALSE)
    {
        char szLog[1024] = {0};
        GLsizei logLength = 0;
        glGetProgramInfoLog(program, 1024, &logLength, szLog);
        qDebug() << "createProgram fail, log:" << szLog;
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}
