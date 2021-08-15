#include "BBRayTracker.h"
#include "Utils/BBUtils.h"
#include "Scene/BBScene.h"
#include "Scene/BBSceneManager.h"
#include "BBScreenSpaceRayTracker.h"
#include "Base/BBGameObject.h"


void BBRayTracker::enable(int nAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        switch (nAlgorithmIndex) {
        case 0:
            BBScreenSpaceRayTracker::open(pScene);
            break;
        default:
            break;
        }
    }
    else
    {
        pScene->setRenderingFunc(&BBScene::defaultRendering);
        // objects go back original materials
        QList<BBGameObject*> objects = pScene->getModels();
        for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
        {
            BBGameObject *pObject = *itr;
            pObject->restoreMaterial();
        }
    }
}




//void BBRayTracker::open()
//{
//    // Stop refresh per frame
//    BBSceneManager::getEditViewOpenGLWidget()->stopRenderThread();

//    m_pScene->setRenderingFunc(&BBScene::rayTracingRendering);

//    // open thread
//    m_pRenderThread = new QThread(this);
//    QObject::connect(m_pRenderThread, SIGNAL(started()), this, SLOT(render()));
//    m_pRenderThread->start();
//}

//void BBRayTracker::close()
//{
//    BBSceneManager::getEditViewOpenGLWidget()->startRenderThread();
//    m_pScene->setRenderingFunc(&BBScene::defaultRendering);

//    m_pRenderThread->quit();
//    m_pRenderThread->wait();
//    BB_SAFE_DELETE(m_pRenderThread);
//}


//QColor BBRayTracker::getPixelColor(int x, int y)
//{
//    QColor color;
//    BBRay inputRay = m_pScene->getCamera()->createRayFromScreen(x, y);
//    m_pScene->pickObjectInModels(inputRay, false);

//    return color;
//}
