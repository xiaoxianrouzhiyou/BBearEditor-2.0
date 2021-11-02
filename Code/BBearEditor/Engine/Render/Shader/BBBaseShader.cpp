#include "BBBaseShader.h"
#include "Render/BBAttribute.h"
#include "Render/BBUniformUpdater.h"
#include "Render/BufferObject/BBVertexBufferObject.h"


BBBaseShader::BBBaseShader()
{
    m_pAttributes = nullptr;
    m_eVertexBufferType = VBO;
    m_pUniforms = nullptr;
    m_Program = 0;
}

void BBBaseShader::activeAttributes()
{
    if (m_pAttributes != nullptr)
    {
        m_pAttributes->active();
    }
}

void BBBaseShader::initAttributes()
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
        else if (strcmp(attribName, LOCATION_SMOOTHNORMAL) == 0)
        {
            nDataOffset = sizeof(float) * 24;
        }
        else
        {
            // If the attribname of VBO is not satisfied, ssbo is used
            m_eVertexBufferType = SSBO;
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

void BBBaseShader::initUniforms()
{
    GLint count = 0;
    glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);
    // Reserved for FBO
    // 0 : View Space Color FBO
    // 1 : View Space Depth FBO
    // 2 : Shadow Map
    // 3 : Irradiance Map
    // 4 : Prefilter Map Mipmap
    // 5 : BRDF LUT Texture
    // 6 : SkyBox Cube
    // 7 : SkyBox Background
    int nSlotIndex = 8;
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
            if (size == 1)
            {
                pUniformUpdater = initUniformFloat(location, uniformName);
            }
            else
            {
                pUniformUpdater = initUniformFloatArray(location, uniformName, size);
            }
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
                pUniformUpdater = initUniformVector4Array(location, uniformName, size);
            }
        }
        else if (type == GL_SAMPLER_2D)
        {
            pUniformUpdater = initUniformSampler2D(location, uniformName, nSlotIndex);
        }
        else if (type == GL_SAMPLER_3D)
        {
            pUniformUpdater = initUniformSampler3D(location, uniformName, nSlotIndex);
        }
        else if (type == GL_SAMPLER_CUBE)
        {
            pUniformUpdater = initUniformSamplerCube(location, uniformName, nSlotIndex);
        }

        appendUniformUpdater(pUniformUpdater);
    }
}

BBUniformUpdater* BBBaseShader::initUniformFloat(GLint location, const char *pUniformName)
{
    BBFloatMaterialProperty *pProperty = new BBFloatMaterialProperty(pUniformName);
    m_Properties.insert(pUniformName, pProperty);
    return new BBUniformUpdater(location, &BBUniformUpdater::updateFloat, pProperty);
}

BBUniformUpdater* BBBaseShader::initUniformFloatArray(GLint location, const char *pUniformName, int nArrayCount)
{
    BBFloatArrayMaterialProperty *pProperty = new BBFloatArrayMaterialProperty(pUniformName, nArrayCount);
    m_Properties.insert(pUniformName, pProperty);
    return new BBUniformUpdater(location, &BBUniformUpdater::updateFloatArray, pProperty);
}

BBUniformUpdater* BBBaseShader::initUniformMatrix4(GLint location, const char *pUniformName)
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

BBUniformUpdater* BBBaseShader::initUniformVector4(GLint location, const char *pUniformName)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateVector4;
    BBVector4MaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_CANVAS) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCanvas;
    }
    else if (strcmp(pUniformName, LOCATION_CAMERA_PARAMETERS0) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraParameters0;
    }
    else if (strcmp(pUniformName, LOCATION_CAMERA_PARAMETERS1) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateCameraParameters1;
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

BBUniformUpdater* BBBaseShader::initUniformVector4Array(GLint location, const char *pUniformName, int nArrayCount)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateVector4Array;
    BBVector4ArrayMaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_SPHERICAL_HARMONIC_LIGHTING) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateSphericalHarmonicLightingCoefficients;
    }
    else
    {
        pProperty = new BBVector4ArrayMaterialProperty(pUniformName, nArrayCount);
        m_Properties.insert(pUniformName, pProperty);
    }
    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBBaseShader::initUniformSampler2D(GLint location, const char *pUniformName, int &nSlotIndex)
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
    else if (strcmp(pUniformName, LOCATION_BRDF_LUT_TEXTURE) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateBRDFLUTTexture;
    }
    else if (strcmp(pUniformName, LOCATION_SKYBOX_BACKGROUND) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateSkyBoxBackground;
    }
    else
    {
        pProperty = new BBSampler2DMaterialProperty(pUniformName, nSlotIndex);
        m_Properties.insert(pUniformName, pProperty);
        nSlotIndex++;
    }

    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBBaseShader::initUniformSampler3D(GLint location, const char *pUniformName, int &nSlotIndex)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateSampler3D;
    BBSampler3DMaterialProperty *pProperty = new BBSampler3DMaterialProperty(pUniformName, nSlotIndex);
    m_Properties.insert(pUniformName, pProperty);
    nSlotIndex++;

    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

BBUniformUpdater* BBBaseShader::initUniformSamplerCube(GLint location, const char *pUniformName, int &nSlotIndex)
{
    BBUpdateUniformFunc updateUniformFunc = &BBUniformUpdater::updateSamplerCube;
    BBSamplerCubeMaterialProperty *pProperty = nullptr;
    if (strcmp(pUniformName, LOCATION_SKYBOX_MAP) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateSkyBoxCube;
    }
    else if (strcmp(pUniformName, LOCATION_IRRADIANCE_MAP) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updateIrradianceMap;
    }
    else if (strcmp(pUniformName, LOCATION_PREFILTER_MAP_MIPMAP) == 0)
    {
        updateUniformFunc = &BBUniformUpdater::updatePrefilterMapMipmap;
    }
    else
    {
        pProperty = new BBSamplerCubeMaterialProperty(pUniformName, nSlotIndex);
        m_Properties.insert(pUniformName, pProperty);
        nSlotIndex++;
    }

    return new BBUniformUpdater(location, updateUniformFunc, pProperty);
}

void BBBaseShader::appendUniformUpdater(BBUniformUpdater *pUniformUpdater)
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

GLuint BBBaseShader::compileShader(GLenum shaderType, const char *shaderCode)
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
