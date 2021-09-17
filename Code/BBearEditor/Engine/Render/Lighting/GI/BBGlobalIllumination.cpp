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
        switch (nAlgorithmIndex) {
        case 0:
            pScene->setRenderingFunc(&BBScene::deferredRendering1_2);
            BBSSAOGlobalIllumination::open(pScene);
            break;
        case 1:
            pScene->setRenderingFunc(&BBScene::deferredRendering1_2);
            BBSSDOGlobalIllumination::open(pScene);
            break;
        case 2:
            pScene->setRenderingFunc(&BBScene::deferredRendering2_1);
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

/**
 * @brief BBGlobalIllumination::setGBufferPassByUsingExtraMaterial          When more materials need to be switched, store them in extramaterial
 * @param pScene
 */
void BBGlobalIllumination::setGBufferPassByUsingExtraMaterial(BBScene *pScene)
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
        pObject->setExtraMaterial(0, pMaterial);
    }
}
