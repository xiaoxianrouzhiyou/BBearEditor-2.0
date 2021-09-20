#ifndef BBSCATTERMATERIAL_H
#define BBSCATTERMATERIAL_H


#include "Geometry/BBRay.h"

struct BBScatterInfo
{
    BBScatterInfo()
    {
        m_bSpecular = false;
    }

    bool m_bSpecular;
    BBRay m_ScatteredRay;
    QVector3D m_Attenuation;
};

// Ray changes that occur on the surface of a material in ray tracing
class BBScatterMaterial
{
public:
    BBScatterMaterial();
    virtual ~BBScatterMaterial();

    virtual bool scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo) = 0;
};

// diffuse
class BBLambertian : public BBScatterMaterial
{
public:
    BBLambertian(const QVector3D &albedo);
    ~BBLambertian();

    bool scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo) override;

private:
    QVector3D m_Albedo;
};

// specular
class BBMetal : public BBScatterMaterial
{
public:
    BBMetal(const QVector3D &albedo, float fFuzz = 0.0f);
    ~BBMetal();

    bool scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo) override;

private:
    QVector3D m_Albedo;
    float m_fFuzz;
};

// Transparent refraction model
class BBDielectric : public BBScatterMaterial
{
public:
    BBDielectric(float fRefractivity);
    ~BBDielectric();

    bool scatter(const BBRay &ray, const BBHitInfo &hitInfo, BBScatterInfo &scatterInfo) override;

private:
    float m_fRefractivity;
};

#endif // BBSCATTERMATERIAL_H
