#include "BBParticleSystem.h"
#include "BBParticle.h"


BBParticleSystem::BBParticleSystem(const QVector3D &position)
    : BBGameObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_pParticle = new BBParticle(position);
}

BBParticleSystem::~BBParticleSystem()
{
    BB_SAFE_DELETE(m_pParticle);
}

void BBParticleSystem::init()
{
    m_pParticle->init();
}

void BBParticleSystem::render(BBCamera *pCamera)
{
    m_pParticle->render(pCamera);
}

void BBParticleSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pParticle->setPosition(position, bUpdateLocalTransform);
}
