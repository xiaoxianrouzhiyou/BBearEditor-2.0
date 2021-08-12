#include "BBGlobalIllumination.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/BBTexture.h"
#include "BBSSAOGlobalIllumination.h"


void BBGlobalIllumination::enable(bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::globalIlluminationRendering);

        setRenderedObjectPass(pScene);
        setScreenQuadPass(pScene);
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

void BBGlobalIllumination::setRenderedObjectPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSAO+SSDO_RenderedObject",
                    BB_PATH_RESOURCE_SHADER(GI_SSAO+SSDO_RenderedObject.vert), BB_PATH_RESOURCE_SHADER(GI_SSAO+SSDO_RenderedObject.frag));

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

void BBGlobalIllumination::setScreenQuadPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad();
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSAO+SSDO_ScreenQuad",
                    BB_PATH_RESOURCE_SHADER(GI_SSAO+SSDO_ScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI_SSAO+SSDO_ScreenQuad.frag));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
    pFullScreenQuad->setTexture("AlbedoTex", pScene->getColorFBO(2, 1));
    pFullScreenQuad->setTexture("NormalTex", pScene->getColorFBO(2, 2));
    pFullScreenQuad->setTexture("PositionTex", pScene->getColorFBO(2, 3));
    BBTexture texture;
    pFullScreenQuad->setTexture("NoiseTex", texture.createTexture2D(BBSSAOGlobalIllumination::generateNoise(), 4, 4, GL_RGBA32F));
    pFullScreenQuad->setArrayVector4("Samples[0]", BBSSAOGlobalIllumination::generateKernel(), 64);
}
