#include "BBAreaLight.h"
#include "Math/BBMath.h"
#include "Geometry/BBBoundingBox.h"
#include "OfflineRenderer/BBScatterMaterial.h"
#include "Render/BufferObject/BBVertexBufferObject.h"


BBAreaLight::BBAreaLight(float fMin0, float fMax0, float fMin1, float fMax1, float fFixedValue, const QVector3D &color)
    : BBModel(AREALIGHT)
{
    // At present, only xoz plane area light sources are considered, Face down
    m_fMin0 = fMin0;
    m_fMax0 = fMax0;
    m_fMin1 = fMin1;
    m_fMax1 = fMax1;
    m_fFixedValue = fFixedValue;
    m_Normal = QVector3D(0, -1, 0);
    m_Color = color;
}

BBAreaLight::~BBAreaLight()
{

}

void BBAreaLight::init()
{
    BBVertexBufferObject *pVBO = new BBVertexBufferObject(4);
    pVBO->setPosition(0, m_fMin0, m_fFixedValue, m_fMin1);
    pVBO->setPosition(1, m_fMin0, m_fFixedValue, m_fMax1);
    pVBO->setPosition(2, m_fMax0, m_fFixedValue, m_fMax1);
    pVBO->setPosition(3, m_fMax0, m_fFixedValue, m_fMin1);
    pVBO->setColor(0, m_Color);
    pVBO->setColor(1, m_Color);
    pVBO->setColor(2, m_Color);
    pVBO->setColor(3, m_Color);

    BBModel::init(pVBO, GL_QUADS, 0, 4);

    setScatterMaterial(new BBAreaLightMaterial(QVector3D(1, 1, 1)));
}

void BBAreaLight::generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, const QVector3D &normal)
{
    // At present, only xoz plane area light sources are considered
    // Emit photons randomly from a point on the area light source
    origin = QVector3D(m_fMin0 + frandom() * (m_fMax0 - m_fMin0), m_fFixedValue, m_fMin1 + frandom() * (m_fMax1 - m_fMin1));
    // Randomly generated vector on hemispherical surface
    direction = hemisphericalRandom(normal);
    // The intensity of light is related to the angle
    fPowerScale = QVector3D::dotProduct(direction, normal);
}

void BBAreaLight::generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, const BBHitInfo &hitInfo)
{
    generatePhoton(origin, direction, fPowerScale, hitInfo.m_Normal);
}
