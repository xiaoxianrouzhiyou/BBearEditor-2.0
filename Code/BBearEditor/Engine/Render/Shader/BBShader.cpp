#include "BBShader.h"
#include "../BBAttribute.h"
#include "../BBUniformUpdater.h"
#include "../BufferObject/BBVertexBufferObject.h"


QMap<std::string, BBShader*> BBShader::m_CachedShaders;

BBShader::BBShader()
    : BBBaseRenderComponent()
{
    m_pAttributes = NULL;
    m_pUniforms = NULL;
    m_Program = 0;
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

void BBShader::activeAttributes()
{
    if (m_pAttributes != nullptr)
    {
        m_pAttributes->active();
    }
}

void BBShader::initAttributes()
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
        if (strcmp(attribName, LOCATION_POSITION) == 0)
        {
            nDataOffset = 0;
        }
        else if (strcmp(attribName, LOCATION_COLOR) == 0)
        {
            nDataOffset = sizeof(float) * 4;
        }
        else if (strcmp(attribName, LOCATION_TEXCOORD) == 0)
        {
            nDataOffset = sizeof(float) * 8;
        }
        else if (strcmp(attribName, LOCATION_NORMAL) == 0)
        {
            nDataOffset = sizeof(float) * 12;
        }
        else if (strcmp(attribName, LOCATION_TANGENT) == 0)
        {
            nDataOffset = sizeof(float) * 16;
        }
        else if (strcmp(attribName, LOCATION_BITANGENT) == 0)
        {
            nDataOffset = sizeof(float) * 20;
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

void BBShader::initUniforms()
{
    GLint count = 0;
    glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);
    int nSlotIndex = 3; // Reserved for FBO
    for (int i = 0; i < count; i++)
    {
        GLsizei length = 0;
        GLenum type = 0;
        GLint size = 0;
        char uniformName[256] = {0};
        glGetActiveUniform(m_Program, i, 256, &length, &size, &type, uniformName);
        GLint location = glGetUniformLocation(m_Program, uniformName);
        BBUniformUpdater *pUniformUpdater = NULL;

        if (type == GL_FLOAT)
        {
            pUniformUpdater = initUniformFloat(location, uniformName);
        }
        else if (type == GL_FLOAT_MAT4)
        {
            pUniformUpdater = initUniformMatrix4(location, uniformName);
        }
        else if (type == GL_FLOAT_VEC4)
        {
            if (size == 1)
            {
                pUniformUpdater = initUniformVector4(location, uniformName);
            }
            else
            {
                pUniformUpdater = initUniformArrayVector4(location, uniformName, size);
            }
        }
        else if (type == GL_SAMPLER_2D)
        {
            pUniformUpdater = initUniformSampler2D(location, uniformName, nSlotIndex);
        }
        else if (type == GL_SAMPLER_CUBE)
        {
            pUniformUpdater = initUniformSamplerCube(location, uniformName, nSlotIndex);
        }

        appendUniformUpdater(pUniformUpdater);
    }
}

BBUniformUpdater* BBShader::initUniformFloat(GLint location, const char *pUniformName)
{
    BBFloatMaterialProperty *pProperty = new BBFloatMaterialProperty(pUniformName);
    m_Properties.insert(pUniformName, pProperty);
    return new BBUniformUpdater(location, &BBUniformUpdater::updateFloat, pProperty);
}

BBUniformUpdater* BBShader::initUniformMatrix4(GLint location, const char *pUniformName)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateMatrix4;
    BBMatrix4MaterialProperty *pProperty = nullptr;
    BBMaterialUniformPropertyType uniformType = BBMaterialUniformPropertyType::Matrix4;
    if (strcmp(pUniformName, LOCATION_PROJECTIONMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraProjectionMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraProjectionMatrix;
    }
    else if (strcmp(pUniformName, LOCATION_PROJECTIONMATRIX_I) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraInverseProjectionMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraInverseProjectionMatrix;
    }
    else if (strcmp(pUniformName, LOCATION_VIEWMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraViewMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraViewMatrix;
    }
    else if (strcmp(pUniformName, LOCATION_VIEWMATRIX_I) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraInverseViewMatrix;
        uniformType = BBMaterialUniformPropertyType::CameraInverseViewMatrix;
    }
    else if (strcmp(pUniformName, LOCATION_LIGHT_PROJECTIONMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateLightProjectionMatrix;
        uniformType = BBMaterialUniformPropertyType::LightProjectionMatrix;
    }
    else if (strcmp(pUniformName, LOCATION_LIGHT_VIEWMATRIX) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateLightViewMatrix;
        uniformType = BBMaterialUniformPropertyType::LightViewMatrix;
    }
    else
    {
        pProperty = new BBMatrix4MaterialProperty(pUniformName);
        m_Properties.insert(pUniformName, pProperty);
    }
    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBShader::initUniformVector4(GLint location, const char *pUniformName)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateVector4;
    BBVector4MaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_CANVAS) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCanvas;
    }
    else if (strcmp(pUniformName, LOCATION_CAMERA_PARAMETERS) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraParameters;
    }
    else if (strcmp(pUniformName, LOCATION_CAMERA_POSITION) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraPosition;
    }
    else if (strcmp(pUniformName, LOCATION_TIME) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateTime;
    }
    else
    {
        pProperty = new BBVector4MaterialProperty(pUniformName);
        m_Properties.insert(pUniformName, pProperty);
    }
    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBShader::initUniformArrayVector4(GLint location, const char *pUniformName, int nArrayCount)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateArrayVector4;
    BBArrayVector4MaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_SPHERICAL_HARMONIC_LIGHTING) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateSphericalHarmonicLightingCoefficients;
    }
    else
    {
        pProperty = new BBArrayVector4MaterialProperty(pUniformName, nArrayCount);
        m_Properties.insert(pUniformName, pProperty);
    }
    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBShader::initUniformSampler2D(GLint location, const char *pUniformName, int &nSlotIndex)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateSampler2D;
    BBSampler2DMaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_CAMERA_COLOR_TEXTURE) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateColorFBO;
        m_bWriteFBO = false;
    }
    else if (strcmp(pUniformName, LOCATION_CAMERA_DEPTH_TEXTURE) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateDepthFBO;
        m_bWriteFBO = false;
    }
    else if (strcmp(pUniformName, LOCATION_SHADOWMAP) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateShadowMap;
    }
    else
    {
        pProperty = new BBSampler2DMaterialProperty(pUniformName, nSlotIndex);
        m_Properties.insert(pUniformName, pProperty);
        nSlotIndex++;
    }

    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBShader::initUniformSamplerCube(GLint location, const char *pUniformName, int &nSlotIndex)
{
    BBSamplerCubeMaterialProperty *pProperty = new BBSamplerCubeMaterialProperty(pUniformName, nSlotIndex);
    m_Properties.insert(pUniformName, pProperty);
    nSlotIndex++;

    return new BBUniformUpdater(location, &BBUniformUpdater::updateSamplerCube, pProperty);
}

void BBShader::appendUniformUpdater(BBUniformUpdater *pUniformUpdater)
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

GLuint BBShader::compileShader(GLenum shaderType, const char *shaderCode)
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

GLuint BBShader::createProgram(GLuint vShader, GLuint fShader)
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
        qDebug() << "create GPU program fail, log:" << szLog;
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}
