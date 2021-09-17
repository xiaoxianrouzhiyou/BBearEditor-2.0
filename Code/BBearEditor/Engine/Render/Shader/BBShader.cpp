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

BBShader* BBShader::loadShader(const char *name, const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath)
{
    auto it = m_CachedShaders.find(name);
    if (it != m_CachedShaders.end())
    {
        return it.value();
    }
    BBShader *pShader = new BBShader();
    pShader->init(vShaderPath, fShaderPath, gShaderPath);
    pShader->setShaderName(name);
    pShader->setVShaderPath(vShaderPath);
    pShader->setFShaderPath(fShaderPath);
    m_CachedShaders.insert(name, pShader);
    return pShader;
}

void BBShader::init(const QString &vShaderPath, const QString &fShaderPath, const QString &gShaderPath)
{
    const char *vCode = nullptr;
    const char *fCode = nullptr;
    const char *gCode = nullptr;
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

        GLuint gShader = 0;
        if (!gShaderPath.isEmpty())
        {
            gCode = BBUtils::loadFileContent(gShaderPath.toStdString().c_str(), nFileSize);
            BB_PROCESS_ERROR(gCode);
            gShader = compileShader(GL_GEOMETRY_SHADER, gCode);
            BB_PROCESS_ERROR(gShader);
        }

        m_Program = createProgram(vShader, fShader, gShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        if (!gShaderPath.isEmpty())
        {
            glDeleteShader(gShader);
        }

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

GLuint BBShader::createProgram(GLuint vShader, GLuint fShader, GLuint gShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    if (gShader > 0)
    {
        glAttachShader(program, gShader);
    }


    // what the gpu needs to extract
    const char *pAttribs[] = {"gl_Position"};
    // Parameter 2 is the number of data to be extracted
    // Parameter 4 indicates that we extract all data into a VBO or ...
    glTransformFeedbackVaryings(program, 1, pAttribs, GL_INTERLEAVED_ATTRIBS);


    glLinkProgram(program);
    glDetachShader(program, vShader);
    glDetachShader(program, fShader);
    if (gShader > 0)
    {
        glDetachShader(program, gShader);
    }

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
