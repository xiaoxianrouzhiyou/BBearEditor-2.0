#ifndef BBDIRECTIONALLIGHT_H
#define BBDIRECTIONALLIGHT_H


#include "BBLight.h"

class BBDirectionalLight : public BBLight
{
public:
    BBDirectionalLight(BBScene *pScene);
    BBDirectionalLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);
};

#endif // BBDIRECTIONALLIGHT_H
