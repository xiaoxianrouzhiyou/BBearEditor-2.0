#include "BBSpotLight.h"


BBSpotLight::BBSpotLight(BBScene *pScene)
    : BBSpotLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBSpotLight::BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation)
    : BBPointLight(pScene, position, rotation)
{
    m_eType = Spot;
}

bool BBSpotLight::cull(BBCamera *pCamera, const QRectF &displayBox)
{

}
