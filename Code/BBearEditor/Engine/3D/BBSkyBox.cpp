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

}

BBSkyBox::~BBSkyBox()
{

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
}

void BBSkyBox::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    m_pCurrentMaterial->setMatrix4(LOCATION_MODELMATRIX, modelMatrix.data());
    BBRenderableObject::render(modelMatrix, pCamera);
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

    GLuint hdrTexture = BBTexture().createHDRTexture2D(BB_PATH_RESOURCE_TEXTURE(HDR/Brooklyn_Bridge_Planks/Brooklyn_Bridge_Planks_2k.hdr));
    m_pCurrentMaterial->setSampler2D(LOCATION_SKYBOX_EQUIRECTANGULAR_MAP, hdrTexture);
    m_pCurrentMaterial->setZTestState(false);
}
