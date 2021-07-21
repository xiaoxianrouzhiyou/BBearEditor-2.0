#include "BBRayTracker.h"
#include "Utils/BBUtils.h"
#include "Scene/BBSceneManager.h"
#include "Render/BBEditViewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Render/BBCamera.h"
#include "Render/BBMaterial.h"
#include "Scene/BBRendererManager.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBTexture.h"


BBRayTracker::BBRayTracker(BBScene *pScene)
{
    m_pScene = pScene;
    BBCamera *pCamera = m_pScene->getCamera();
    m_pRenderThread = nullptr;

    onWindowResize(pCamera->getViewportWidth(), pCamera->getViewportHeight());
}

BBRayTracker::~BBRayTracker()
{
    m_pRenderThread->quit();
    m_pRenderThread->wait();
    BB_SAFE_DELETE(m_pRenderThread);
}

void BBRayTracker::open()
{
    // Stop refresh per frame
    BBSceneManager::getEditViewOpenGLWidget()->stopRenderThread();

    m_pScene->setRenderingFunc(&BBScene::rayTracingRender);

    // open thread
    m_pRenderThread = new QThread(this);
    QObject::connect(m_pRenderThread, SIGNAL(started()), this, SLOT(render()));
    m_pRenderThread->start();
}

void BBRayTracker::close()
{
    BBSceneManager::getEditViewOpenGLWidget()->startRenderThread();
    m_pScene->setRenderingFunc(&BBScene::defaultRender);

    m_pRenderThread->quit();
    m_pRenderThread->wait();
    BB_SAFE_DELETE(m_pRenderThread);
}

void BBRayTracker::onWindowResize(int nWidth, int nHeight)
{
    m_nCurrentPixel = 0;
    m_nViewportWidth = nWidth;
    m_nViewportHeight = nHeight;
    m_nPixelCount = m_nViewportWidth * m_nViewportHeight;
    m_Image = QImage(m_nViewportWidth, m_nViewportHeight, QImage::Format_RGB888);
}

void BBRayTracker::render()
{
    while (m_nCurrentPixel < m_nPixelCount)
    {
        int x = m_nCurrentPixel % m_nViewportWidth;
        int y = m_nCurrentPixel / m_nViewportWidth;
        m_Image.setPixelColor(x, y, getPixelColor(x, y));

        m_nCurrentPixel++;
    }

    BBTexture texture;
    m_pScene->setFullScreenQuadTexture(LOCATION_TEXTURE(3), texture.createTexture2D(m_Image, GL_RGB));

    updateWindow();
}

void BBRayTracker::updateWindow()
{
    BBSceneManager::getEditViewOpenGLWidget()->update();
}

QColor BBRayTracker::getPixelColor(int x, int y)
{
    QColor color;
    BBRay inputRay = m_pScene->getCamera()->createRayFromScreen(x, y);
    m_pScene->pickObjectInModels(inputRay, false);

    return color;
}
