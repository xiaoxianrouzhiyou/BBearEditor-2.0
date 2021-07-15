#ifndef BBCANVAS_H
#define BBCANVAS_H


#include "Base/BBGameObject.h"

class BBAABBBoundingBox2D;
class BBSpriteObject2D;

class BBCanvas : public BBGameObject
{
public:
    BBCanvas(int x = 0, int y = 0, int nWidth = 630, int nHeight = 400);
    ~BBCanvas();

    void init() override;
    void render() override;
    void resize(float fWidth, float fHeight);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

    bool hit(int x, int y) override;

    const float* getUniformInfo() { return m_UniformInfo; }

public:
    void addSpriteObject2D(BBSpriteObject2D *pSpriteObject2D);

private:
    BBAABBBoundingBox2D *m_pAABBBoundingBox2D;
    QList<BBSpriteObject2D*> m_SpriteObject2DSet;
    float m_UniformInfo[4];
};

#endif // BBCANVAS_H
