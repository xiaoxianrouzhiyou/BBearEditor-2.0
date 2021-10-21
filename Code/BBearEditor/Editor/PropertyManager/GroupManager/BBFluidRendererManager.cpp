#include "BBFluidRendererManager.h"
#include "Physics/FluidSystem/BBSPHFluidRenderer.h"


BBFluidRendererManager::BBFluidRendererManager(BBSPHFluidRenderer *pFluidRenderer, QWidget *pParent)
    : BBGroupManager("Render", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pFluidRenderer = pFluidRenderer;

    QCheckBox *pTriggerSSF = new QCheckBox(this);
    QObject::connect(pTriggerSSF, SIGNAL(clicked(bool)), this, SLOT(switchSSF(bool)));
    addFactory("SSF", pTriggerSSF);
}

BBFluidRendererManager::~BBFluidRendererManager()
{

}

void BBFluidRendererManager::switchSSF(bool bEnable)
{
    m_pFluidRenderer->switchSSF(bEnable);
}
