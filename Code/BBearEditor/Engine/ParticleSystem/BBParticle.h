#ifndef BBPARTICLE_H
#define BBPARTICLE_H


#include "Base/BBRenderableObject.h"

class BBShaderStorageBufferObject;
class BBTranslateFeedbackObject;
class BBComputeShader;

class BBParticle : public BBRenderableObject
{
public:
    BBParticle(const QVector3D &position = QVector3D(0, 0, 0));
    ~BBParticle();

    void init() override;
    void render(BBCamera *pCamera) override;

private:
    void create0();
    void update0(BBCamera *pCamera);

    void create1();
    void update1(BBCamera *pCamera);

    void create2();
    void update2(BBCamera *pCamera);

private:
    BBShaderStorageBufferObject *m_pSSBO;
    BBComputeShader *m_pUpdateCShader;
    BBTranslateFeedbackObject *m_pTFO;
};

#endif // BBPARTICLE_H
