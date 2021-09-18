#include "BBOfflineRenderer.h"
#include "Utils/BBUtils.h"
#include "Scene/BBScene.h"
#include "3D/BBModel.h"
#include "BBCamera.h"
#include "Geometry/BBTracer.h"


BBOfflineRenderer::BBOfflineRenderer(BBScene *pScene)
{
    m_pScene = pScene;
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i] = nullptr;
    }
}

BBOfflineRenderer::~BBOfflineRenderer()
{
    for (int i = 0; i < TestModelCount; i++)
    {
        BB_SAFE_DELETE(m_pModels[i]);
    }
}

void BBOfflineRenderer::createTestScene()
{
    BB_PROCESS_ERROR_RETURN(!m_pModels[0]);
    m_pModels[0] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(1, 1, 1));
    m_pModels[1] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(-2, 0, 0), QVector3D(0, 0, -90), QVector3D(1, 1, 1));
    m_pModels[2] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 2, 0), QVector3D(0, 0, -180), QVector3D(1, 1, 1));
    m_pModels[3] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(2, 0, 0), QVector3D(0, 0, 90), QVector3D(1, 1, 1));
    m_pModels[4] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, -2), QVector3D(90, 0, 0), QVector3D(1, 1, 1));
    m_pModels[5] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(cube.obj), QVector3D(0.4f, 0, 0), QVector3D(0, 0, 0), QVector3D(0.5f, 0.5f, 0.5f));
    m_pModels[6] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(sphere.obj), QVector3D(-0.8f, 0.5f, 1), QVector3D(0, 0, 0), QVector3D(0.2f, 0.2f, 0.2f));
    for (int i = 0; i < TestModelCount; i++)
    {
        m_pModels[i]->setBoundingBoxVisibility(false);
    }
    m_pScene->getCamera()->setPosition(QVector3D(0, 1, 5));
}

void BBOfflineRenderer::generatePhotonMap()
{
    qDebug() << "generatePhotonMap()";
//    BBTracer::tracePhoton();
}
