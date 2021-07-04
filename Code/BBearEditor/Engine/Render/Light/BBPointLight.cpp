#include "BBPointLight.h"
#include "3D/BBLightIndicator.h"


BBPointLight::BBPointLight(BBScene *pScene)
    : BBPointLight(pScene, QVector3D(0, 0, 0))
{

}

BBPointLight::BBPointLight(BBScene *pScene, const QVector3D &position)
    : BBPointLight(pScene, position, QVector3D(0, 0, 0))
{

}

BBPointLight::BBPointLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation)
    : BBLight(pScene, position, rotation, QVector3D(1, 1, 1))
{
    m_eType = Point;
    m_pIndicator = new BBPointLightIndicator(position);
    // m_Setting0[0] : radius
    setRadius(1.0f);
}

void BBPointLight::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    Q_UNUSED(nAngle);
    Q_UNUSED(axis);
    Q_UNUSED(bUpdateLocalTransform);
}

void BBPointLight::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    Q_UNUSED(rotation);
    Q_UNUSED(bUpdateLocalTransform);
}

void BBPointLight::setRadius(float fRadius)
{
    m_Setting0[0] = fRadius;
    m_pIndicator->setScale(fRadius);
}
