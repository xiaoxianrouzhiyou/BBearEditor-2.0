#ifndef BBSPHFLUIDRENDERER_H
#define BBSPHFLUIDRENDERER_H


#include "Base/BBRenderableObject.h"

class BBSPHParticleSystem;

class BBSPHFluidRenderer : public BBRenderableObject
{
public:
    BBSPHFluidRenderer(const QVector3D &position = QVector3D(0, 0, 0));

    void init(BBSPHParticleSystem *pParticleSystem);
    void render(BBCamera *pCamera) override;

private:
    BBSPHParticleSystem *m_pParticleSystem;
};

#endif // BBSPHFLUIDRENDERER_H
