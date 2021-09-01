#include "BBGlobalIllumination.h"
#include "Scene/BBSceneManager.h"
#include "BBSSAOGlobalIllumination.h"
#include "BBSSDOGlobalIllumination.h"
#include "BBFLCGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"
#include "Render/BBMaterial.h"


void BBGlobalIllumination::enable(int nAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRenderingAndPostProcessing);

        switch (nAlgorithmIndex) {
        case 0:
            BBSSAOGlobalIllumination::open(pScene);
            break;
        case 1:
            BBSSDOGlobalIllumination::open(pScene);
            break;
        case 2:
            BBFLCGlobalIllumination::open(pScene);
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

void BBGlobalIllumination::setGBufferPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_GBuffer", BB_PATH_RESOURCE_SHADER(GI/GBuffer.vert), BB_PATH_RESOURCE_SHADER(GI/GBuffer.frag));

    // test
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    pMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->setCurrentMaterial(pMaterial);
    }
}
