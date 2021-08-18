#ifndef BBPARTICLE_H
#define BBPARTICLE_H


#include "Base/BBRenderableObject.h"

class BBParticle : public BBRenderableObject
{
public:
    BBParticle(const QVector3D &position = QVector3D(0, 0, 0));

    void init() override;
    void render(BBCamera *pCamera) override;

private:
    void create0();
    void update0();

    void create1();
    void update1();
};

#endif // BBPARTICLE_H
