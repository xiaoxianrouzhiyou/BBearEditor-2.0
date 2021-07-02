#ifndef BBFULLSCREENQUAD_H
#define BBFULLSCREENQUAD_H


#include "Base/BBRenderableObject.h"

class BBFullScreenQuad : public BBRenderableObject
{
public:
    BBFullScreenQuad();
    BBFullScreenQuad(float fScale, float fOffsetX, float fOffsetY);

    void init() override;
    void render(BBCamera *pCamera) override;

private:
    float m_fScale;
    float m_fOffsetX;
    float m_fOffsetY;
};


class BBTiledFullScreenQuad : public BBGameObject
{
public:
    BBTiledFullScreenQuad();
    ~BBTiledFullScreenQuad();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setTexture(const std::string &uniformName, GLuint textureName) override;

private:
    BBFullScreenQuad *m_pFullScreenQuad[4];
};

#endif // BBFULLSCREENQUAD_H
