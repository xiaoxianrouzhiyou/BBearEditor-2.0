#ifndef BBOFFLINERENDERERMANAGER_H
#define BBOFFLINERENDERERMANAGER_H


#include "BBGroupManager.h"

class BBOfflineRenderer;

class BBOfflineRendererManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBOfflineRendererManager(BBOfflineRenderer *pOfflineRenderer, QWidget *pParent = 0);
    ~BBOfflineRendererManager();

private slots:
    void startPhotonMapping();

private:
    BBOfflineRenderer *m_pOfflineRenderer;
};

#endif // BBOFFLINERENDERERMANAGER_H
