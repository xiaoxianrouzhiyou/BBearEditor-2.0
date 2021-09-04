#include "BBSSDOGlobalIllumination.h"
#include "BBGlobalIllumination.h"
#include "BBSSAOGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBTexture.h"


void BBSSDOGlobalIllumination::open(BBScene *pScene)
{
    BBGlobalIllumination::setGBufferPass(pScene);
    setSSDOPass(pScene);
    setSSDOBlurPass(pScene);
}

void BBSSDOGlobalIllumination::setSSDOPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSDO", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/SSDO.frag));

    pMaterial->setSampler2D("LightTex", pScene->getColorFBO(0, 0));
    pMaterial->setSampler2D("NormalTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));
    BBTexture texture;
    pMaterial->setSampler2D("NoiseTex", texture.createTexture2D(BBSSAOGlobalIllumination::generateNoise(), 4, 4, GL_RGBA32F));
    pMaterial->setVector4Array("Samples[0]", BBSSAOGlobalIllumination::generateKernel(), 64);

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}

void BBSSDOGlobalIllumination::setSSDOBlurPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(1);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSDO_Blur", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/SSDO_Blur.frag));

    pMaterial->setSampler2D("SSAOTex", pScene->getColorFBO(1, 0));
    pMaterial->setSampler2D("SSDOTex", pScene->getColorFBO(1, 1));
    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(0, 0));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}
