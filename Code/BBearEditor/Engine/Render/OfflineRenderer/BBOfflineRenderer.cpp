#include "BBOfflineRenderer.h"
#include "Utils/BBUtils.h"
#include "Scene/BBScene.h"
#include "3D/BBModel.h"
#include "BBCamera.h"
#include "Geometry/BBPhotonMap.h"
#include "Render/Lighting/GameObject/BBAreaLight.h"
#include "Render/BBOfflineOpenGLWidget.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Scene/BBSceneManager.h"
#include "OfflineRenderer/BBScatterMaterial.h"
#include "Math/BBMath.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBTexture.h"
#include "2D/BBFullScreenQuad.h"


BBOfflineRenderer::BBOfflineRenderer(BBScene *pScene)
{
    m_pScene = pScene;
    m_pMaterial = nullptr;
    m_nWidth = pScene->getCamera()->getViewportWidth();
    m_nHeight = pScene->getCamera()->getViewportHeight();
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i] = nullptr;
    }
    m_pAreaLight = nullptr;
    m_pPhotonMap = nullptr;

    m_nBlendFrameCount = 0;
}

BBOfflineRenderer::~BBOfflineRenderer()
{
    for (int i = 0; i < TestModelCount; i++)
    {
        BB_SAFE_DELETE(m_pModels[i]);
    }
    BB_SAFE_DELETE(m_pAreaLight);
    BB_SAFE_DELETE(m_pPhotonMap);
}

void BBOfflineRenderer::createTestScene()
{
    BB_PROCESS_ERROR_RETURN(!m_pModels[0]);
    // cornell box
    m_pModels[0] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(-2, 0, 0), QVector3D(0, 0, -90), QVector3D(1, 1, 1));
    m_pModels[1] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(2, 0, 0), QVector3D(0, 0, 90), QVector3D(1, 1, 1));
    m_pModels[2] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(1, 1, 1));
    m_pModels[3] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 2, 0), QVector3D(0, 0, -180), QVector3D(1, 1, 1));
    m_pModels[4] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, -2), QVector3D(90, 0, 0), QVector3D(1, 1, 1));
    m_pModels[5] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, 4), QVector3D(-90, 0, 0), QVector3D(1, 1, 1));
    m_pModels[6] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(cube.obj), QVector3D(0.4f, 0, 0), QVector3D(0, 0, 0), QVector3D(0.5f, 1.0f, 0.5f));
    m_pModels[7] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(cube.obj), QVector3D(-0.4f, 0, 0), QVector3D(0, 0, 0), QVector3D(0.25f, 1.0f, 0.5f));
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i]->setBoundingBoxVisibility(false);
        m_pModels[i]->setScatterMaterial(new BBLambertian(QVector3D(1, 1, 1)));
    }
    m_pModels[6]->setScatterMaterial(new BBLambertian(QVector3D(0, 1, 0)));
    m_pModels[7]->setScatterMaterial(new BBMetal(QVector3D(1, 0, 0)));
    m_pAreaLight = new BBAreaLight(-0.3f, 0.3f, 1.0f, 1.2f, 2.0f);
    m_pAreaLight->init();

    m_pScene->getCamera()->update(QVector3D(0, 1, 3.5f), QVector3D(0, 1, 2.5f));

    m_pMaterial = new BBMaterial();
    m_pMaterial->init("fullscreenquad_texture", BB_PATH_RESOURCE_SHADER(fullscreenquad_texture.vert), BB_PATH_RESOURCE_SHADER(fullscreenquad_texture.frag));
    BBFullScreenQuad *pFullScreenQuad = m_pScene->getFinalFullScreenQuad();
    pFullScreenQuad->setCurrentMaterial(m_pMaterial);
    m_pScene->setRenderingFunc(&BBScene::deferredRendering0_1);
}

void BBOfflineRenderer::startPhotonMapping()
{
    generatePhotonMap();
    m_pPhotonMap->balance();
//    for (int i = 0; i < 3; i++)
    {
        renderFrame();
        showFrame();
    }
    showPhotonMap();
}

