#include "BBPointLight.h"
#include "3D/BBLightIndicator.h"


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
}
