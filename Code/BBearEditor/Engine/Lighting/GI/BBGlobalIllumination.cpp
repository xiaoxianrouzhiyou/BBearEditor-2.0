#include "BBGlobalIllumination.h"
#include "Scene/BBSceneManager.h"
#include "BBSSAOGlobalIllumination.h"
#include "BBSSDOGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"


void BBGlobalIllumination::enable(int nSubAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRendering);

        switch (nSubAlgorithmIndex) {
        case 0:
            BBSSAOGlobalIllumination::open(pScene);
            break;
        case 1:
            BBSSDOGlobalIllumination::open(pScene);
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
