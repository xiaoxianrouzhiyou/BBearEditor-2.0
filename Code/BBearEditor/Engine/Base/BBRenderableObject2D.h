#ifndef BBRENDERABLEOBJECT2D_H
#define BBRENDERABLEOBJECT2D_H


#include "Base/BBRenderableObject.h"

class BBRectBoundingBox2D;

class BBRenderableObject2D : public BBRenderableObject
{
public:
    BBRenderableObject2D();
    BBRenderableObject2D(int nCenterX, int nCenterY, int nWidth, int nHeight);
    ~BBRenderableObject2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

protected:
    BBRectBoundingBox2D *m_pBoundingBox2D;
};

#endif // BBRENDERABLEOBJECT2D_H
