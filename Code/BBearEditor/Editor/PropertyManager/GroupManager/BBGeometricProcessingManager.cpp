#include "BBGeometricProcessingManager.h"
#include "Utils/BBUtils.h"
#include "Geometry/BBMeshSubdivision.h"
#include <QCheckBox>
#include "3D/Mesh/BBProcedureMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"


BBGeometricProcessingManager::BBGeometricProcessingManager(BBProcedureMesh *pMesh, QWidget *pParent)
    : BBGroupManager("Geometry", BB_PATH_RESOURCE_ICON(model.png), pParent)
{
    m_pMesh = pMesh;

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
        BBCatmullClarkMeshSubdivision *pSubdivision = new BBCatmullClarkMeshSubdivision(m_pMesh);
    }
}
