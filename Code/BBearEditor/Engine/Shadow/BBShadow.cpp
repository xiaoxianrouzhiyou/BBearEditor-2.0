#include "BBShadow.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"


void BBShadow::enable(int nAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRenderingAndPostProcessing);

        open(pScene);
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

void BBShadow::open(BBScene *pScene)
{

}
