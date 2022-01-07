#ifndef BBGEOMETRICPROCESSINGMANAGER_H
#define BBGEOMETRICPROCESSINGMANAGER_H


#include "BBGroupManager.h"

class BBGeometricProcessingManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBGeometricProcessingManager(QWidget *pParent = 0);
    ~BBGeometricProcessingManager();

private slots:
    void triggerCatmullClarkAlgorithm(bool bEnable);
};

#endif // BBGEOMETRICPROCESSINGMANAGER_H
