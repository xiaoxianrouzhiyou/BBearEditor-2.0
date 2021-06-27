#include "BBGroupManager.h"
#include "Utils/BBUtils.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include "Base/BBGameObject.h"
#include "BBPropertyFactory.h"
#include <QTreeWidgetItem>
#include "Scene/BBSceneManager.h"
#include "Render/BBDrawCall.h"
#include "Render/BBShader.h"
#include "Render/BBTexture.h"
#include "Render/BBPreviewOpenGLWidget.h"


/**
 * @brief BBGroupManager::BBGroupManager
 * @param groupName
 * @param iconPath
 * @param pParent
 */
BBGroupManager::BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent)
    : QWidget(pParent)
{
    m_pMenu = NULL;

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);

    // Upper bar
    QWidget *pBar = new QWidget(this);
    pBar->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *pBarLayout = new QHBoxLayout(pBar);
    pBarLayout->setMargin(0);
    pBarLayout->setSpacing(0);

    // button used to expand container
    m_pMainButton = new QToolButton(pBar);
    m_pMainButton->setCheckable(true);
    m_pMainButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    m_pMainButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // default is to expand the container
    m_pMainButton->setArrowType(Qt::DownArrow);
    m_pMainButton->setChecked(true);
    m_pMainButton->setText(groupName);
    QObject::connect(m_pMainButton, SIGNAL(clicked(bool)), this, SLOT(setContainerExpanded(bool)));
    pBarLayout->addWidget(m_pMainButton, 1);

    m_pMenuButton = new QPushButton(pBar);
    m_pMenuButton->setIcon(QIcon(iconPath));
    pBarLayout->addWidget(m_pMenuButton, 0, Qt::AlignRight);

    pLayout->addWidget(pBar);

    m_pContainer = new QWidget(this);
    // Add a vertical layout to the container for addFactory
    QVBoxLayout *pContainerLayout = new QVBoxLayout(m_pContainer);
    pContainerLayout->setContentsMargins(10, 0, 0, 0);
    pLayout->addWidget(m_pContainer);
}

BBGroupManager::~BBGroupManager()
{
    BB_SAFE_DELETE(m_pMainButton);
    BB_SAFE_DELETE(m_pMenuButton);
    BB_SAFE_DELETE(m_pMenu);
    BB_SAFE_DELETE(m_pContainer);
}

void BBGroupManager::addFactory(const QString &name, QWidget *pFactory, int nStretch)
{
    // Add a line of content to the container
    QWidget *pWidget = new QWidget(m_pContainer);
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setMargin(0);
    // name showed in the left side
    QLabel *pLabel = new QLabel(pWidget);
    pLabel->setText(name);
    pLayout->addWidget(pLabel, 1, Qt::AlignBottom);
    // factory showed in the right side
    pFactory->setParent(pWidget);
    pLayout->addWidget(pFactory, nStretch, Qt::AlignRight);
    m_pContainer->layout()->addWidget(pWidget);
}

void BBGroupManager::addFactory(QWidget *pFactory)
{
    QWidget *pWidget = new QWidget(m_pContainer);
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setMargin(0);
    pFactory->setParent(pWidget);
    pLayout->addWidget(pFactory);
    m_pContainer->layout()->addWidget(pWidget);
}

void BBGroupManager::setContainerExpanded(bool bExpanded)
{
    if (bExpanded)
    {
        // Modify the direction of the arrow
        m_pMainButton->setArrowType(Qt::DownArrow);
        // expand container
        m_pContainer->setVisible(true);
    }
    else
    {
        m_pMainButton->setArrowType(Qt::UpArrow);
        // collapse container
        // If you only set the container to be invisible
        // the height of the parent widget does not change at the moment
        // but container disappeared, and other widgets will be displayed in the center, which causes bad visual effects
        // First make the whole invisible, then make the container invisible, then restore the whole
        setVisible(false);
        m_pContainer->setVisible(false);
        setVisible(true);
    }
}


/**
 * @brief BBTransformGroupManager::BBTransformGroupManager
 * @param pGameObject
 * @param pParent
 */
