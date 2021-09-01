#include "BBRenderManager.h"
#include "Dialog/BBResourceDialog.h"
#include "3D/BBNormalIndicator.h"
#include "Scene/BBScene.h"
#include "Scene/BBRendererManager.h"
#include "Scene/BBSceneManager.h"
#include "../BBPropertyFactory.h"
#include "FileSystem/BBFileSystemDataManager.h"
#include "3D/BBNormalIndicator.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"


BBRenderManager::BBRenderManager(BBRenderableObject *pObject, QWidget *pParent)
    : BBGroupManager("Render", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pRenderableObject = pObject;
    QString materialPath = BBRendererManager::getMaterialPath(pObject->getMaterial());

    m_pAttributeColorFactory = new BBColorFactory(1.0f, 1.0f, 1.0f, 1.0f, this);
    addFactory("Attribute Color", m_pAttributeColorFactory, 1);
    QObject::connect(m_pAttributeColorFactory, SIGNAL(colorChanged(float, float, float, float)),
                     this, SLOT(changeAttributeColor(float, float, float, float)));

    m_pMaterialFactory = new BBDragAcceptedFactory(BB_PATH_RESOURCE_ICON(material5.png), materialPath, this);
    m_pMaterialFactory->setFilter(BBFileSystemDataManager::m_MaterialSuffixs);
    addFactory("Material", m_pMaterialFactory, 1);
    QObject::connect(m_pMaterialFactory, SIGNAL(iconClicked()), this, SLOT(popupResourceDialog()));
    QObject::connect(m_pMaterialFactory, SIGNAL(currentFilePathChanged(QString)), this, SLOT(changeMaterial(QString)));

    QCheckBox *pNormalIndicatorTrigger = new QCheckBox(this);
    addFactory("Normal Indicator", pNormalIndicatorTrigger, 1, Qt::AlignRight);
    QObject::connect(pNormalIndicatorTrigger, SIGNAL(clicked(bool)), this, SLOT(triggerNormalIndicator(bool)));

    QCheckBox *pLinePolygonModeTrigger = new QCheckBox(this);
    addFactory("Line PolygonMode", pLinePolygonModeTrigger, 1, Qt::AlignRight);
    QObject::connect(pLinePolygonModeTrigger, SIGNAL(clicked(bool)), this, SLOT(triggerLinePolygonMode(bool)));
}

BBRenderManager::~BBRenderManager()
{
    BB_SAFE_DELETE(m_pAttributeColorFactory);
    BB_SAFE_DELETE(m_pMaterialFactory);
}

void BBRenderManager::changeAttributeColor(float r, float g, float b, float a)
{
    m_pRenderableObject->getVBO()->setColor(r, g, b, a);
}

void BBRenderManager::changeMaterial(const QString &filePath)
{
    m_pRenderableObject->setCurrentMaterial(BBRendererManager::loadMaterial(filePath));
}

void BBRenderManager::popupResourceDialog()
{
    BBResourceDialog dialog(BB_PATH_RESOURCE_MATERIAL, this);
    if (dialog.exec())
    {
        m_pMaterialFactory->changeCurrentFilePath(dialog.getCurrentItemFilePath());
    }
}

void BBRenderManager::triggerNormalIndicator(bool bEnable)
{
    BBNormalIndicator *pNormalIndicator = BBSceneManager::getScene()->getNormalIndicator();
    if (bEnable)
    {
        pNormalIndicator->init(m_pRenderableObject);
        pNormalIndicator->setActivity(true);
    }
    else
    {
        pNormalIndicator->setActivity(false);
    }
}

void BBRenderManager::triggerLinePolygonMode(bool bEnable)
{
    if (bEnable)
    {
        m_pRenderableObject->getMaterial()->getBaseRenderPass()->setPolygonMode(GL_LINE);
    }
    else
    {
        m_pRenderableObject->getMaterial()->getBaseRenderPass()->setPolygonMode(GL_FILL);
    }
}
