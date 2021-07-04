#include "BBPointLight.h"
#include "3D/BBLightIndicator.h"
#include "Render/BBCamera.h"


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
    // m_Setting1[0] : radius
    setRadius(1.0f);
    // m_Setting1[1] : constant factor
    setConstantFactor(2.0f);
    // m_Setting1[2] : linear factor
    setLinearFactor(0.5f);
    // m_Setting1[3] : quadric factor
    setQuadricFactor(0.2f);
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
    m_Setting1[0] = fRadius;
    m_pIndicator->setScale(fRadius);
}

bool BBPointLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{
    QVector3D pointLightPosOnViewSpace = pCamera->getViewMatrix() * m_Position;
    QVector3D pointOnSpherePosOnViewSpace = pointLightPosOnViewSpace + QVector3D(getRadius(), 0.0f, 0.0f);
    QVector3D pointLightPosOnOpenGLScreenSpace = pCamera->projectPointToScreenSpace(pointLightPosOnViewSpace);
    QVector3D pointOnSpherePosOnOpenGLScreenSpace = pCamera->projectPointToScreenSpace(pointOnSpherePosOnViewSpace);
    // calculate AABB of the light on the screen space
    float r = pointOnSpherePosOnOpenGLScreenSpace.x() - pointLightPosOnOpenGLScreenSpace.x();
    QRectF lightBox = QRectF(pointLightPosOnOpenGLScreenSpace.x() - r, pointLightPosOnOpenGLScreenSpace.y() + r,
                             2.0f * r, 2.0f * r);
    if (lightBox.intersects(displayBox))
    {
        return false;
    }
    else
    {
        return true;
    }
}
