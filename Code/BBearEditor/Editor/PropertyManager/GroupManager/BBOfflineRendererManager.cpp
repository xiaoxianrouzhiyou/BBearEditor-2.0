#include "BBOfflineRendererManager.h"
#include "Utils/BBUtils.h"
#include "../BBPropertyFactory.h"
#include "OfflineRenderer/BBOfflineRenderer.h"


BBOfflineRendererManager::BBOfflineRendererManager(BBOfflineRenderer *pOfflineRenderer, QWidget *pParent)
    : BBGroupManager("Offline Renderer", BB_PATH_RESOURCE_ICON(teapot.png), pParent)
{
    m_pOfflineRenderer = pOfflineRenderer;

    QStringList photonMappingAlgorithmName = {"Common"};
    BBEnumExpansionFactory *pPhotonMappingFactory = new BBEnumExpansionFactory("Photon Mapping", photonMappingAlgorithmName, "Bake", "", this, 1, 1);
    pPhotonMappingFactory->enableTrigger(false);
    QObject::connect(pPhotonMappingFactory, SIGNAL(buttonClicked()), this, SLOT(startPhotonMapping()));
    addFactory(pPhotonMappingFactory);
}

BBOfflineRendererManager::~BBOfflineRendererManager()
{

}

void BBOfflineRendererManager::startPhotonMapping()
{
    m_pOfflineRenderer->startPhotonMapping();
}
