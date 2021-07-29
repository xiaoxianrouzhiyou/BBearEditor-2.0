#ifndef BBLIGHTMANAGER_H
#define BBLIGHTMANAGER_H


#include "BBGroupManager.h"

class BBPointLight;
class BBSpotLight;

class BBPointLightManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBPointLightManager(BBPointLight *pLight, QWidget *pParent = 0);
    ~BBPointLightManager();

protected slots:
    void setRadius(float fRadius);
    void setConstantFactor(float fValue);
    void setLinearFactor(float fValue);
    void setQuadricFactor(float fValue);

protected:
    BBPointLight *m_pLight;
};


class BBSpotLightManager : public BBPointLightManager
{
    Q_OBJECT

public:
    BBSpotLightManager(BBSpotLight *pLight, QWidget *pParent = 0);
    ~BBSpotLightManager();
};

#endif // BBLIGHTMANAGER_H
