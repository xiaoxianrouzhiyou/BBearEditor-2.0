#include "BBScreenSpaceRayTracker.h"
#include "Render/BBMaterial.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBTexture.h"


void BBScreenSpaceRayTracker::open(BBScene *pScene)
{
    setGBufferPass(pScene);
    setRayTracingPass(pScene);
}

void BBScreenSpaceRayTracker::setGBufferPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Efficient GPU Screen-Space Ray Tracing_GBuffer",
                    BB_PATH_RESOURCE_SHADER(RayTracing/0_GBuffer.vert),
                    BB_PATH_RESOURCE_SHADER(RayTracing/0_GBuffer.frag));

    BBTexture texture;
    pMaterial->setSampler2D("DiffuseTex", texture.createTexture2D(BB_PATH_RESOURCE_TEXTURE(gold.jpg)));

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->setCurrentMaterial(pMaterial->clone());
    }
}

void BBScreenSpaceRayTracker::setRayTracingPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Efficient GPU Screen-Space Ray Tracing_RayTracing",
                    BB_PATH_RESOURCE_SHADER(RayTracing/0_RayTracing.vert),
                    BB_PATH_RESOURCE_SHADER(RayTracing/0_RayTracing.frag));

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        BBMaterial *pMaterialInstance = pMaterial->clone();

        BBTexture texture;
        pMaterialInstance->setSampler2D("DiffuseTex", texture.createTexture2D(BB_PATH_RESOURCE_TEXTURE(gold.jpg)));

        pObject->setCurrentMaterial(pMaterialInstance);
    }
}
