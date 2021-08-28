#include "BBSkyBox.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBCamera.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/Texture/BBTexture.h"
#include "Render/BBDrawCall.h"
#include "Utils/BBUtils.h"


BBSkyBox::BBSkyBox()
    : BBRenderableObject()
{
    m_pEnvironmentMapMaterial = nullptr;
}

BBSkyBox::~BBSkyBox()
{
    BB_SAFE_DELETE(m_pEnvironmentMapMaterial);
}

void BBSkyBox::init(const QString &path)
{
    m_SkyBoxFilePath = path;

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

//    initFrom6Map();
    initFromHDREnvironmentMap();

    BBRenderableObject::init();
    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLE_STRIP, 0, 24);
    appendDrawCall(pDrawCall);

    initEnvironmentMapMaterial();
}

void BBSkyBox::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    m_pCurrentMaterial->setMatrix4(LOCATION_MODELMATRIX, modelMatrix.data());
    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBSkyBox::writeCubeMap(BBCamera *pCamera, int nSideIndex)
{
    // Switch to a specific material and render the 6 faces of the sky box into FBO respectively
    setCurrentMaterial(m_pEnvironmentMapMaterial);
    m_pEnvironmentMapMaterial->setMatrix4("ViewMatrix", m_EnvironmentMapViewMatrix[nSideIndex].data());
    BBRenderableObject::render(pCamera);
    // Restore default material
    restoreMaterial();
}

void BBSkyBox::changeResource(const QString &path)
{

}

void BBSkyBox::changeAlgorithm(int nIndex)
{

}

void BBSkyBox::initFrom6Map()
{
    m_pCurrentMaterial->init("SkyBox", BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_Common.vert), BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_Common.frag));

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
    m_pCurrentMaterial->init("SkyBox", BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_Equirectangular.vert), BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_Equirectangular.frag));

    GLuint hdrTexture = BBTexture().createHDRTexture2D(BB_PATH_RESOURCE_TEXTURE(HDR/Walk_Of_Fame/Mans_Outside_2k.hdr));
    m_pCurrentMaterial->setSampler2D(LOCATION_SKYBOX_EQUIRECTANGULAR_MAP, hdrTexture);
    m_pCurrentMaterial->setZTestState(false);
}

/**
 * @brief BBSkyBox::initEnvironmentMapMaterial              init the material that is used for making lighting map
 */
void BBSkyBox::initEnvironmentMapMaterial()
{
    m_EnvironmentMapProjectionMatrix.perspective(90.0f, 1.0f, 0.1f, 10.0f);
    m_EnvironmentMapViewMatrix[0].lookAt(QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, -1, 0));
    m_EnvironmentMapViewMatrix[1].lookAt(QVector3D(0, 0, 0), QVector3D(-1, 0, 0), QVector3D(0, -1, 0));
    m_EnvironmentMapViewMatrix[2].lookAt(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1));
    m_EnvironmentMapViewMatrix[3].lookAt(QVector3D(0, 0, 0), QVector3D(0, -1, 0), QVector3D(0, 0, -1));
    m_EnvironmentMapViewMatrix[4].lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(0, -1, 0));
    m_EnvironmentMapViewMatrix[5].lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, -1), QVector3D(0, -1, 0));

    m_pEnvironmentMapMaterial = new BBMaterial();
    m_pEnvironmentMapMaterial->init("SkyBox_MakeCubeMapFBO",
                                    BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_MakeCubeMapFBO.vert),
                                    BB_PATH_RESOURCE_SHADER(SkyBox/SkyBox_MakeCubeMapFBO.frag));

    m_pEnvironmentMapMaterial->setZFunc(GL_LEQUAL);
    m_pEnvironmentMapMaterial->setMatrix4("ProjectionMatrix", m_EnvironmentMapProjectionMatrix.data());
    GLuint hdrTexture = BBTexture().createHDRTexture2D(BB_PATH_RESOURCE_TEXTURE(HDR/Walk_Of_Fame/Mans_Outside_2k.hdr));
    m_pEnvironmentMapMaterial->setSampler2D(LOCATION_SKYBOX_EQUIRECTANGULAR_MAP, hdrTexture);
}
