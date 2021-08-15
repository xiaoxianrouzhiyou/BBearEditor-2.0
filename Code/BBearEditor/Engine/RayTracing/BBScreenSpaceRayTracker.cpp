#include "BBScreenSpaceRayTracker.h"
#include "Render/BBMaterial.h"
#include "Scene/BBScene.h"
#include "Base/BBGameObject.h"
#include "2D/BBFullScreenQuad.h"


void BBScreenSpaceRayTracker::open(BBScene *pScene)
{
    setGBufferPass(pScene);
    setRayTracingPass(pScene);
}

void BBScreenSpaceRayTracker::setGBufferPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Efficient GPU Screen-Space Ray Tracing_GBuffer",
                    BB_PATH_RESOURCE_SHADER(RayTracing/Efficient GPU Screen-Space Ray Tracing_GBuffer.vert),
                    BB_PATH_RESOURCE_SHADER(RayTracing/Efficient GPU Screen-Space Ray Tracing_GBuffer.frag));

    QList<BBGameObject*> objects = pScene->getModels();
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->setCurrentMaterial(pMaterial);
    }
}

void BBScreenSpaceRayTracker::setRayTracingPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("Efficient GPU Screen-Space Ray Tracing_RayTracing",
                    BB_PATH_RESOURCE_SHADER(RayTracing/Efficient GPU Screen-Space Ray Tracing_RayTracing.vert),
                    BB_PATH_RESOURCE_SHADER(RayTracing/Efficient GPU Screen-Space Ray Tracing_RayTracing.frag));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}
