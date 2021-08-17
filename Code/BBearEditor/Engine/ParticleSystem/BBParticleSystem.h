#ifndef BBPARTICLESYSTEM_H
#define BBPARTICLESYSTEM_H


#include "Base/BBGameObject.h"

class BBParticle;

class BBParticleSystem : public BBGameObject
{
public:
    BBParticleSystem(const QVector3D &position = QVector3D(0, 0, 0)); 
    ~BBParticleSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;

private:
    BBParticle *m_pParticle;
};

#endif // BBPARTICLESYSTEM_H
