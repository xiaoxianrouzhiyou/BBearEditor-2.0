#ifndef BBRENDERABLEOBJECT2D_H
#define BBRENDERABLEOBJECT2D_H


#include "Base/BBRenderableObject.h"

class BBRectBoundingBox2D;

class BBRenderableObject2D : public BBRenderableObject
{
public:
    BBRenderableObject2D();
    BBRenderableObject2D(int x, int y, int nWidth = 500, int nHeight = 500);
    ~BBRenderableObject2D();

    void init() override;
    void resize(float fWidth, float fHeight) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true);
    void setSize(int nWidth, int nHeight);

    virtual bool hit(int x, int y);

protected:

};

#endif // BBRENDERABLEOBJECT2D_H
