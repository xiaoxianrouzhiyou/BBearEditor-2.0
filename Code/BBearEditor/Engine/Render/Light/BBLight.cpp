#include "BBLight.h"
#include "BBUtils.h"


BBLight* BBLight::m_pMainLight = nullptr;

BBLight::BBLight(BBScene *pScene)
    : BBLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{

}

BBLight::BBLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBGameObject(position, rotation, scale)
{
    m_eType = Directional;
}

BBLight::~BBLight()
{
    BB_SAFE_DELETE(m_pMainLight);
}

void BBLight::setAmbientColor(float r, float g, float b, float a)
{
    m_Ambient[0] = r;
    m_Ambient[1] = g;
    m_Ambient[2] = b;
    m_Ambient[3] = a;
}

void BBLight::setDiffuseColor(float r, float g, float b, float a)
{
    m_Diffuse[0] = r;
    m_Diffuse[1] = g;
    m_Diffuse[2] = b;
    m_Diffuse[3] = a;
}

void BBLight::setSpecularColor(float r, float g, float b, float a)
{
    m_Specular[0] = r;
    m_Specular[1] = g;
    m_Specular[2] = b;
    m_Specular[3] = a;
}

void BBLight::setSetting0(float x, float y, float z, float w)
{
    m_Setting0[0] = x;
    m_Setting0[1] = y;
    m_Setting0[2] = z;
    m_Setting0[3] = w;
}

void BBLight::setSetting1(float x, float y, float z, float w)
{
    m_Setting1[0] = x;
    m_Setting1[1] = y;
    m_Setting1[2] = z;
    m_Setting1[3] = w;
}
