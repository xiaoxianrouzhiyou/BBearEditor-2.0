#include "BBShader.h"


QMap<std::string, BBShader*> BBShader::m_CachedShaders;

BBShader::BBShader()
    : BBBaseShader()
{
    m_bWriteFBO = true;
}

BBShader::~BBShader()
{

}

BBShader* BBShader::loadShader(const char *name, const QString &vShaderPath, const QString &fShaderPath)
{
    auto it = m_CachedShaders.find(name);
    if (it != m_CachedShaders.end())
    {
        return it.value();
    }
    BBShader *pShader = new BBShader();
    pShader->init(vShaderPath, fShaderPath);
    pShader->setShaderName(name);
    pShader->setVShaderPath(vShaderPath);
    pShader->setFShaderPath(fShaderPath);
    m_CachedShaders.insert(name, pShader);
    return pShader;
}

void BBShader::init(const QString &vShaderPath, const QString &fShaderPath)
{
    const char *vCode = nullptr;
    const char *fCode = nullptr;
    do
    {
        int nFileSize;
        // path.toLatin1().data(); will cause Chinese garbled
        vCode = BBUtils::loadFileContent(vShaderPath.toStdString().c_str(), nFileSize);
        BB_PROCESS_ERROR(vCode);
        fCode = BBUtils::loadFileContent(fShaderPath.toStdString().c_str(), nFileSize);
        BB_PROCESS_ERROR(fCode);

        GLuint vShader = compileShader(GL_VERTEX_SHADER, vCode);
        BB_PROCESS_ERROR(vShader);
        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fCode);
        BB_PROCESS_ERROR(fShader);
        m_Program = createProgram(vShader, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (m_Program != 0)
        {
            // generate attribute chain
            initAttributes();
            initUniforms();
        }
    } while(0);

    BB_SAFE_DELETE(vCode);
    BB_SAFE_DELETE(fCode);
}

GLuint BBShader::createProgram(GLuint vShader, GLuint fShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);


    // what the gpu needs to extract
    const char *pAttribs[] = {"gl_Position"};
    // Parameter 2 is the number of data to be extracted
    // Parameter 4 indicates that we extract all data into a VBO or ...
    glTransformFeedbackVaryings(program, 1, pAttribs, GL_INTERLEAVED_ATTRIBS);


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
        qDebug() << "create GPU program fail, log:" << szLog;
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}
