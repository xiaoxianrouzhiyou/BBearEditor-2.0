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
    void changeCurrentRenderingAlgorithm(int nIndex);
    void switchRayTracing(bool bEnable);
    void bakeSphericalHarmonicLightingMap();
    void switchGlobalIllumination(bool bEnable);

signals:
    void updateFileList();

private:
    void initRenderingAlgorithmFactory();
    void initRayTracingFactory();
    void initSphericalHarmonicLightingFactory();
    void initGlobalIlluminationFactory();

    static int m_nCurrentRenderingAlgorithmIndex;
    BBScene *m_pScene;
    BBEnumFactory *m_pRenderingAlgorithmFactory;
    QCheckBox *m_pTriggerRayTracing;
    BBEnumExpansionFactory *m_pSphericalHarmonicLightingFactory;
    BBEnumExpansionFactory *m_pGlobalIlluminationFactory;
};

#endif // BBGLOBALSETTINGSGROUPMANAGER_H
