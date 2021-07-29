#ifndef BBSPOTLIGHT_H
#define BBSPOTLIGHT_H


#include "BBPointLight.h"

class BBSpotLight : public BBPointLight
{
public:
    BBSpotLight(BBScene *pScene);
    BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);

    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;

    bool cull(BBCamera *pCamera, const QRectF &displayBox) override;
    bool cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY) override;

    void setDirection();
    void setAngle(float fAngle) { m_Setting0[2] = fAngle; }
    void setLevel(float fValue) { m_Setting0[3] = fValue; }

    inline float* getDirection() { return m_Setting2; }
    inline float getAngle() { return m_Setting0[2]; }
    inline float getLevel() { return m_Setting0[3]; }
};

#endif // BBSPOTLIGHT_H
