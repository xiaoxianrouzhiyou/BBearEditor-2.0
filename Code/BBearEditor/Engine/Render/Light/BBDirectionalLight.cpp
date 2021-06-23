#include "BBDirectionalLight.h"
#include "3D/BBLightIndicator.h"


BBDirectionalLight::BBDirectionalLight(BBScene *pScene)
    : BBDirectionalLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBDirectionalLight::BBDirectionalLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation)
    : BBLight(pScene, position, rotation, QVector3D(1, 1, 1))
{
    m_eType = Directional;
    m_pIndicator = new BBDirectionalLightIndicator(position, rotation);
}
