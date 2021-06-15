#include "BBMaterial.h"
#include "BBVertexBufferObject.h"


BBMaterial::BBMaterial()
{
    m_pAttributes = NULL;
}

BBMaterial::~BBMaterial()
{

}

void BBMaterial::init(const QString &vShaderPath, const QString &fShaderPath,
                      const unsigned short *pIndexes, int nIndexCount)
{
    const char *vCode = NULL;
    const char *fCode = NULL;
    do
    {
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
            m_ProjectionMatrixLocation = glGetUniformLocation(m_Program, NAME_PROJECTIONMATRIX);
            m_ViewMatrixLocation = glGetUniformLocation(m_Program, NAME_VIEWMATRIX);
            m_ModelMatrixLocation = glGetUniformLocation(m_Program, NAME_MODELMATRIX);
            m_ITModelMatrixLocation = glGetUniformLocation(m_Program, NAME_ITMODELMATRIX);

            // generate attribute chain
            GLint count = 0;
            glGetProgramiv(m_Program, GL_ACTIVE_ATTRIBUTES, &count);
            for (int i = 0; i < count; i++)
            {
                GLsizei length = 0;
                GLenum type = 0;
                GLint size = 0;
                char attribName[256] = {0};
                glGetActiveAttrib(m_Program, i, 256, &length, &size, &type, attribName);
                GLint location = glGetAttribLocation(m_Program, attribName);
                int nComponentCount = 0;
                int nBasicDataType = 0;
                if (type == GL_FLOAT_VEC4)
                {
                    nComponentCount = 4;
                    nBasicDataType = GL_FLOAT;
                }
                int nDataOffset = 0;
                if (strcmp(attribName, NAME_POSITION) == 0)
                {
                    nDataOffset = 0;
                }
                else if (strcmp(attribName, NAME_COLOR) == 0)
                {
                    nDataOffset = sizeof(float) * 4;
                }
                else if (strcmp(attribName, NAME_TEXCOORD) == 0)
                {
                    nDataOffset = sizeof(float) * 8;
                }
                else if (strcmp(attribName, NAME_NORMAL) == 0)
                {
                    nDataOffset = sizeof(float) * 12;
                }
                BBAttribute *pAttribute = new BBAttribute(location, nComponentCount, nBasicDataType,
                                                          GL_FALSE, sizeof(BBVertex), nDataOffset);
                if (m_pAttributes == nullptr)
                {
                    m_pAttributes = pAttribute;
                }
                else
                {
                    m_pAttributes->pushBack(pAttribute);
                }
            }
        }
    } while(0);

    BB_SAFE_DELETE(vCode);
    BB_SAFE_DELETE(fCode);
}

void BBMaterial::bind(const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix)
{
    glUseProgram(m_Program);

    glUniformMatrix4fv(m_ModelMatrixLocation, 1, GL_FALSE, modelMatrix.data());

    QMatrix4x4 ITModelMatrix = modelMatrix.transposed().inverted();
    glUniformMatrix4fv(m_ITModelMatrixLocation, 1, GL_FALSE, ITModelMatrix.data());

    glUniformMatrix4fv(m_ViewMatrixLocation, 1, GL_FALSE, viewMatrix.data());

    glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, projectionMatrix.data());

    if (m_pAttributes != nullptr)
    {
        m_pAttributes->active();
    }
}

void BBMaterial::bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount)
{
//    m_pElementBufferObject->set(pIndexes, nIndexCount);
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
