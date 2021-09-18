#include "BBAreaLight.h"
#include "Math/BBMath.h"


BBAreaLight::BBAreaLight(int nMin0, int nMax0, int nMin1, int nMax1, int nFixedValue)
{
    m_nMin0 = nMin0;
    m_nMax0 = nMax0;
    m_nMin1 = nMin1;
    m_nMax1 = nMax1;
    m_nFixedValue = nFixedValue;
}

void BBAreaLight::generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, BBHitInfo &hitInfo)
{
    // At present, only xoz plane area light sources are considered
    // Emit photons randomly from a point on the area light source
    origin = QVector3D(m_nMin0 + rand() * (m_nMax0 - m_nMin0), m_nFixedValue, m_nMin1 + rand() * (m_nMax1 - m_nMin1));
    // Randomly generated vector on hemispherical surface
    direction = hemisphericalRandom(hitInfo.m_Normal);
    // The intensity of light is related to the angle
    fPowerScale = QVector3D::dotProduct(direction, hitInfo.m_Normal);
}
