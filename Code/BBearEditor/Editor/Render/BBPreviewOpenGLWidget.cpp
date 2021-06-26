#include "BBPreviewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Utils/BBUtils.h"
#include "3D/BBModel.h"


BBPreviewOpenGLWidget::BBPreviewOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    // sphere for preview
    m_pSphere = NULL;
}

void BBPreviewOpenGLWidget::showMaterialPreview(const QString &filePath)
{
    if (!m_pSphere)
    {
        createSphere();
    }
    else
    {
        m_pSphere->setActivity(true);
        m_pSphere->setVisibility(false);
    }
}

void BBPreviewOpenGLWidget::removeMaterialPreview()
{
    if (m_pSphere)
    {
        m_pSphere->setActivity(false);
    }
}

void BBPreviewOpenGLWidget::createSphere()
{
    m_pSphere = m_pScene->createModelForPreview(BB_PATH_RESOURCE_MESH(sphere.obj));
}
