#ifndef BBSPHFLUIDRENDERER_H
#define BBSPHFLUIDRENDERER_H


#include "Base/BBRenderableObject.h"

class BBSPHParticleSystem;

class BBSPHFluidRenderer : public BBRenderableObject
{
public:
    BBSPHFluidRenderer(const QVector3D &position = QVector3D(0, 0, 0));
    ~BBSPHFluidRenderer();

    void init(BBSPHParticleSystem *pParticleSystem);
    void render(BBCamera *pCamera) override;

public:
    void switchSSF(bool bEnable);

private:
    void initSSFMaterial();

    BBMaterial *m_pScreenQuadXFilterMaterial;
    BBMaterial *m_pScreenQuadYFilterMaterial;
    BBMaterial *m_pScreenQuadNormalMaterial;
    BBMaterial *m_pScreenQuadShadingMaterial;

private:
    BBSPHParticleSystem *m_pParticleSystem;
    const int m_nSSFGBufferMaterialIndex;
};

#endif // BBSPHFLUIDRENDERER_H
