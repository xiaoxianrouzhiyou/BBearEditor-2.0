#include "BBGlobalIllumination.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/BBTexture.h"


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
    pFullScreenQuad->setTexture("AlbedoTex", pScene->getColorFBO(2, 0));
    pFullScreenQuad->setTexture("NormalTex", pScene->getColorFBO(2, 1));
    pFullScreenQuad->setTexture("PositionTex", pScene->getColorFBO(2, 2));
}