#include "BBGeometricProcessingManager.h"
#include "Utils/BBUtils.h"
#include "Geometry/BBMeshSubdivision.h"
#include <QCheckBox>


BBGeometricProcessingManager::BBGeometricProcessingManager(QWidget *pParent)
    : BBGroupManager("Geometry", BB_PATH_RESOURCE_ICON(model.png), pParent)
{
    QCheckBox *pCatmullClarkTrigger = new QCheckBox(this);
    addFactory("Catmull-Clark", pCatmullClarkTrigger, 1, Qt::AlignRight);
    QObject::connect(pCatmullClarkTrigger, SIGNAL(clicked(bool)), this, SLOT(triggerCatmullClarkAlgorithm(bool)));
}

BBGeometricProcessingManager::~BBGeometricProcessingManager()
{

}

void BBGeometricProcessingManager::triggerCatmullClarkAlgorithm(bool bEnable)
{
    if (bEnable)
    {
        BBCatmullClarkMeshSubdivision *pSubdivision = new BBCatmullClarkMeshSubdivision();
    }
}
