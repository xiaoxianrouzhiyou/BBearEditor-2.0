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

signals:
    void updateFileList();

private:
    void initSkyBoxFactory();
    void initRayTracingFactory();
    void initSphericalHarmonicLightingFactory();
    void initGlobalIlluminationFactory();

    BBScene *m_pScene;
    BBEnumFactory *m_pSkyBoxFactory;
    BBEnumExpansionFactory *m_pRayTracingFactory;
    BBEnumExpansionFactory *m_pSphericalHarmonicLightingFactory;
    BBEnumExpansionFactory *m_pGlobalIlluminationFactory;
};

#endif // BBGLOBALSETTINGSGROUPMANAGER_H
