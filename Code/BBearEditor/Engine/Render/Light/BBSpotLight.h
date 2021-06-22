#ifndef BBSPOTLIGHT_H
#define BBSPOTLIGHT_H


#include "BBPointLight.h"

class BBSpotLight : public BBPointLight
{
public:
    BBSpotLight(BBScene *pScene);
    BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);
};

#endif // BBSPOTLIGHT_H
