#include "BBSpotLight.h"
#include "3D/BBLightIndicator.h"


BBSpotLight::BBSpotLight(BBScene *pScene)
    : BBSpotLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBSpotLight::BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation)
    : BBPointLight(pScene, position, rotation)
{
    m_eType = Spot;
    m_pIndicator = new BBSpotLightIndicator(position, rotation);

    // Mark as spot light
    m_Setting0[0] = 2.0f;

    // m_Setting0[1] : Intensity
    setIntensity(4.0f);

    // m_Setting0[2] : Angle
    setAngle(30.0f);
    // m_Setting0[3] : Level
    setLevel(48.0f);

    // m_Setting1[0] : radius
    setRadius(5.0f);
    // m_Setting1[1] : constant factor
    // m_Setting1[2] : linear factor
    // m_Setting1[3] : quadric factor

    // The light shines downward
    // m_Setting2
    setDirection();
}

void BBSpotLight::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBLight::setRotation(nAngle, axis, bUpdateLocalTransform);
    setDirection();
}

void BBSpotLight::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBLight::setRotation(rotation, bUpdateLocalTransform);
    setDirection();
}

bool BBSpotLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{

}

bool BBSpotLight::cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY)
{

}

void BBSpotLight::setDirection()
{
    QVector3D dir = m_Quaternion * QVector3D(0.0f, -1.0f, 0.0f);
    m_Setting2[0] = dir.x();
    m_Setting2[1] = dir.y();
    m_Setting2[2] = dir.z();
    m_Setting2[3] = 1.0f;
}
