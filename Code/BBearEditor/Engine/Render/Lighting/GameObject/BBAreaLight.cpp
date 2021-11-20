#include "BBAreaLight.h"
#include "Math/BBMath.h"
#include "Geometry/BBBoundingBox.h"
#include "OfflineRenderer/BBScatterMaterial.h"
#include "Render/BufferObject/BBVertexBufferObject.h"


BBAreaLight::BBAreaLight(float fMin0, float fMax0, float fMin1, float fMax1, float fFixedValue, const QVector3D &color)
    : BBModel((fMax0 + fMin0) / 2, fFixedValue, (fMax1 + fMin1) / 2, 0, 0, 0, (fMax0 - fMin0) * 0.1f, 1, (fMax1 - fMin1) * 0.1f, AREALIGHT)
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
    BBModel::init(BB_PATH_RESOURCE_MESH(plane.obj));

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
