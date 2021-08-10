#ifndef BBGLOBALSETTINGSGROUPMANAGER_H
#define BBGLOBALSETTINGSGROUPMANAGER_H


#include "BBGroupManager.h"

class BBEnumAndButtonFactory;

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

signals:
    void updateFileList();

private:
    void initRenderingAlgorithmFactory();
    void initRayTracingFactory();
    void initSphericalHarmonicLightingFactory();

    static int m_nCurrentRenderingAlgorithmIndex;
    BBScene *m_pScene;
    BBEnumFactory *m_pRenderingAlgorithmFactory;
    QCheckBox *m_pTriggerRayTracing;
    BBEnumAndButtonFactory *m_pSphericalHarmonicLightingFactory;
};

#endif // BBGLOBALSETTINGSGROUPMANAGER_H