BBTransformGroupManager::BBTransformGroupManager(BBGameObject *pGameObject, QWidget *pParent)
    : BBGroupManager("Transform [Global]", BB_PATH_RESOURCE_ICON(transform.png), pParent)
{
    m_pCurrentGameObject = pGameObject;
    // default is to show global coordinate
    m_eReferenceSystem = BBReferenceSystem::Global;
    m_pPositionFactory = new BBVector3DFactory(m_pCurrentGameObject->getPosition(), this);
    m_pRotationFactory = new BBVector3DFactory(m_pCurrentGameObject->getRotation(), this);
    m_pScaleFactory = new BBVector3DFactory(m_pCurrentGameObject->getScale(), this);
    addFactory("Position", m_pPositionFactory, 3);
    addFactory("Rotation", m_pRotationFactory, 3);
    addFactory("Scale", m_pScaleFactory, 3);

    QObject::connect(m_pPositionFactory, SIGNAL(valueChanged(QVector3D)), this, SLOT(changePosition(QVector3D)));
    QObject::connect(m_pRotationFactory, SIGNAL(valueChanged(QVector3D)), this, SLOT(changeRotation(QVector3D)));
    QObject::connect(m_pScaleFactory, SIGNAL(valueChanged(QVector3D)), this, SLOT(changeScale(QVector3D)));
    // pop menu
    QObject::connect(m_pMenuButton, SIGNAL(clicked()), this, SLOT(popMenu()));

    m_pMenu = new QMenu(this);
    QAction *pActionReset = new QAction(tr("Reset"));
    QAction *pActionResetPosition = new QAction(tr("Reset Position"));
    QAction *pActionResetRotation = new QAction(tr("Reset Rotation"));
    QAction *pActionResetScale = new QAction(tr("Reset Scale"));
    QAction *pActionGlobal = new QAction(tr("Global Transform"));
    QAction *pActionLocal = new QAction(tr("Local Transform"));
    m_pMenu->addAction(pActionReset);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionResetPosition);
    m_pMenu->addAction(pActionResetRotation);
    m_pMenu->addAction(pActionResetScale);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionGlobal);
    m_pMenu->addAction(pActionLocal);
    QObject::connect(pActionGlobal, SIGNAL(triggered()), this, SLOT(showGlobalCoordinate()));
    QObject::connect(pActionLocal, SIGNAL(triggered()), this, SLOT(showLocalCoordinate()));

    // if the item corresponding pGameObject is not at the top level, show local coordinate
    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(m_pCurrentGameObject);
    if (pItem)
    {
        pActionLocal->setEnabled(true);
        if (pItem->parent())
        {
            showLocalCoordinate();
        }
    }
    else
    {
        // BBGameObjectSet does not have local coordinate and corresponding item
        pActionLocal->setEnabled(false);
    }
}

BBTransformGroupManager::~BBTransformGroupManager()
{
    BB_SAFE_DELETE(m_pPositionFactory);
    BB_SAFE_DELETE(m_pRotationFactory);
    BB_SAFE_DELETE(m_pScaleFactory);
}

void BBTransformGroupManager::updatePositionValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pPositionFactory->setValue(m_pCurrentGameObject->getPosition());
    }
    else
    {
        m_pPositionFactory->setValue(m_pCurrentGameObject->getLocalPosition());
    }
}

void BBTransformGroupManager::updateRotationValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pRotationFactory->setValue(m_pCurrentGameObject->getRotation());
    }
    else
    {
        m_pRotationFactory->setValue(m_pCurrentGameObject->getLocalRotation());
    }
}

void BBTransformGroupManager::updateScaleValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pScaleFactory->setValue(m_pCurrentGameObject->getScale());
    }
    else
    {
        m_pScaleFactory->setValue(m_pCurrentGameObject->getLocalScale());
    }
}

void BBTransformGroupManager::changePosition(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pCurrentGameObject->setPosition(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBSceneManager::getSceneTreeItem(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBSceneManager::getGameObject(pParent);
            // Convert local coordinates to global coordinates
            m_pCurrentGameObject->setPosition(pParentObject->getPosition() + value);
        }
        else
        {
            // if at the top level, local coordinates == global coordinates
            m_pCurrentGameObject->setPosition(value);
        }
    }
    // The coordinate system needs to move with the object
    coordinateSystemUpdated();
}

void BBTransformGroupManager::changeRotation(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pCurrentGameObject->setRotation(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBSceneManager::getSceneTreeItem(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBSceneManager::getGameObject(pParent);

            QQuaternion rot = QQuaternion::fromEulerAngles(value);
            // Convert local coordinates to global coordinates
            rot = pParentObject->getQuaternion() * rot;
            m_pCurrentGameObject->setRotation(rot.toEulerAngles());
        }
        else
        {
            m_pCurrentGameObject->setRotation(value);
        }
    }
    coordinateSystemUpdated();
}

