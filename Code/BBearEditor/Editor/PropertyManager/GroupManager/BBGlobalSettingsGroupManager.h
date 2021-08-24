#ifndef BBGLOBALSETTINGSGROUPMANAGER_H
#define BBGLOBALSETTINGSGROUPMANAGER_H


#include "BBGroupManager.h"

class BBEnumExpansionFactory;

class BBGlobalSettingsGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent = 0);
    ~BBGlobalSettingsGroupManager();

private slots:
    void switchSkyBoxAlgorithm(int nAlgorithmIndex);
    void switchRayTracing(bool bEnable);
    void bakeSphericalHarmonicLightingMap();
    void switchGlobalIllumination(bool bEnable);
    void switchShadow(bool bEnable);

signals:
    void updateFileList();

private:
    void initSkyBoxFactory();
    void initRayTracingFactory();
    void initSphericalHarmonicLightingFactory();
    void initGlobalIlluminationFactory();
    void initShadowFactory();

    BBScene *m_pScene;
    BBEnumFactory *m_pSkyBoxFactory;
    BBEnumExpansionFactory *m_pRayTracingFactory;
    BBEnumExpansionFactory *m_pSphericalHarmonicLightingFactory;
    BBEnumExpansionFactory *m_pGlobalIlluminationFactory;
    BBEnumExpansionFactory *m_pShadowFactory;
};

#endif // BBGLOBALSETTINGSGROUPMANAGER_H
