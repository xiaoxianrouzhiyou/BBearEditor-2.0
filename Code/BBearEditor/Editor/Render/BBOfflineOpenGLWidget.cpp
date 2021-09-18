#include "BBOfflineOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "OfflineRenderer/BBOfflineRenderer.h"


BBOfflineOpenGLWidget::BBOfflineOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    m_pOfflineRenderer = nullptr;
}

BBOfflineOpenGLWidget::~BBOfflineOpenGLWidget()
{
    BB_SAFE_DELETE(m_pOfflineRenderer);
}

void BBOfflineOpenGLWidget::initializeGL()
{
    BBOpenGLWidget::initializeGL();
    m_pScene->enableHorizontalPlane(false);
    m_pOfflineRenderer = new BBOfflineRenderer(m_pScene);
}

void BBOfflineOpenGLWidget::resizeGL(int nWidth, int nHeight)
{
    BBOpenGLWidget::resizeGL(nWidth, nHeight);
    m_pOfflineRenderer->createTestScene();
}
