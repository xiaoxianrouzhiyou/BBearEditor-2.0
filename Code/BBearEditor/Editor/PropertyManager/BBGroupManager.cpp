#include "BBGroupManager.h"
#include "BBUtils.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include <QTreeWidgetItem>
#include "BBPropertyFactory.h"
#include "BBGameObject.h"
#include "SceneManager/BBHierarchyTreeWidget.h"


//---------------------------------------------------------------------------------------------------
//  BBBaseInformationManager
//---------------------------------------------------------------------------------------------------

BBBaseInformationManager::BBBaseInformationManager(BBGameObject *pGameObject, QWidget *pParent)
    : QWidget(pParent)
{
    m_pCurrentGameObject = pGameObject;
    // Horizontal layout of icon and specific information
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(10, 0, 0, 0);

    // left side
    // icon
    QLabel *pIcon = new QLabel(this);
    pIcon->setFocusPolicy(Qt::NoFocus);
    QPixmap pixmap = QPixmap(BB_PATH_RESOURCE_ICON() + pGameObject->getIconName() + ".png");
    pixmap.setDevicePixelRatio(devicePixelRatio());
    pixmap = pixmap.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pIcon->setPixmap(pixmap);
    pLayout->addWidget(pIcon, 1, Qt::AlignLeft);

    // right side
    // specific information
    QWidget *pInfoWidget = new QWidget(this);
    QVBoxLayout *pInfoLayout = new QVBoxLayout(pInfoWidget);
    pInfoLayout->setMargin(0);
    // visibility button and name edit box in the top
    QWidget *pInfoWidgetTop = new QWidget(pInfoWidget);
    QHBoxLayout *pInfoLayoutTop = new QHBoxLayout(pInfoWidgetTop);
    pInfoLayoutTop->setMargin(0);
    m_pVisibilityButton = new QPushButton(pInfoWidgetTop);
    m_pVisibilityButton->setMaximumWidth(17);
    m_pVisibilityButton->setFocusPolicy(Qt::NoFocus);
    m_pVisibilityButton->setCheckable(true);
    setVisibilityButtonChecked(pGameObject->getActivity());
    QObject::connect(m_pVisibilityButton, SIGNAL(clicked()), this, SLOT(changeVisibility()));
    pInfoLayoutTop->addWidget(m_pVisibilityButton, 0, Qt::AlignLeft);
    m_pNameEdit = new QLineEdit(pInfoWidgetTop);
    m_pNameEdit->setText(pGameObject->getName());
//    QObject::connect(editName, SIGNAL(editingFinished()), this, SLOT(finishRename()));
    pInfoLayoutTop->addWidget(m_pNameEdit, 1);
    pInfoLayout->addWidget(pInfoWidgetTop);
    // tag and layer in the bottom
    QWidget *pInfoWidgetBottom = new QWidget(pInfoWidget);
    QHBoxLayout *pInfoLayoutBottom = new QHBoxLayout(pInfoWidgetBottom);
    pInfoLayoutBottom->setMargin(0);
    QStringList tagItems;
    tagItems.append("tag1");
    tagItems.append("tag2");
    pInfoLayoutBottom->addWidget(new BBEnumFactory("Tag", tagItems, "", pInfoWidgetBottom));
    QStringList layerItems;
    layerItems.append("layer1");
    layerItems.append("layer2");
    pInfoLayoutBottom->addWidget(new BBEnumFactory("Layer", layerItems, "", pInfoWidgetBottom));
    pInfoLayout->addWidget(pInfoWidgetBottom);
    pLayout->addWidget(pInfoWidget, 3);
}

BBBaseInformationManager::~BBBaseInformationManager()
{
    BB_SAFE_DELETE(m_pVisibilityButton);
    BB_SAFE_DELETE(m_pNameEdit);
}

void BBBaseInformationManager::changeVisibility()
{
    if (m_pVisibilityButton->isChecked())
    {
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(visible2.png)));
    }
    else
    {
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(invisible3.png)));
    }
    visibilityChanged(m_pCurrentGameObject, m_pVisibilityButton->isChecked());
}

void BBBaseInformationManager::setVisibilityButtonChecked(bool bChecked)
{
    m_pVisibilityButton->setChecked(bChecked);
    if (bChecked)
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(visible2.png)));
    else
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(invisible3.png)));
}


//---------------------------------------------------------------------------------------------------
//  BBSetBaseInformationManager
//---------------------------------------------------------------------------------------------------

BBSetBaseInformationManager::BBSetBaseInformationManager(BBGameObject *pCenterGameObject,
                                                         const QList<BBGameObject*> &gameObjectSet,
                                                         QWidget *pParent)
    : BBBaseInformationManager(pCenterGameObject, pParent)
{
    m_pNameEdit->setEnabled(false);
    m_CurrentGameObjectSet = gameObjectSet;
}

void BBSetBaseInformationManager::changeVisibility()
{
    if (m_pVisibilityButton->isChecked())
    {
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(visible2.png)));
    }
    else
    {
        m_pVisibilityButton->setStyleSheet(BB_STYLE_IMAGE(BB_PATH_RESOURCE_ICON(invisible3.png)));
    }
    visibilityChanged(m_CurrentGameObjectSet, m_pVisibilityButton->isChecked());
}


//---------------------------------------------------------------------------------------------------
//  BBGroupManager
//---------------------------------------------------------------------------------------------------

BBGroupManager::BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent)
    : QWidget(pParent)
{
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
    pLayout->addWidget(pLabel, 1);
    // factory showed in the right side
    pFactory->setParent(pWidget);
    pLayout->addWidget(pFactory, nStretch);
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


//---------------------------------------------------------------------------------------------------
//  BBTransformGroupManager
//---------------------------------------------------------------------------------------------------

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
    QTreeWidgetItem *pItem = BBHierarchyTreeWidget::m_ObjectMap.key(m_pCurrentGameObject);
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
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);
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
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);

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
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pCurrentGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);
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




//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
