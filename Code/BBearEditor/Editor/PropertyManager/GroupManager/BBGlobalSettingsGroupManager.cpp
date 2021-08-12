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
    initRayTracingFactory();
    initSphericalHarmonicLightingFactory();
    initGlobalIlluminationFactory();
}

BBGlobalSettingsGroupManager::~BBGlobalSettingsGroupManager()
{
    BB_SAFE_DELETE(m_pTriggerRayTracing);
    BB_SAFE_DELETE(m_pSphericalHarmonicLightingFactory);
    BB_SAFE_DELETE(m_pGlobalIlluminationFactory);
}

void BBGlobalSettingsGroupManager::switchRayTracing(bool bEnable)
{

}

void BBGlobalSettingsGroupManager::bakeSphericalHarmonicLightingMap()
{
    BBSphericalHarmonicLighting::computeLightingData(m_pSphericalHarmonicLightingFactory->getCurrentItemIndex());
    emit updateFileList();
}

void BBGlobalSettingsGroupManager::switchGlobalIllumination(bool bEnable)
{
    BBSceneManager::enableDeferredRendering(0, bEnable);
}

void BBGlobalSettingsGroupManager::initRayTracingFactory()
{
    m_pTriggerRayTracing = new QCheckBox(this);
    QObject::connect(m_pTriggerRayTracing, SIGNAL(clicked(bool)), this, SLOT(switchRayTracing(bool)));
    addFactory("Ray Tracing", m_pTriggerRayTracing);
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
                                                   "SSAO + SSDO"};
    m_pGlobalIlluminationFactory = new BBEnumExpansionFactory("Global Illumination", globalIlluminationAlgorithmName,
                                                              "", "SSAO + SSDO", this, 1, 1);
    m_pGlobalIlluminationFactory->enableButton(false);
    QObject::connect(m_pGlobalIlluminationFactory, SIGNAL(triggerClicked(bool)), this, SLOT(switchGlobalIllumination(bool)));
    addFactory(m_pGlobalIlluminationFactory);
}
