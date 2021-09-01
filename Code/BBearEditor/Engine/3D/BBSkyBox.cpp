#include "BBSkyBox.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBCamera.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/Texture/BBTexture.h"
#include "Render/BBDrawCall.h"
#include "Utils/BBUtils.h"
#include "2D/BBFullScreenQuad.h"


int BBSkyBox::m_nEnvironmentMapSize = 512;
// Since the irradiance map averages all the surrounding radiation values, it loses most of the high-frequency details,
// so we can store it at a lower resolution (32x32) and let OpenGL's linear filtering complete most of the work.
int BBSkyBox::m_nIrradianceMapSize = 32;
int BBSkyBox::m_nBaseMipmapSize = 128;
int BBSkyBox::m_nMaxMipLevels = 5;
int BBSkyBox::m_nBRDFLUTTextureSize = 512;

BBSkyBox::BBSkyBox()
    : BBRenderableObject()
{
    m_pEnvironmentMapMaterial = nullptr;
    m_pIrradianceMapMaterial = nullptr;
    m_pPrefilterMapMaterial = nullptr;
    m_pBRDFLUTTextureMaterial = nullptr;
}

BBSkyBox::~BBSkyBox()
{
    BB_SAFE_DELETE(m_pEnvironmentMapMaterial);
    BB_SAFE_DELETE(m_pIrradianceMapMaterial);
    BB_SAFE_DELETE(m_pPrefilterMapMaterial);
    BB_SAFE_DELETE(m_pBRDFLUTTextureMaterial);
}

