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

void BBPreviewOpenGLWidget::createSphere()
{
    m_pScene->createModel(BB_PATH_RESOURCE_MESH(sphere.obj));
}
