#ifndef BBSPOTLIGHT_H
#define BBSPOTLIGHT_H


#include "BBPointLight.h"

class BBSpotLight : public BBPointLight
{
public:
    BBSpotLight(BBScene *pScene);
    BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);

    bool cull(BBCamera *pCamera, const QRectF &displayBox) override;
    bool cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY) override;

    void setDirection(float x, float y, float z, float w = 1.0f);
    void setAngle(float fAngle) { m_Setting0[1] = fAngle; }
    void setLevel(float fValue) { m_Setting0[2] = fValue; }

    inline float* getDirection() { return m_Setting2; }
    inline float getAngle() { return m_Setting0[1]; }
    inline float getLevel() { return m_Setting0[2]; }
};

#endif // BBSPOTLIGHT_H
