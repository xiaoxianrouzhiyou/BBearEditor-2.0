#ifndef BBSPHFLUIDRENDERER_H
#define BBSPHFLUIDRENDERER_H


#include "Base/BBRenderableObject.h"

class BBSPHParticleSystem;
class BBSPHFluidSystem;

class BBSPHFluidRenderer : public BBRenderableObject
{
public:
    BBSPHFluidRenderer(const QVector3D &position = QVector3D(0, 0, 0));
    ~BBSPHFluidRenderer();

    void init(BBSPHParticleSystem *pParticleSystem, BBSPHFluidSystem *pFluidSystem);
    void render(BBCamera *pCamera) override;

public:
    void switchSSF(bool bEnable);
    void resetFluidParticles();

private:
    void initSSFMaterial();

    BBMaterial *m_pScreenQuadXFilterMaterial;
    BBMaterial *m_pScreenQuadYFilterMaterial;
    BBMaterial *m_pScreenQuadNormalMaterial;
    BBMaterial *m_pScreenQuadShadingMaterial;

private:
    BBSPHParticleSystem *m_pParticleSystem;
    BBSPHFluidSystem *m_pFluidSystem;
    const int m_nSSFGBufferMaterialIndex;
};

#endif // BBSPHFLUIDRENDERER_H
