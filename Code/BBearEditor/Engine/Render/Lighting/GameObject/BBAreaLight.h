#ifndef BBAREALIGHT_H
#define BBAREALIGHT_H


#include "Base/BBGameObject.h"

class BBRectBoundingBox3D;

class BBAreaLight : public BBGameObject
{
public:
    BBAreaLight(float fMin0, float fMax0, float fMin1, float fMax1, float fFixedValue);
    ~BBAreaLight();

    void init() override;
    void render(BBCamera *pCamera) override;

    bool hit(const BBRay &ray, float &fDistance) override;

    void generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, const QVector3D &normal = QVector3D(0, -1, 0));
    void generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, const BBHitInfo &hitInfo);

    QVector3D getColor() { return m_Color; }

private:
    // Define location and size
    // For example, an area light source in an xoz plane, m_nFixedValue means y,
    // m_nMin0 and m_nMax0 indicates the range of x, m_nMin1 and m_nMax1 indicates the range of z
    float m_fMin0;
    float m_fMax0;
    float m_fMin1;
    float m_fMax1;
    float m_fFixedValue;
    QVector3D m_Normal;
    BBRectBoundingBox3D *m_pBoundingBox;
    QVector3D m_Color;
};

#endif // BBAREALIGHT_H
