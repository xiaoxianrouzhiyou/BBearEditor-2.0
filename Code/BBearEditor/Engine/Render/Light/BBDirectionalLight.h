#ifndef BBDIRECTIONALLIGHT_H
#define BBDIRECTIONALLIGHT_H


#include "BBLight.h"

class BBDirectionalLight : public BBLight
{
public:
    BBDirectionalLight(BBScene *pScene);
    BBDirectionalLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;

    bool cull(BBCamera *pCamera, const QRectF &displayBox) override;

private:
    void setHomogeneousPosition();
};

#endif // BBDIRECTIONALLIGHT_H
