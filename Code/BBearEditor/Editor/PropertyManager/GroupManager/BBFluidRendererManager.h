#ifndef BBFLUIDRENDERERMANAGER_H
#define BBFLUIDRENDERERMANAGER_H


#include "BBGroupManager.h"

class BBSPHFluidRenderer;

class BBFluidRendererManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBFluidRendererManager(BBSPHFluidRenderer *pFluidRenderer, QWidget *pParent = 0);
    ~BBFluidRendererManager();

private slots:
    void switchSSF(bool bEnable);

private:
    BBSPHFluidRenderer *m_pFluidRenderer;
};

#endif // BBFLUIDRENDERERMANAGER_H
