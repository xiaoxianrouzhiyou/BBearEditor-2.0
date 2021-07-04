#include "BBDirectionalLight.h"
#include "3D/BBLightIndicator.h"
#include "3D/BBIcon.h"


BBDirectionalLight::BBDirectionalLight(BBScene *pScene)
    : BBDirectionalLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBDirectionalLight::BBDirectionalLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation)
    : BBLight(pScene, position, rotation, QVector3D(1, 1, 1))
{
    m_eType = Directional;
    m_pIndicator = new BBDirectionalLightIndicator(position, rotation);
    setHomogeneousPosition();
}

void BBDirectionalLight::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pIcon->setPosition(position, bUpdateLocalTransform);
    m_pIndicator->setPosition(position, bUpdateLocalTransform);
}

void BBDirectionalLight::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBLight::setRotation(nAngle, axis, bUpdateLocalTransform);
    setHomogeneousPosition();
}

void BBDirectionalLight::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBLight::setRotation(rotation, bUpdateLocalTransform);
    setHomogeneousPosition();
}

bool BBDirectionalLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{

}

void BBDirectionalLight::setHomogeneousPosition()
{
    BBLight::setHomogeneousPosition(m_Quaternion * QVector3D(0, 1, 0), 0.0f);
}
