#ifndef BBGEOMETRICPROCESSINGMANAGER_H
#define BBGEOMETRICPROCESSINGMANAGER_H


#include "BBGroupManager.h"

class BBProcedureMesh;
class BBGeometricProcessingManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBGeometricProcessingManager(BBProcedureMesh *pMesh, QWidget *pParent = 0);
    ~BBGeometricProcessingManager();

private slots:
    void triggerCatmullClarkAlgorithm(bool bEnable);

private:
    BBProcedureMesh *m_pMesh;
};

#endif // BBGEOMETRICPROCESSINGMANAGER_H
