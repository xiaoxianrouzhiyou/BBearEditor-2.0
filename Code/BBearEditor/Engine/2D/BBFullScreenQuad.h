#ifndef BBFULLSCREENQUAD_H
#define BBFULLSCREENQUAD_H


#include "Base/BBRenderableObject.h"

class BBFullScreenQuad : public BBRenderableObject
{
public:
    BBFullScreenQuad();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setTexture(GLuint textureName);
};

#endif // BBFULLSCREENQUAD_H
