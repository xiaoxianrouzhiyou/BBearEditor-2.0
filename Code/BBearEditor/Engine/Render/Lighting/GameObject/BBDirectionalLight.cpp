#include "BBDirectionalLight.h"
#include "3D/BBLightIndicator.h"
#include "3D/BBIcon.h"
#include "Render/BBCamera.h"
#include "Scene/BBScene.h"


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

BBCamera* BBDirectionalLight::getLightSpaceCamera(int nLightPosX, int nLightPosY, int nLightPosZ)
{
    BBCamera *pLightSpaceCamera = new BBCamera();
    m_ViewMatrix.setToIdentity();
    m_ViewMatrix.lookAt(QVector3D(nLightPosX, 0.1, nLightPosZ), QVector3D(nLightPosX, 0, nLightPosZ), QVector3D(0, 0, -1));
    m_ViewMatrix.rotate(-m_Rotation.z(), 0, 0, 1);
    m_ViewMatrix.rotate(-m_Rotation.x(), 1, 0, 0);
    m_ViewMatrix.rotate(-m_Rotation.y(), 0, 1, 0);
    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.ortho(-10, 10, -10, 10, -10, 10);
//    m_ProjectionMatrix.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    pLightSpaceCamera->setViewMatrix(m_ViewMatrix);
    pLightSpaceCamera->setProjectionMatrix(m_ProjectionMatrix);
    return pLightSpaceCamera;
}

bool BBDirectionalLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{

}

bool BBDirectionalLight::cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY)
{

}

void BBDirectionalLight::setHomogeneousPosition()
{
    BBLight::setHomogeneousPosition(m_Quaternion * QVector3D(0, 1, 0), 0.0f);
}