void BBOfflineRenderer::renderFrame()
{
    // Emit rays to each pixel of the screen
    BBCamera *pCamera = m_pScene->getCamera();
    int w = pCamera->getViewportWidth();
    int h = pCamera->getViewportHeight();
    QImage image(w, h, QImage::Format_ARGB32);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            QVector3D color(0.0f, 0.0f, 0.0f);
            int nSampleCount = 4;
            for (int sample = 0; sample < nSampleCount; sample++)
            {
                BBRay ray = pCamera->createRayFromScreen(x + sfrandom(), y + sfrandom());
                float d;
                if (m_pAreaLight->hit(ray, d))
                {
                    color = m_pAreaLight->getColor();
                }
                else
                {
                    color += BBPhotonMap::traceRay(ray, m_pModels, TestModelCount, 0, m_pPhotonMap);
                }
            }
            color /= nSampleCount;

            color.setX(clamp(color.x(), 0.0f, 1.0f));
            color.setY(clamp(color.y(), 0.0f, 1.0f));
            color.setZ(clamp(color.z(), 0.0f, 1.0f));
//            qDebug() << color;

            // blend m_CurrentImage and image
            // Do not blend at the first time
            if (m_nBlendFrameCount > 0)
            {
                QColor currentColor = m_CurrentImage.pixelColor(x, h - y - 1);
                QVector3D currentColorVector(currentColor.redF(), currentColor.greenF(), currentColor.blueF());
                color = currentColorVector * m_nBlendFrameCount / (m_nBlendFrameCount + 1) + color / (m_nBlendFrameCount + 1);
                color.setX(clamp(color.x(), 0.0f, 1.0f));
                color.setY(clamp(color.y(), 0.0f, 1.0f));
                color.setZ(clamp(color.z(), 0.0f, 1.0f));
            }

            image.setPixelColor(x, h - y - 1, QColor::fromRgbF(color.x(), color.y(), color.z()));
        }
    }
    m_CurrentImage = image;
    m_nBlendFrameCount++;
}

void BBOfflineRenderer::showFrame()
{
    qDebug() << m_nBlendFrameCount;
    m_pMaterial->setSampler2D(LOCATION_TEXTURE(0), BBTexture().createTexture2D(m_CurrentImage));
    m_pScene->update();
}

void BBOfflineRenderer::generatePhotonMap()
{
    int nMaxPhotonNum = 10000;
    m_pPhotonMap = new BBPhotonMap(nMaxPhotonNum);
    QVector3D origin;
    QVector3D direction;
    QVector3D power(1.0f, 1.0f, 1.0f);
    float fPowerScale;

    for (int i = 0; i < nMaxPhotonNum; i++)
    {
        m_pAreaLight->generatePhoton(origin, direction, fPowerScale);
        BBRay ray(origin, direction);
        BBPhotonMap::tracePhoton(ray, m_pModels, TestModelCount, 0, power * fPowerScale, m_pPhotonMap, false);
    }
}

void BBOfflineRenderer::showPhotonMap()
{
//    qDebug() << m_pPhotonMap->getBoxMin() << m_pPhotonMap->getBoxMax();
    int nPhotonNum = m_pPhotonMap->getPhotonNum();
    QVector3D *pPositions = m_pPhotonMap->getPhotonPositions();

    // test
    BBNearestPhotons nearestPhotons(QVector3D(0, 0, -1), 100, 0.6f);
    m_pPhotonMap->getKNearestPhotons(&nearestPhotons, 1);
    m_pPhotonMap->markKNearestPhotons(&nearestPhotons);
//    m_pPhotonMap->debug(&nearestPhotons);

    BBVertexBufferObject *pVBO = new BBVertexBufferObject(nPhotonNum);
    for (int i = 0; i < nPhotonNum; i++)
    {
        // The index of m_pPhotonMap starts from 1
        pVBO->setPosition(i, pPositions[i + 1]);
        if (m_pPhotonMap->isMarkedKNearestPhotons(i + 1))
            pVBO->setColor(i, BBConstant::m_OrangeRed);
        else
            pVBO->setColor(i, BBConstant::m_LightGreen);
    }
    BB_SAFE_DELETE_ARRAY(pPositions);

    // Display in real-time scene for easy observation
    BBScene *pRealTimeScene = BBSceneManager::getScene();
    BBModel *pScatterPlot = pRealTimeScene->createModel(pVBO, GL_POINTS, 0, nPhotonNum);
    pScatterPlot->setBoundingBoxVisibility(false);
    pRealTimeScene->update();
}
