#include "BBLightManager.h"
#include "Render/Light/BBPointLight.h"
#include "../BBPropertyFactory.h"
#include "Render/Light/BBSpotLight.h"


/**
 * @brief BBPointLightManager::BBPointLightManager
 * @param pLight
 * @param pParent
 */
BBPointLightManager::BBPointLightManager(BBPointLight *pLight, QWidget *pParent)
    : BBGroupManager("Render", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pLight = pLight;

    BBLightColorFactory *pColorFactory = new BBLightColorFactory(pLight);
    addFactory("Color", pColorFactory, 1);

    BBLineEditFactory *pRadiusFactory = addFactory("Radius", pLight->getRadius());
    pRadiusFactory->setSlideStep(0.005f);
    pRadiusFactory->setRange(0, 1000);
    QObject::connect(pRadiusFactory, SIGNAL(valueChanged(float)), this, SLOT(setRadius(float)));

    BBLineEditFactory *pConstantFactory = addFactory("Constant Factor", pLight->getConstantFactor());
    pConstantFactory->setSlideStep(0.005f);
    pConstantFactory->setRange(0, 10);
    QObject::connect(pConstantFactory, SIGNAL(valueChanged(float)), this, SLOT(setConstantFactor(float)));

    BBLineEditFactory *pLinearFactory = addFactory("Linear Factor", pLight->getLinearFactor());
    pLinearFactory->setSlideStep(0.005f);
    pLinearFactory->setRange(0, 10);
    QObject::connect(pLinearFactory, SIGNAL(valueChanged(float)), this, SLOT(setLinearFactor(float)));

    BBLineEditFactory *pQuadricFactory = addFactory("Quadric Factor", pLight->getQuadricFactor());
    pQuadricFactory->setSlideStep(0.005f);
    pQuadricFactory->setRange(0, 10);
    QObject::connect(pQuadricFactory, SIGNAL(valueChanged(float)), this, SLOT(setQuadricFactor(float)));
}

BBPointLightManager::~BBPointLightManager()
{

}

void BBPointLightManager::setRadius(float fRadius)
{
    m_pLight->setRadius(fRadius);
}

void BBPointLightManager::setConstantFactor(float fValue)
{
    m_pLight->setConstantFactor(fValue);
}

void BBPointLightManager::setLinearFactor(float fValue)
{
    m_pLight->setLinearFactor(fValue);
}

void BBPointLightManager::setQuadricFactor(float fValue)
{
    m_pLight->setQuadricFactor(fValue);
}


/**
 * @brief BBSpotLightManager::BBSpotLightManager
 * @param pLight
 * @param pParent
 */
BBSpotLightManager::BBSpotLightManager(BBSpotLight *pLight, QWidget *pParent)
    : BBPointLightManager(pLight, pParent)
{

}

BBSpotLightManager::~BBSpotLightManager()
{

}
