#include "BBPointLight.h"
#include "3D/BBLightIndicator.h"
#include "Render/BBCamera.h"
#include "Geometry/BBBoundingBox.h"


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
    m_pAABBBoundingBox3D = new BBAABBBoundingBox3D(m_Position, m_Rotation, m_Scale,
                                                   m_Position, QVector3D(1.0f, 1.0f, 1.0f));
    // m_Setting1[0] : radius
    setRadius(10.0f);
    // m_Setting1[1] : constant factor
    setConstantFactor(2.0f);
    // m_Setting1[2] : linear factor
    setLinearFactor(0.5f);
    // m_Setting1[3] : quadric factor
    setQuadricFactor(0.2f);
}

BBPointLight::~BBPointLight()
{
    BB_SAFE_DELETE(m_pAABBBoundingBox3D);
}

void BBPointLight::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBLight::setPosition(position, bUpdateLocalTransform);
    m_pAABBBoundingBox3D->setPosition(position, bUpdateLocalTransform);
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
    m_pAABBBoundingBox3D->setScale(fRadius);
}

bool BBPointLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{
    QVector3D pointLightPosOnViewSpace = pCamera->getViewMatrix() * m_Position;
    QVector3D pointOnSpherePosOnViewSpace = pointLightPosOnViewSpace + QVector3D(getRadius(), 0.0f, 0.0f);
    QVector3D pointLightPosOnOpenGLScreenSpace = pCamera->projectPointToScreenSpace(pointLightPosOnViewSpace);
    QVector3D pointOnSpherePosOnOpenGLScreenSpace = pCamera->projectPointToScreenSpace(pointOnSpherePosOnViewSpace);
    // calculate AABB of the light on the screen space
    float r = pointOnSpherePosOnOpenGLScreenSpace.x() - pointLightPosOnOpenGLScreenSpace.x();
    QRectF lightBox = QRectF(pointLightPosOnOpenGLScreenSpace.x() - r, pointLightPosOnOpenGLScreenSpace.y() - r,
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

bool BBPointLight::cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY)
{
    // at first, detect center
    if (pCamera->isFrustumContainPoint(nFrustumIndexX, nFrustumIndexY, 0, m_Position))
    {
        return false;
    }
    else
    {
        // detect bounding box and frustum
        return !pCamera->isFrustumIntersectWithAABB(nFrustumIndexX, nFrustumIndexY, 0, m_pAABBBoundingBox3D);
    }
}
