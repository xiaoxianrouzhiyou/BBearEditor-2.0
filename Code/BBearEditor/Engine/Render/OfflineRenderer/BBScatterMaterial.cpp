#include "BBScatterMaterial.h"
#include "Math/BBMath.h"


/**
 * @brief BBScatterMaterial::BBScatterMaterial
 */
BBScatterMaterial::BBScatterMaterial()
{

}

BBScatterMaterial::~BBScatterMaterial()
{

}


/**
 * @brief BBLambertian::BBLambertian
 * @param albedo
 */
BBLambertian::BBLambertian(const QVector3D &albedo)
{
    m_Albedo = albedo;
}

BBLambertian::~BBLambertian()
{

}

bool BBLambertian::scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo)
{
    QVector3D target = hitInfo.m_Position + hemisphericalRandom(hitInfo.m_Normal);
    scatterInfo.m_bSpecular = false;
    scatterInfo.m_ScatteredRay.setRay(hitInfo.m_Position, target - hitInfo.m_Position);
    scatterInfo.m_Attenuation = m_Albedo;
    return true;
}


/**
 * @brief BBMetal::BBMetal
 * @param albedo
 * @param fFuzz
 */
BBMetal::BBMetal(const QVector3D &albedo, float fFuzz)
{
    m_Albedo = albedo;
    m_fFuzz = fFuzz < 1 ? fFuzz : 1;
}

BBMetal::~BBMetal()
{

}

bool BBMetal::scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo)
{
    QVector3D reflected = reflect(ray.getDirection(), hitInfo.m_Normal);
    QVector3D scatteredRayDir = reflected + m_fFuzz * sphericalRandom();
    if (QVector3D::dotProduct(scatteredRayDir, hitInfo.m_Normal) <= 0)
    {
        return false;
    }
    scatterInfo.m_bSpecular = true;
    scatterInfo.m_ScatteredRay.setRay(hitInfo.m_Position, scatteredRayDir);
    scatterInfo.m_Attenuation = m_Albedo;
    return true;
}


/**
 * @brief BBDielectric::BBDielectric
 * @param fRefractivity
 */
BBDielectric::BBDielectric(float fRefractivity)
{
    m_fRefractivity = fRefractivity;
}

BBDielectric::~BBDielectric()
{

}

bool BBDielectric::scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo)
{
    QVector3D outNormal;
    QVector3D reflected = reflect(ray.getDirection(), hitInfo.m_Normal);
    float reflectance = 0.0f;
    // Transparent objects do not absorb energy
    scatterInfo.m_Attenuation = QVector3D(1, 1, 1);
    scatterInfo.m_bSpecular = true;
    QVector3D refracted = QVector3D(0, 0, 0);
    float fRefractivity = 1.0f;
    float cosRN = 0.0f;

    // When the light is transmitted from the medium to the outside of the medium, the normal will be reversed
    float cosLN = QVector3D::dotProduct(ray.getDirection(), hitInfo.m_Normal);
    if (cosLN > 0)
    {
        outNormal = -hitInfo.m_Normal;
        fRefractivity = m_fRefractivity;
        cosRN = fRefractivity * cosLN;
    }
    else
    {
        outNormal = hitInfo.m_Normal;
        fRefractivity = 1.0f / m_fRefractivity;
        cosRN = -cosLN;
    }
    // When there is no refraction, it reflects
    if (refract(ray.getDirection(), outNormal, fRefractivity, refracted))
    {
        reflectance = schlick(cosRN, fRefractivity);
    }
    else
    {
        // The reflectance is 100%
        reflectance = 1.0f;
    }
    // Use probability to decide
    if (frandom() <= reflectance)
    {
        scatterInfo.m_ScatteredRay.setRay(hitInfo.m_Position, reflected);
    }
    else
    {
        scatterInfo.m_ScatteredRay.setRay(hitInfo.m_Position, refracted);
    }
    return true;
}
