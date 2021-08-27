#include "BBHeadManager.h"
#include <QHBoxLayout>
#include <QLabel>
#include "Utils/BBUtils.h"
#include <QPushButton>
#include "Base/BBGameObject.h"
#include <QLineEdit>
#include "BBPropertyFactory.h"
#include "Scene/BBRendererManager.h"
#include "Render/BBMaterial.h"
#include "Shader/BBShader.h"
#include <QFileInfo>
#include <QDir>


/**
 * @brief BBBaseInformationManager::BBBaseInformationManager
 * @param pGameObject
 * @param pParent
 */
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


/**
 * @brief BBSetBaseInformationManager::BBSetBaseInformationManager
 * @param pCenterGameObject
 * @param gameObjectSet
 * @param pParent
 */
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


/**
 * @brief BBMaterialManager::BBMaterialManager
 * @param filePath
 * @param pParent
 */
BBMaterialManager::BBMaterialManager(const QString &filePath, QWidget *pParent)
    : QWidget(pParent)
{
    m_FilePath = filePath;
    m_pMaterial = BBRendererManager::loadMaterial(m_FilePath);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    // icon + info
    QWidget *pBaseInfoWidget = new QWidget(this);
    QHBoxLayout *pBaseInfoLayout = new QHBoxLayout(pBaseInfoWidget);
    pBaseInfoLayout->setContentsMargins(10, 0, 0, 0);
    m_pIcon = new QLabel(pBaseInfoWidget);
    m_pIcon->setFocusPolicy(Qt::NoFocus);
    setIcon();
    pBaseInfoLayout->addWidget(m_pIcon, 1, Qt::AlignLeft);
    // specific data on the right
    QWidget *pInfoWidget = new QWidget(pBaseInfoWidget);
    QVBoxLayout *pInfoLayout = new QVBoxLayout(pInfoWidget);
    pInfoLayout->setMargin(0);
    // file name on the top right
    QLabel *pLabelName = new QLabel(pInfoWidget);
    QString name = filePath.mid(filePath.lastIndexOf('/') + 1);
    name = name.mid(0, name.lastIndexOf('.'));
    pLabelName->setText(name);
    pLabelName->setStyleSheet("font: 75 9pt \"Arial\";");
    pInfoLayout->addWidget(pLabelName, 1);
    // shader on the bottom right
    setShaderEnumFactory(pInfoWidget);
    pInfoLayout->addWidget(m_pVShaderEnumFactory);
    pInfoLayout->addWidget(m_pFShaderEnumFactory);
    pBaseInfoLayout->addWidget(pInfoWidget, 3);
    pLayout->addWidget(pBaseInfoWidget);

    QObject::connect(m_pVShaderEnumFactory, SIGNAL(currentItemChanged(QString)),
                     this, SLOT(changeCurrentVShader(QString)));
    QObject::connect(m_pFShaderEnumFactory, SIGNAL(currentItemChanged(QString)),
                     this, SLOT(changeCurrentFShader(QString)));
}

BBMaterialManager::~BBMaterialManager()
{
    BB_SAFE_DELETE(m_pIcon);
    BB_SAFE_DELETE(m_pVShaderEnumFactory);
    BB_SAFE_DELETE(m_pFShaderEnumFactory);
}

void BBMaterialManager::changeCurrentVShader(const QString &name)
{
    BBRendererManager::changeVShader(m_pMaterial, name + ".vert");
}

void BBMaterialManager::changeCurrentFShader(const QString &name)
{
    BBRendererManager::changeFShader(m_pMaterial, name + ".frag");
}

void BBMaterialManager::setIcon()
{
    QPixmap pix(BB_PATH_RESOURCE_ICON(material5.png));
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(45 * devicePixelRatio(), 45 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_pIcon->setPixmap(pix);
}

void BBMaterialManager::setShaderEnumFactory(QWidget *pParent)
{
    char shaderName[64];
    strcpy(shaderName, m_pMaterial->getShader()->getShaderName());
    QString vShaderPath = m_pMaterial->getShader()->getVShaderPath();
    QString fShaderPath = m_pMaterial->getShader()->getFShaderPath();

    QString currentVName = vShaderPath.mid(QString(BB_PATH_RESOURCE_SHADER()).length());
    currentVName = BBUtils::getBaseName(currentVName);
    QString currentFName = fShaderPath.mid(QString(BB_PATH_RESOURCE_SHADER()).length());
    currentFName = BBUtils::getBaseName(currentFName);

    m_pVShaderEnumFactory = new BBEnumFactory("V Shader", BBRendererManager::loadVShaderList(), currentVName, pParent);
    m_pFShaderEnumFactory = new BBEnumFactory("F Shader", BBRendererManager::loadFShaderList(), currentFName, pParent);
}
