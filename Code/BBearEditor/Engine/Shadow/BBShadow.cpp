#include "BBShadow.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"


void BBShadow::enable(int nAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRendering);

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
    setGBufferPass(pScene);
    setScreenQuadPass(pScene);
}

void BBShadow::setGBufferPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Shadow_VSM_GBuffer", BB_PATH_RESOURCE_SHADER(Shadow/VSM_GBuffer.vert), BB_PATH_RESOURCE_SHADER(Shadow/VSM_GBuffer.frag));

    // test
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    pMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->setCurrentMaterial(pMaterial->clone());
    }
}

void BBShadow::setScreenQuadPass(BBScene *pScene)
{
    // depth map is saved in the FBO 2
    // get d and d^2
    // filter them to get E(d) and E(d^2)
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Shadow_VSM_ScreenQuad", BB_PATH_RESOURCE_SHADER(Shadow/VSM_ScreenQuad.vert), BB_PATH_RESOURCE_SHADER(Shadow/VSM_ScreenQuad.frag));

    pMaterial->setSampler2D("AlbedoAndMetallicTex", pScene->getColorFBO(0, 0));
    pMaterial->setSampler2D("NormalAndDoubleRoughnessTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}
