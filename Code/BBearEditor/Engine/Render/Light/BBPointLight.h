#ifndef BBPOINTLIGHT_H
#define BBPOINTLIGHT_H


#include "BBLight.h"

class BBPointLight : public BBLight
{
public:
    BBPointLight(BBScene *pScene);
    BBPointLight(BBScene *pScene, const QVector3D &position);
    BBPointLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);

    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;

    void setRadius(float fRadius);
    float getRadius() { return m_Setting0[0]; }
};

#endif // BBPOINTLIGHT_H
