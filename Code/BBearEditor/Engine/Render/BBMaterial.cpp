#include "BBMaterial.h"
#include "BBVertexBufferObject.h"
#include "BBCamera.h"
#include "BBAttribute.h"
#include "BBUniformUpdater.h"


BBMaterial::BBMaterial()
{
    m_pAttributes = NULL;
    m_pUniforms = NULL;
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
            // generate attribute chain
            initAttributes();
            initUniforms();
        }
    } while(0);

    BB_SAFE_DELETE(vCode);
    BB_SAFE_DELETE(fCode);
}

void BBMaterial::bind(BBCamera *pCamera)
{
    glUseProgram(m_Program);

    if (m_pAttributes != nullptr)
    {
        m_pAttributes->active();
    }

    BBUniformUpdater *pUniformUpdater = m_pUniforms;
    while (pUniformUpdater != nullptr)
    {
        pUniformUpdater->updateUniform(pUniformUpdater->getLocation(), pCamera, pUniformUpdater->getTargetProperty());
        pUniformUpdater = pUniformUpdater->next<BBUniformUpdater>();
    }
}

void BBMaterial::setMatrix4(const std::string &uniformName, const float *pMatrix4)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBMatrix4MaterialProperty*)it.value())->setPropertyValue(pMatrix4);
    }
}

void BBMaterial::setVector4(const std::string &uniformName, const float *pVector4)
{
    auto it = m_Properties.find(uniformName);
    if (it != m_Properties.end())
    {
        ((BBVector4MaterialProperty*)it.value())->setPropertyValue(pVector4);
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

void BBMaterial::initAttributes()
{
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

void BBMaterial::initUniforms()
{
    GLint count = 0;
    glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);
    for (int i = 0; i < count; i++)
    {
        GLsizei length = 0;
        GLenum type = 0;
        GLint size = 0;
        char uniformName[256] = {0};
        glGetActiveUniform(m_Program, i, 256, &length, &size, &type, uniformName);
        GLint location = glGetUniformLocation(m_Program, uniformName);
        BBUniformUpdater *pUniformUpdater = NULL;

        if (type == GL_FLOAT_MAT4)
        {
            pUniformUpdater = initUniformMatrix4(location, uniformName);
        }
        else if (type == GL_FLOAT_VEC4)
        {
            pUniformUpdater = initUniformVector4(location, uniformName);
        }

        appendUniformUpdater(pUniformUpdater);
    }
}

BBUniformUpdater* BBMaterial::initUniformMatrix4(GLint location, const char *pUniformName)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateMatrix4;
    BBMatrix4MaterialProperty *pProperty = NULL;
    BBMaterialUniformPropertyType uniformType = BBMaterialUniformPropertyType::Matrix4;
    if (strcmp(pUniformName, NAME_PROJECTIONMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraProjectionMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraProjectionMatrix;
    }
    else if (strcmp(pUniformName, NAME_VIEWMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraViewMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraViewMatrix;
    }
    else if (strcmp(pUniformName, NAME_MODELMATRIX) == 0)
    {
        pProperty = new BBMatrix4MaterialProperty(uniformType);
        m_Properties.insert(pUniformName, pProperty);
    }
    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBMaterial::initUniformVector4(GLint location, const char *pUniformName)
{
    BBVector4MaterialProperty *pProperty = new BBVector4MaterialProperty(Vector4);
    m_Properties.insert(pUniformName, pProperty);
    return new BBUniformUpdater(location, &BBUniformUpdater::updateVector4, pProperty);
}

void BBMaterial::appendUniformUpdater(BBUniformUpdater *pUniformUpdater)
{
    if (m_pUniforms == nullptr)
    {
        m_pUniforms = pUniformUpdater;
    }
    else
    {
        m_pUniforms->pushBack(pUniformUpdater);
    }
}
