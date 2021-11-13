#include "BBVolumetricCloud.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBProcedureTexture.h"


void BBVolumetricCloud::enable(int nAlgorithmIndex, bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRendering1_1);

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

void BBVolumetricCloud::open(BBScene *pScene)
{
    setGBufferPass(pScene);
    setScreenQuadPass(pScene);
}

void BBVolumetricCloud::setGBufferPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("VolumetricCloud_GBuffer", BB_PATH_RESOURCE_SHADER(Volumetric/GBuffer.vert), BB_PATH_RESOURCE_SHADER(Volumetric/GBuffer.frag));

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->setCurrentMaterial(pMaterial->clone());
    }
}

void BBVolumetricCloud::setScreenQuadPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("VolumetricCloud_ScreenQuad", BB_PATH_RESOURCE_SHADER(Volumetric/Cloud.vert), BB_PATH_RESOURCE_SHADER(Volumetric/Cloud.frag));

    pMaterial->setBlendState(true);

    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(0, 0));
    pMaterial->setSampler2D("NormalTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));
    pMaterial->setSampler2D("WeatherTex", BBTexture().createTexture2D(BB_PATH_RESOURCE_TEXTURE(Noise/weather.png)));
    pMaterial->setSampler2D("PerlinNoiseTex2D", BBProcedureTexture().createPerlinNoiseTexture2D(32, 0.0625f));
    pMaterial->setSampler3D("PerlinNoiseTex3D", BBProcedureTexture().create3D0(32, 32, 32));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}
