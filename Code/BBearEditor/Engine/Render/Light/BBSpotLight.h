#ifndef BBSPOTLIGHT_H
#define BBSPOTLIGHT_H


#include "BBPointLight.h"

class BBSpotLight : public BBPointLight
{
public:
    BBSpotLight(BBScene *pScene);
    BBSpotLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);

    bool cull(BBCamera *pCamera, const QRectF &displayBox) override;
    bool cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY) override;
};

#endif // BBSPOTLIGHT_H