void BBTransformGroupManager::changeScale(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pCurrentGameObject->setScale(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBSceneManager::getSceneTreeItem(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBSceneManager::getGameObject(pParent);
            // Convert local coordinates to global coordinates
            m_pCurrentGameObject->setScale(pParentObject->getScale() * value);
        }
        else
        {
            m_pCurrentGameObject->setScale(value);
        }
    }
    coordinateSystemUpdated();
}

void BBTransformGroupManager::popMenu()
{
    m_pMenu->exec(QCursor::pos());
}

void BBTransformGroupManager::showGlobalCoordinate()
{
    // Repeat operation
    if (m_eReferenceSystem == BBReferenceSystem::Global)
        return;

    m_pMainButton->setText("Transform [Global]");
    m_eReferenceSystem = BBReferenceSystem::Global;
    // change value
    updatePositionValue();
    updateRotationValue();
    updateScaleValue();
}

void BBTransformGroupManager::showLocalCoordinate()
{
    if (m_eReferenceSystem == BBReferenceSystem::Local)
        return;

    m_pMainButton->setText("Transform [Local]");
    m_eReferenceSystem = BBReferenceSystem::Local;
    updatePositionValue();
    updateRotationValue();
    updateScaleValue();
}


/**
 * @brief BBGlobalSettingsGroupManager::BBGlobalSettingsGroupManager
 * @param pScene
 * @param pParent
 */
int BBGlobalSettingsGroupManager::m_nCurrentRenderingAlgorithmIndex = 0;

BBGlobalSettingsGroupManager::BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent)
    : BBGroupManager("Global Settings", BB_PATH_RESOURCE_ICON(earth.png), pParent)
{
    m_pScene = pScene;
    m_pRenderingAlgorithmEnumFactory = NULL;
    initRenderingAlgorithmEnumFactory();
}

BBGlobalSettingsGroupManager::~BBGlobalSettingsGroupManager()
{
    BB_SAFE_DELETE(m_pRenderingAlgorithmEnumFactory);
}

void BBGlobalSettingsGroupManager::changeCurrentRenderingAlgorithm(int nIndex)
{
    // 0 Forward Rendering
    // 1 Deferred Rendering
    BBDrawCall::setDrawFunc(nIndex);
    m_nCurrentRenderingAlgorithmIndex = nIndex;
}

void BBGlobalSettingsGroupManager::initRenderingAlgorithmEnumFactory()
{
    QStringList items;
    items.append("Forward Rendering");
    items.append("Deferred Rendering");
    m_pRenderingAlgorithmEnumFactory = new BBEnumFactory("Rendering Algorithm", items,
                                                         items[m_nCurrentRenderingAlgorithmIndex],
                                                         this, 1, 1);
    QObject::connect(m_pRenderingAlgorithmEnumFactory, SIGNAL(currentItemChanged(int)),
                     this, SLOT(changeCurrentRenderingAlgorithm(int)));
    addFactory(m_pRenderingAlgorithmEnumFactory);
}


/**
 * @brief BBMaterialPropertyGroupManager::BBMaterialPropertyGroupManager
 * @param pMaterial
 * @param pPreviewOpenGLWidget
 * @param pParent
 */
BBMaterialPropertyGroupManager::BBMaterialPropertyGroupManager(BBMaterial *pMaterial,
                                                               BBPreviewOpenGLWidget *pPreviewOpenGLWidget,
                                                               QWidget *pParent)
    : BBGroupManager("Main", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pMaterial = pMaterial;
    m_pPreviewOpenGLWidget = pPreviewOpenGLWidget;

    // read BBMaterialProperty
    setPropertyItems();
}

BBMaterialPropertyGroupManager::~BBMaterialPropertyGroupManager()
{

}

void BBMaterialPropertyGroupManager::setSampler2D(const QString &uniformName, const QString &texturePath)
{
    BBTexture texture;
    m_pMaterial->setSampler2D(uniformName.toStdString().c_str(), texture.createTexture2D(texturePath));
    m_pPreviewOpenGLWidget->update();
}

void BBMaterialPropertyGroupManager::setPropertyItems()
{
    QList<std::string> names;
    QList<BBMaterialUniformPropertyType> types;
    m_pMaterial->getShader()->getEditableProperties(names, types);
    for (int i = 0; i < types.count(); i++)
    {
        if (types[i] == BBMaterialUniformPropertyType::Sampler2D)
        {
            QString name = QString::fromStdString(names[i]);
            BBTextureFactory *pTextureFactory = new BBTextureFactory(name, this);
            addFactory(name, pTextureFactory);
            QObject::connect(pTextureFactory, SIGNAL(setSampler2D(QString, QString)),
                             this, SLOT(setSampler2D(QString, QString)));
        }
    }
}
