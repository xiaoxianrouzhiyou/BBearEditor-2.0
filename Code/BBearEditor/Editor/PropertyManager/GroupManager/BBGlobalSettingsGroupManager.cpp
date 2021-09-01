#include "BBGlobalSettingsGroupManager.h"
#include "Scene/BBScene.h"
#include "RayTracing/BBRayTracker.h"
#include "../BBPropertyFactory.h"
#include "Render/BBDrawCall.h"
#include <QCheckBox>
#include "Lighting/BBSphericalHarmonicLighting.h"
#include "Scene/BBSceneManager.h"


BBGlobalSettingsGroupManager::BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent)
    : BBGroupManager("Global Settings", BB_PATH_RESOURCE_ICON(earth.png), pParent)
{
    m_pScene = pScene;
    initSkyBoxFactory();
    initRayTracingFactory();
    initSphericalHarmonicLightingFactory();
    initGlobalIlluminationFactory();
    initShadowFactory();
}

BBGlobalSettingsGroupManager::~BBGlobalSettingsGroupManager()
{
    BB_SAFE_DELETE(m_pSkyBoxFactory);
    BB_SAFE_DELETE(m_pRayTracingFactory);
    BB_SAFE_DELETE(m_pSphericalHarmonicLightingFactory);
    BB_SAFE_DELETE(m_pGlobalIlluminationFactory);
    BB_SAFE_DELETE(m_pShadowFactory);
}

void BBGlobalSettingsGroupManager::switchSkyBoxAlgorithm(int nAlgorithmIndex)
{
    m_pScene->changeSkyBoxAlgorithm(nAlgorithmIndex);
}

void BBGlobalSettingsGroupManager::switchRayTracing(bool bEnable)
{
    // parameter 1
    // 0 : Ray Tracing
    // parameter 2
    // 0 : Efficient GPU Screen-Space Ray Tracing
    BBSceneManager::enableDeferredRendering(0, m_pRayTracingFactory->getCurrentItemIndex(), bEnable);
}

void BBGlobalSettingsGroupManager::bakeSphericalHarmonicLightingMap()
{
    BBSphericalHarmonicLighting::computeLightingData(m_pSphericalHarmonicLightingFactory->getCurrentItemIndex());
    emit updateFileList();
}

void BBGlobalSettingsGroupManager::switchGlobalIllumination(bool bEnable)
{
    // parameter 1
    // 1 : GI
    // parameter 2
    // 0 : SSAO
    // 1 : SSDO
    // 2 : FLC
    BBSceneManager::enableDeferredRendering(1, m_pGlobalIlluminationFactory->getCurrentItemIndex(), bEnable);
}

void BBGlobalSettingsGroupManager::switchShadow(bool bEnable)
{
    BBSceneManager::enableDeferredRendering(2, m_pShadowFactory->getCurrentItemIndex(), bEnable);
}

void BBGlobalSettingsGroupManager::initSkyBoxFactory()
{
    QStringList skyBoxAlgorithmName = {"Common",
                                       "Octahedral Mapping"};
    m_pSkyBoxFactory = new BBEnumFactory("SkyBox", skyBoxAlgorithmName, "", this, 1, 1);
    QObject::connect(m_pSkyBoxFactory, SIGNAL(currentItemChanged(int)), this, SLOT(switchSkyBoxAlgorithm(int)));
    addFactory(m_pSkyBoxFactory);
}

void BBGlobalSettingsGroupManager::initRayTracingFactory()
{
    QStringList rayTracingAlgorithmName = {"Efficient GPU Screen-Space"};
    m_pRayTracingFactory = new BBEnumExpansionFactory("Ray Tracing", rayTracingAlgorithmName, "", "Efficient GPU Screen-Space", this, 1, 1);
    m_pRayTracingFactory->enableButton(false);
    QObject::connect(m_pRayTracingFactory, SIGNAL(triggerClicked(bool)), this, SLOT(switchRayTracing(bool)));
    addFactory(m_pRayTracingFactory);
}

void BBGlobalSettingsGroupManager::initSphericalHarmonicLightingFactory()
{
    QStringList sphericalHarmonicLightingAlgorithmName = {"The Gritty Details",
                                                          "Zonal Harmonics"};
    m_pSphericalHarmonicLightingFactory = new BBEnumExpansionFactory("Spherical Harmonic Lighting", sphericalHarmonicLightingAlgorithmName,
                                                                     "Bake", "Zonal Harmonics", this, 1, 1);
    m_pSphericalHarmonicLightingFactory->enableTrigger(false);
    QObject::connect(m_pSphericalHarmonicLightingFactory, SIGNAL(buttonClicked()), this, SLOT(bakeSphericalHarmonicLightingMap()));
    addFactory(m_pSphericalHarmonicLightingFactory);
}

void BBGlobalSettingsGroupManager::initGlobalIlluminationFactory()
{
    QStringList globalIlluminationAlgorithmName = {"SSAO",
                                                   "SSDO",
                                                   "FLC"};
    m_pGlobalIlluminationFactory = new BBEnumExpansionFactory("Global Illumination", globalIlluminationAlgorithmName, "", "FLC", this, 1, 1);
    m_pGlobalIlluminationFactory->enableButton(false);
    QObject::connect(m_pGlobalIlluminationFactory, SIGNAL(triggerClicked(bool)), this, SLOT(switchGlobalIllumination(bool)));
    addFactory(m_pGlobalIlluminationFactory);
}

void BBGlobalSettingsGroupManager::initShadowFactory()
{
    QStringList shadowAlgorithmName = {"VSM"};
    m_pShadowFactory = new BBEnumExpansionFactory("Shadow", shadowAlgorithmName, "", "", this, 1, 1);
    m_pShadowFactory->enableButton(false);
    QObject::connect(m_pShadowFactory, SIGNAL(triggerClicked(bool)), this, SLOT(switchShadow(bool)));
    addFactory(m_pShadowFactory);
}