void BBSkyBox::init(const QString &path)
{
    m_SkyBoxFilePath = path;
    initIBLSettings();

    m_pVBO = new BBVertexBufferObject(24);

    m_pVBO->setPosition(0, -0.5f, -0.5f, -0.5f);
    m_pVBO->setPosition(1, 0.5f, -0.5f, -0.5f);
    m_pVBO->setPosition(2, -0.5f, 0.5f, -0.5f);
    m_pVBO->setPosition(3, 0.5f, 0.5f, -0.5f);

    m_pVBO->setPosition(4, 0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(5, -0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(6, 0.5f, 0.5f, 0.5f);
    m_pVBO->setPosition(7, -0.5f, 0.5f, 0.5f);

    m_pVBO->setPosition(8, -0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(9, -0.5f, -0.5f, -0.5f);
    m_pVBO->setPosition(10, -0.5f, 0.5f, 0.5f);
    m_pVBO->setPosition(11, -0.5f, 0.5f, -0.5f);

    m_pVBO->setPosition(12, 0.5f, -0.5f, -0.5f);
    m_pVBO->setPosition(13, 0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(14, 0.5f, 0.5f, -0.5f);
    m_pVBO->setPosition(15, 0.5f, 0.5f, 0.5f);

    m_pVBO->setPosition(16, -0.5f, 0.5f, -0.5f);
    m_pVBO->setPosition(17, 0.5f, 0.5f, -0.5f);
    m_pVBO->setPosition(18, -0.5f, 0.5f, 0.5f);
    m_pVBO->setPosition(19, 0.5f, 0.5f, 0.5f);

    m_pVBO->setPosition(20, -0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(21, 0.5f, -0.5f, 0.5f);
    m_pVBO->setPosition(22, -0.5f, -0.5f, -0.5f);
    m_pVBO->setPosition(23, 0.5f, -0.5f, -0.5f);

    initFrom6Map();
//    initFromHDREnvironmentMap();
//    initFromEnvironmentCubeMap();

    BBRenderableObject::init();
    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLE_STRIP, 0, 24);
    appendDrawCall(pDrawCall);
}

void BBSkyBox::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    m_pCurrentMaterial->setMatrix4(LOCATION_MODELMATRIX, modelMatrix.data());
    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBSkyBox::writeEnvironmentMap(BBCamera *pCamera)
{
    // Switch to a specific material and render the 6 faces of the sky box into FBO respectively
    setCurrentMaterial(m_pEnvironmentMapMaterial);
    for (int i = 0; i < 6; i++)
    {
        m_pEnvironmentMapMaterial->setMatrix4("ViewMatrix", m_IBLCubeMapViewMatrix[i].data());
        BBTexture().startWritingTextureCube(m_EnvironmentMap, i);
        BBRenderableObject::render(pCamera);
    }
    // After setting the texture of the cube map, let OpenGL generate mipmap
    BBTexture().endWritingTextureCube(m_EnvironmentMap);
    // Restore default material
    restoreMaterial();
}

void BBSkyBox::writeIrradianceMap(BBCamera *pCamera)
{
    setCurrentMaterial(m_pIrradianceMapMaterial);
    for (int i = 0; i < 6; i++)
    {
        m_pIrradianceMapMaterial->setMatrix4("ViewMatrix", m_IBLCubeMapViewMatrix[i].data());
        BBTexture().startWritingTextureCube(m_IrradianceMap, i);
        BBRenderableObject::render(pCamera);
    }
    restoreMaterial();
}

void BBSkyBox::writePrefilterMapMipmap(BBCamera *pCamera, int nMipLevel)
{
    setCurrentMaterial(m_pPrefilterMapMaterial);

    m_pPrefilterMapMaterial->setFloat("Roughness", (float)nMipLevel / (float)(m_nMaxMipLevels - 1));
    for (int i = 0; i < 6; i++)
    {
        m_pPrefilterMapMaterial->setMatrix4("ViewMatrix", m_IBLCubeMapViewMatrix[i].data());
        BBTexture().startWritingTextureCubeMipmap(m_PrefilterMapMipmap, i, nMipLevel);
        BBRenderableObject::render(pCamera);
    }

    restoreMaterial();
}

void BBSkyBox::writeBRDFLUTTexture(BBCamera *pCamera, BBFullScreenQuad *pFullScreenQuad)
{
    pFullScreenQuad->setCurrentMaterial(m_pBRDFLUTTextureMaterial);
    BBTexture().startWritingTexture2D(m_BRDFLUTTexture);
    pFullScreenQuad->render(pCamera);
    pFullScreenQuad->restoreMaterial();
}

void BBSkyBox::changeResource(const QString &path)
{

}

void BBSkyBox::changeAlgorithm(int nIndex)
{

}

void BBSkyBox::initFrom6Map()
{
    m_pCurrentMaterial->init("SkyBox_Common", BB_PATH_RESOURCE_SHADER(SkyBox/Common.vert), BB_PATH_RESOURCE_SHADER(SkyBox/Common.frag));

    QString paths[6] = {m_SkyBoxFilePath + "right",
                        m_SkyBoxFilePath + "left",
                        m_SkyBoxFilePath + "bottom",
                        m_SkyBoxFilePath + "top",
                        m_SkyBoxFilePath + "back",
                        m_SkyBoxFilePath + "front"};
    BBTexture texture;
    m_pCurrentMaterial->setSamplerCube(LOCATION_SKYBOX_MAP, texture.createTextureCube(paths), paths);
    m_pCurrentMaterial->setZTestState(false);
}

void BBSkyBox::initFromHDREnvironmentMap()
{
    m_pCurrentMaterial->init("SkyBox_Equirectangular", BB_PATH_RESOURCE_SHADER(SkyBox/Equirectangular.vert), BB_PATH_RESOURCE_SHADER(SkyBox/Equirectangular.frag));

    GLuint hdrTexture = BBTexture().createHDRTexture2D(BB_PATH_RESOURCE_TEXTURE(HDR/newport_loft.hdr));
    m_pCurrentMaterial->setSampler2D(LOCATION_SKYBOX_EQUIRECTANGULAR_MAP, hdrTexture);
    m_pCurrentMaterial->setZTestState(false);
}

void BBSkyBox::initFromEnvironmentCubeMap()
{
    m_pCurrentMaterial->init("SkyBox_Common", BB_PATH_RESOURCE_SHADER(SkyBox/Common.vert), BB_PATH_RESOURCE_SHADER(SkyBox/Common.frag));

    m_pCurrentMaterial->setSamplerCube(LOCATION_SKYBOX_MAP, m_EnvironmentMap);
    m_pCurrentMaterial->setZTestState(false);
}

/**
 * @brief BBSkyBox::initEnvironmentMapMaterial              init the material that is used for making lighting map
 */
void BBSkyBox::initIBLSettings()
{
    // We need to sample mipmap and turn on trilinear filtering on the environment map
    m_EnvironmentMap = BBTexture().allocateTextureCube(m_nEnvironmentMapSize, m_nEnvironmentMapSize, GL_RGB16F, GL_RGB, GL_LINEAR_MIPMAP_LINEAR);
    m_IrradianceMap = BBTexture().allocateTextureCube(m_nIrradianceMapSize, m_nIrradianceMapSize, GL_RGB16F, GL_RGB);
    m_PrefilterMapMipmap = BBTexture().allocateTextureCubeMipmap(m_nBaseMipmapSize, m_nBaseMipmapSize, GL_RGB16F, GL_RGB);
    m_BRDFLUTTexture = BBTexture().allocateTexture2D(m_nBRDFLUTTextureSize, m_nBRDFLUTTextureSize, GL_RG16F, GL_RG);

    m_IBLCubeMapProjectionMatrix.perspective(90.0f, 1.0f, 0.1f, 10.0f);
    m_IBLCubeMapViewMatrix[0].lookAt(QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0));
    m_IBLCubeMapViewMatrix[1].lookAt(QVector3D(0, 0, 0), QVector3D(-1, 0, 0), QVector3D(0, 1, 0));
    m_IBLCubeMapViewMatrix[2].lookAt(QVector3D(0, 0, 0), QVector3D(0, -1, 0), QVector3D(0, 0, -1));
    m_IBLCubeMapViewMatrix[3].lookAt(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1));
    m_IBLCubeMapViewMatrix[4].lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, -1), QVector3D(0, 1, 0));
    m_IBLCubeMapViewMatrix[5].lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(0, 1, 0));

    m_pEnvironmentMapMaterial = new BBMaterial();
    m_pEnvironmentMapMaterial->init("SkyBox_Equirectangular_To_Cubemap", BB_PATH_RESOURCE_SHADER(SkyBox/Cubemap.vert), BB_PATH_RESOURCE_SHADER(SkyBox/Equirectangular2Cubemap.frag));
    m_pEnvironmentMapMaterial->setZFunc(GL_LEQUAL);
    m_pEnvironmentMapMaterial->setMatrix4("ProjectionMatrix", m_IBLCubeMapProjectionMatrix.data());
    GLuint hdrTexture = BBTexture().createHDRTexture2D(BB_PATH_RESOURCE_TEXTURE(HDR/Walk_Of_Fame/Mans_Outside_2k.hdr));
    m_pEnvironmentMapMaterial->setSampler2D(LOCATION_SKYBOX_EQUIRECTANGULAR_MAP, hdrTexture);

    m_pIrradianceMapMaterial = new BBMaterial();
    m_pIrradianceMapMaterial->init("SkyBox_Irradiance_Convolution", BB_PATH_RESOURCE_SHADER(SkyBox/Cubemap.vert), BB_PATH_RESOURCE_SHADER(SkyBox/Irradiance_Convolution.frag));
    m_pIrradianceMapMaterial->setZFunc(GL_LEQUAL);
    m_pIrradianceMapMaterial->setMatrix4("ProjectionMatrix", m_IBLCubeMapProjectionMatrix.data());
    m_pIrradianceMapMaterial->setSamplerCube("EnvironmentMap", m_EnvironmentMap);

    m_pPrefilterMapMaterial = new BBMaterial();
    m_pPrefilterMapMaterial->init("SkyBox_PrefilterMap", BB_PATH_RESOURCE_SHADER(SkyBox/Cubemap.vert), BB_PATH_RESOURCE_SHADER(SkyBox/PrefilterMap.frag));
    m_pPrefilterMapMaterial->setZFunc(GL_LEQUAL);
    m_pPrefilterMapMaterial->setMatrix4("ProjectionMatrix", m_IBLCubeMapProjectionMatrix.data());
    m_pPrefilterMapMaterial->setSamplerCube("EnvironmentMap", m_EnvironmentMap);

    m_pBRDFLUTTextureMaterial = new BBMaterial();
    m_pBRDFLUTTextureMaterial->init("BRDFLUT", BB_PATH_RESOURCE_SHADER(PBR/BRDFLUT.vert), BB_PATH_RESOURCE_SHADER(PBR/BRDFLUT.frag));
}
