#include "BBComputeShader.h"

BBComputeShader::BBComputeShader()
    : BBShader()
{

}

void BBComputeShader::init(const QString &shaderPath)
{
    const char *code = nullptr;
    do
    {
        int nFileSize;
        code = BBUtils::loadFileContent(shaderPath.toStdString().c_str(), nFileSize);
        BB_PROCESS_ERROR(code);

        GLuint shader = compileShader(GL_COMPUTE_SHADER, code);
        BB_PROCESS_ERROR(shader);

        m_Program = glCreateProgram();
        glAttachShader(m_Program, shader);
        glLinkProgram(m_Program);
        glDetachShader(m_Program, shader);
        GLint nResult;
        glGetProgramiv(m_Program, GL_LINK_STATUS, &nResult);
        if (nResult == GL_FALSE)
        {
            char szLog[1024] = {0};
            GLsizei logLength = 0;
            glGetProgramInfoLog(m_Program, 1024, &logLength, szLog);
            qDebug() << "create compute program fail, log:" << szLog;
            glDeleteProgram(m_Program);
            m_Program = 0;
        }

        glDeleteShader(shader);

        if (m_Program != 0)
        {
            // generate attribute chain
            initAttributes();
            initUniforms();
        }
    } while(0);

    BB_SAFE_DELETE(code);
}

void BBComputeShader::bind(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ)
{
    glUseProgram(m_Program);
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    // sync cpu with gpu
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void BBComputeShader::unbind()
{
    glUseProgram(0);
}
