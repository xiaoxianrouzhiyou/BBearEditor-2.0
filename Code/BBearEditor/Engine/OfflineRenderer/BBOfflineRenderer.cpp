#include "BBOfflineRenderer.h"
#include "Utils/BBUtils.h"
#include "Scene/BBScene.h"
#include "3D/BBModel.h"


BBOfflineRenderer::BBOfflineRenderer(BBScene *pScene)
{
    m_pScene = pScene;
    m_pModels[0] = nullptr;
    m_pModels[1] = nullptr;
    m_pModels[2] = nullptr;
}

BBOfflineRenderer::~BBOfflineRenderer()
{
    BB_SAFE_DELETE(m_pModels[0]);
    BB_SAFE_DELETE(m_pModels[1]);
    BB_SAFE_DELETE(m_pModels[2]);
}

void BBOfflineRenderer::createTestModels()
{
    BB_PROCESS_ERROR_RETURN(!m_pModels[0]);
    m_pModels[0] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(teapot.obj), QVector3D(0.5f, 0, 7), QVector3D(0, 0, 0), QVector3D(0.01f, 0.01f, 0.01f));
    m_pModels[1] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(plane.obj), QVector3D(0, 0, 7), QVector3D(0, 0, 0), QVector3D(1, 1, 1));
    m_pModels[2] = m_pScene->createModel(BB_PATH_RESOURCE_MESH(sphere.obj), QVector3D(-0.6f, 0.5f, 7), QVector3D(0, 0, 0), QVector3D(1, 1, 1));
    m_pModels[0]->setBoundingBoxVisibility(false);
    m_pModels[1]->setBoundingBoxVisibility(false);
    m_pModels[2]->setBoundingBoxVisibility(false);
}
