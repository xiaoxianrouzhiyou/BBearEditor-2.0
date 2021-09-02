#include "BBFLCGlobalIllumination.h"
#include "BBGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"


void BBFLCGlobalIllumination::open(BBScene *pScene)
{
//    BBGlobalIllumination::setGBufferPass(pScene);
    // Divide triangles evenly
    setTriangleCutPass(pScene);
    setIndirectShadingPass(pScene);
    setFullScreenQuadPass(pScene);
}

void BBFLCGlobalIllumination::setTriangleCutPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC_TriangleCut",
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_VS.vert),
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_FS.frag),
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_GS.shader));

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

void BBFLCGlobalIllumination::setIndirectShadingPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/FLC.frag));

    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(0, 0));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}

void BBFLCGlobalIllumination::setFullScreenQuadPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(1);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/FLC.frag));

    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(1, 0));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}
