#include "BBFluidRendererManager.h"
#include "Physics/FluidSystem/BBSPHFluidRenderer.h"


BBFluidRendererManager::BBFluidRendererManager(BBSPHFluidRenderer *pFluidRenderer, QWidget *pParent)
    : BBGroupManager("Render", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pFluidRenderer = pFluidRenderer;

    QCheckBox *pTriggerSSF = new QCheckBox(this);
    QObject::connect(pTriggerSSF, SIGNAL(clicked(bool)), this, SLOT(switchSSF(bool)));
    addFactory("SSF", pTriggerSSF);

    QPushButton *pButtonReset = new QPushButton("Reset Particles", this);
    pButtonReset->setStyleSheet("QPushButton { border: none; border-radius: 2px; padding-left: 3px; padding-right: 3px; color: #d6dfeb; font: 9pt \"Arial\"; background: #0ebf9c; }"
                                "QPushButton:hover { background: #8c0ebf9c; }");
    QObject::connect(pButtonReset, SIGNAL(clicked()), this, SLOT(resetFluidParticles()));
    addFactory("", pButtonReset, 1, Qt::AlignLeft);
}

BBFluidRendererManager::~BBFluidRendererManager()
{

}

void BBFluidRendererManager::switchSSF(bool bEnable)
{
    m_pFluidRenderer->switchSSF(bEnable);
}

void BBFluidRendererManager::resetFluidParticles()
{
    m_pFluidRenderer->resetFluidParticles();
}
