#include "BBFLCGlobalIllumination.h"
#include "BBGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"


void BBFLCGlobalIllumination::open(BBScene *pScene)
{
    BBGlobalIllumination::setGBufferPass(pScene);
    setTriangleCutPass(pScene);
    setFullScreenQuadPass(pScene);
}

void BBFLCGlobalIllumination::setTriangleCutPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC_TriangleCut", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut.frag));

    pMaterial->setSampler2D("NormalTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));

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
