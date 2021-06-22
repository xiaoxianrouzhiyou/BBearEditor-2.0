#ifndef BBPOINTLIGHT_H
#define BBPOINTLIGHT_H


#include "BBLight.h"

class BBPointLight : public BBLight
{
public:
    BBPointLight(BBScene *pScene);
    BBPointLight(BBScene *pScene, const QVector3D &position);
    BBPointLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);
};

#endif // BBPOINTLIGHT_H
