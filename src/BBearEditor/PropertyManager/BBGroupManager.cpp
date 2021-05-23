#include "BBGroupManager.h"
#include "BBUtils.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include <QTreeWidgetItem>
#include "BBPropertyFactory.h"
#include "BBGameObject.h"
#include "BBHierarchyTreeWidget.h"


//---------------------------------------------------------------------------------------------------
//  BBGroupManager
//---------------------------------------------------------------------------------------------------

BBGroupManager::BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent)
    : QWidget(pParent)
{
    QVBoxLayout layout(this);
    layout.setMargin(0);

    // Upper bar
    QWidget *pBar = new QWidget(this);
    pBar->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout barLayout(pBar);
    barLayout.setMargin(0);
    barLayout.setSpacing(0);

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
    barLayout.addWidget(m_pMainButton, 1);

    m_pMenuButton = new QPushButton(pBar);
    m_pMenuButton->setIcon(QIcon(iconPath));
    barLayout.addWidget(m_pMenuButton, 0, Qt::AlignRight);

    layout.addWidget(pBar);

    m_pContainer = new QWidget(this);
    // Add a vertical layout to the container for addFactory
    QVBoxLayout containerLayout(m_pContainer);
    containerLayout.setContentsMargins(10, 0, 0, 0);
    layout.addWidget(m_pContainer);
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
    QHBoxLayout layout(pWidget);
    layout.setMargin(0);
    // name showed in the left side
    QLabel *pLabel = new QLabel(pWidget);
    pLabel->setText(name);
    layout.addWidget(pLabel, 1);
    // factory showed in the right side
    pFactory->setParent(pWidget);
    layout.addWidget(pFactory, nStretch);
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
    : BBGroupManager("Transform (Global)", QString(BB_PATH_RESOURCE_ICON) + "transform.png", pParent)
{
    m_pGameObject = pGameObject;
    // default is to show global coordinate
    m_eReferenceSystem = BBReferenceSystem::Global;
    m_pPositionFactory = new BBVector3DFactory(m_pGameObject->getPosition(), this);
    m_pRotationFactory = new BBVector3DFactory(m_pGameObject->getRotation(), this);
    m_pScaleFactory = new BBVector3DFactory(m_pGameObject->getScale(), this);
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
    QTreeWidgetItem *pItem = BBHierarchyTreeWidget::m_ObjectMap.key(m_pGameObject);
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
    BB_SAFE_DELETE(m_pGameObject);
}

void BBTransformGroupManager::changePosition(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pGameObject->setPosition(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);
            // Convert local coordinates to global coordinates
            m_pGameObject->setPosition(pParentObject->getPosition() + value);
        }
        else
        {
            // if at the top level, local coordinates == global coordinates
            m_pGameObject->setPosition(value);
        }
    }
    // The coordinate system needs to move with the object
    coordinateSystemUpdated();
}

void BBTransformGroupManager::changeRotation(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pGameObject->setRotation(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);

            QQuaternion rot = QQuaternion::fromEulerAngles(value);
            // Convert local coordinates to global coordinates
            rot = pParentObject->getQuaternion() * rot;
            m_pGameObject->setRotation(rot.toEulerAngles());
        }
        else
        {
            m_pGameObject->setRotation(value);
        }
    }
    coordinateSystemUpdated();
}

void BBTransformGroupManager::changeScale(const QVector3D &value)
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pGameObject->setScale(value);
    }
    else
    {
        QTreeWidgetItem *pParent = BBHierarchyTreeWidget::m_ObjectMap.key(m_pGameObject)->parent();
        if (pParent)
        {
            BBGameObject *pParentObject = BBHierarchyTreeWidget::m_ObjectMap.value(pParent);
            // Convert local coordinates to global coordinates
            m_pGameObject->setScale(pParentObject->getScale() * value);
        }
        else
        {
            m_pGameObject->setScale(value);
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
    switchPositionValue();
    switchRotationValue();
    switchScaleValue();
}

void BBTransformGroupManager::showLocalCoordinate()
{
    if (m_eReferenceSystem == BBReferenceSystem::Local)
        return;

    m_pMainButton->setText("Transform [Local]");
    m_eReferenceSystem = BBReferenceSystem::Local;
    switchPositionValue();
    switchRotationValue();
    switchScaleValue();
}

void BBTransformGroupManager::switchPositionValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pPositionFactory->setValue(m_pGameObject->getPosition());
    }
    else
    {
        m_pPositionFactory->setValue(m_pGameObject->getLocalPosition());
    }
}

void BBTransformGroupManager::switchRotationValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pRotationFactory->setValue(m_pGameObject->getRotation());
    }
    else
    {
        m_pRotationFactory->setValue(m_pGameObject->getLocalRotation());
    }
}

void BBTransformGroupManager::switchScaleValue()
{
    if (m_eReferenceSystem == BBReferenceSystem::Global)
    {
        m_pScaleFactory->setValue(m_pGameObject->getScale());
    }
    else
    {
        m_pScaleFactory->setValue(m_pGameObject->getLocalScale());
    }
}


//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
