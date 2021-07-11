#ifndef BBCANVAS_H
#define BBCANVAS_H


#include "Base/BBGameObject.h"

//class BBRectBoundingBox2D;
class BBSprite2D;

class BBCanvas : public BBGameObject
{
public:
    BBCanvas();
    BBCanvas(int nCenterX, int nCenterY, int nWidth = 500, int nHeight = 500);
    ~BBCanvas();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

public:
    void addSprite2D(BBSprite2D *pSprite2D);

private:
//    BBRectBoundingBox2D *m_pBoundingBox2D;
    QList<BBSprite2D*> m_Sprite2DSet;
};

#endif // BBCANVAS_H
