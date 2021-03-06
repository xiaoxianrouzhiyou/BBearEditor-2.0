#ifndef BBSPRITEOBJECT2D_H
#define BBSPRITEOBJECT2D_H


#include "Base/BBGameObject.h"

class BBSprite2D;
class BBAABBBoundingBox2D;

class BBSpriteObject2D : public BBGameObject
{
public:
    BBSpriteObject2D(int x = 0, int y = 0, int nWidth = 50, int nHeight = 50);
    ~BBSpriteObject2D();

    void init() override;
    void render(BBCanvas *pCanvas) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

private:
    BBSprite2D *m_pSprite2D;
    BBAABBBoundingBox2D *m_pAABBBoundingBox2D;
};

#endif // BBSPRITEOBJECT2D_H
