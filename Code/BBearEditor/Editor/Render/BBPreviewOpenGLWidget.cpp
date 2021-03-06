#include "BBPreviewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Utils/BBUtils.h"
#include "3D/BBModel.h"
#include "Scene/BBRendererManager.h"
#include "Render/BBMaterial.h"


BBPreviewOpenGLWidget::BBPreviewOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    BBRendererManager::bindPreviewOpenGLWidget(this);
    // sphere for preview
    m_pSphere = NULL;
}

void BBPreviewOpenGLWidget::updateMaterialSphere(BBMaterial *pMaterial)
{
    BB_PROCESS_ERROR_RETURN(m_pSphere);
    BB_PROCESS_ERROR_RETURN(pMaterial);
    m_pSphere->setCurrentMaterial(pMaterial);
    update();
}

void BBPreviewOpenGLWidget::showMaterialPreview(const QString &filePath)
{
    if (!m_pSphere)
    {
        createSphere();
        m_pSphere->setCurrentMaterial(BBRendererManager::loadMaterial(filePath));
    }
    else
    {
        m_pSphere->setActivity(true);
        m_pSphere->setVisibility(false);
    }
    update();
}

void BBPreviewOpenGLWidget::removeMaterialPreview()
{
    BB_PROCESS_ERROR_RETURN(m_pSphere);
    m_pSphere->setActivity(false);
    update();
}

void BBPreviewOpenGLWidget::initializeGL()
{
    BBOpenGLWidget::initializeGL();
    m_pScene->enableSkyBox(false);
    m_pScene->enableHorizontalPlane(false);
}

void BBPreviewOpenGLWidget::createSphere()
{
    m_pSphere = m_pScene->createModelForPreview(BB_PATH_RESOURCE_MESH(sphere.obj));
}
