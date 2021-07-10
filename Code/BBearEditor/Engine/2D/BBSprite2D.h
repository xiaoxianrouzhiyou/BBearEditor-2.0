#ifndef BBSPRITE2D_H
#define BBSPRITE2D_H


#include "Base/BBRenderableObject.h"

class BBRectBoundingBox2D;

class BBSprite2D : public BBRenderableObject
{
public:
    BBSprite2D();
    BBSprite2D(int nCenterX, int nCenterY, int nWidth, int nHeight);
    ~BBSprite2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

private:
    int m_nHalfWidth;
    int m_nHalfHeight;
    BBRectBoundingBox2D *m_pBoundingBox2D;
};

#endif // BBSPRITE2D_H
