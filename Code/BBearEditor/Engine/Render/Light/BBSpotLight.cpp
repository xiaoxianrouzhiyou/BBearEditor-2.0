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

    // m_Setting0[1] : Angle
    setAngle(30.0f);
    // m_Setting0[2] : Level
    setLevel(48.0f);

    // m_Setting1[0] : radius
    // m_Setting1[1] : constant factor
    // m_Setting1[2] : linear factor
    // m_Setting1[3] : quadric factor

    // The light shines downward
    // m_Setting2
    setDirection(0.0f, -1.0f, 0.0f, 1.0f);
}

bool BBSpotLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{

}

bool BBSpotLight::cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY)
{

}

void BBSpotLight::setDirection(float x, float y, float z, float w)
{
    m_Setting2[0] = x;
    m_Setting2[1] = y;
    m_Setting2[2] = z;
    m_Setting2[3] = w;
}
