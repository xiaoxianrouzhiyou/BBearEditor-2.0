#include "BBOfflineRenderer.h"
#include "Utils/BBUtils.h"
#include "Scene/BBScene.h"
#include "3D/BBModel.h"
#include "BBCamera.h"
#include "Geometry/BBTracer.h"
#include "Geometry/BBPhotonMap.h"
#include "Render/Lighting/GameObject/BBAreaLight.h"
#include "Render/BBOfflineOpenGLWidget.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Scene/BBSceneManager.h"
#include "OfflineRenderer/BBScatterMaterial.h"


BBOfflineRenderer::BBOfflineRenderer(BBScene *pScene)
{
    m_pScene = pScene;
    m_nWidth = pScene->getCamera()->getViewportWidth();
    m_nHeight = pScene->getCamera()->getViewportHeight();
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i] = nullptr;
    }
    m_pAreaLight = nullptr;
    m_pPhotonMap = nullptr;
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
    m_pModels[7] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(sphere.obj), QVector3D(-0.4f, 0.5f, 1.5f), QVector3D(0, 0, 0), QVector3D(0.2f, 0.2f, 0.2f));
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i]->setBoundingBoxVisibility(false);
        m_pModels[i]->setScatterMaterial(new BBLambertian(QVector3D(1, 0, 0)));
    }
    m_pScene->getCamera()->setPosition(QVector3D(0, 1, 3.5f));
}

void BBOfflineRenderer::startPhotonMapping()
{
    generatePhotonMap();
    m_pPhotonMap->balance();
    showPhotonMap();
//    m_pPhotonMap->debug();
}

void BBOfflineRenderer::generatePhotonMap()
{
    int nMaxPhotonNum = 50000;
    m_pAreaLight = new BBAreaLight(-0.3f, 0.3f, -0.1f, -0.1f, 2);
    m_pPhotonMap = new BBPhotonMap(nMaxPhotonNum);
    QVector3D origin;
    QVector3D direction;
    QVector3D power(1.0f, 1.0f, 1.0f);
    float fPowerScale;

    for (int i = 0; i < nMaxPhotonNum; i++)
    {
        m_pAreaLight->generatePhoton(origin, direction, fPowerScale);
        BBRay ray(origin, direction);
        BBTracer::tracePhoton(ray, m_pModels, TestModelCount, 0, power * fPowerScale, m_pPhotonMap);
    }
}

void BBOfflineRenderer::showPhotonMap()
{
//    qDebug() << m_pPhotonMap->getBoxMin() << m_pPhotonMap->getBoxMax();
    int nPhotonNum = m_pPhotonMap->getPhotonNum();
    QVector3D *pPositions = m_pPhotonMap->getPhotonPositions();

    // test
    BBNearestPhotons nearestPhotons(QVector3D(0, 0, -1), 100, 0.2f);
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
