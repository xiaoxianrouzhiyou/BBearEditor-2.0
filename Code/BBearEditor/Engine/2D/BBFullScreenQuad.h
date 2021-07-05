#ifndef BBFULLSCREENQUAD_H
#define BBFULLSCREENQUAD_H


#include "Base/BBRenderableObject.h"

class BBLight;

class BBFullScreenQuad : public BBRenderableObject
{
public:
    BBFullScreenQuad();
    BBFullScreenQuad(float fScale, float fOffsetX, float fOffsetY);

    void init() override;
    void render(BBCamera *pCamera) override;

//    void setAABB(float fWidth, float fHeight);

private:
    float m_fScale;
    float m_fOffsetX;
    float m_fOffsetY;
    // projecting screen space is imprecise
//    QRectF m_AABB;
};


class BBTiledFullScreenQuad : public BBGameObject
{
public:
    BBTiledFullScreenQuad();
    ~BBTiledFullScreenQuad();

    void init() override;
    void render(BBCamera *pCamera) override;

//    void setTiledAABB(float fWidth, float fHeight);

    void setTexture(const std::string &uniformName, GLuint textureName) override;

    void openLight() override;
    void closeLight() override;

private:
    BBFullScreenQuad *m_pFullScreenQuad[4];
    int m_nQuadCount;
};

#endif // BBFULLSCREENQUAD_H
