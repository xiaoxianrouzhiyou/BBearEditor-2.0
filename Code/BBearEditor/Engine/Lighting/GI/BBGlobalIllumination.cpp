#include "BBGlobalIllumination.h"
#include "Scene/BBSceneManager.h"
#include "BBSSAOGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"


void BBGlobalIllumination::enable(bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::globalIlluminationRendering);

        useSSAOAlgorithm(pScene);
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

void BBGlobalIllumination::useSSAOAlgorithm(BBScene *pScene)
{
    BBSSAOGlobalIllumination::open(pScene);
}
