#include "BBPropertyFactory.h"
#include <QHBoxLayout>
#include "Utils/BBUtils.h"
#include "BBFactoryComponent.h"
#include <QLineEdit>
#include <QRegExpValidator>
#include <QListView>
#include <QLabel>
#include <QComboBox>
#include "Lighting/GameObject/BBLight.h"
#include "FileSystem/BBFileSystemDataManager.h"
#include <QFile>
#include <QFileInfo>
#include "Render/BBMaterial.h"
#include <QCheckBox>


/**
 * @brief BBLineEditFactory::BBLineEditFactory
 * @param name
 * @param fValue
 * @param pParent
 * @param nNameStretch
 * @param nValueStretch
 */
BBLineEditFactory::BBLineEditFactory(const QString &name, float fValue, QWidget *pParent,
                                     int nNameStretch, int nValueStretch)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    // name lable in the left side
    m_pSliderLabel = new BBSliderLabel(this);
    m_pSliderLabel->setText(name);
    // otherwise, using tab key can focus on it
    m_pSliderLabel->setFocusPolicy(Qt::NoFocus);
    pLayout->addWidget(m_pSliderLabel, nNameStretch, Qt::AlignLeft);
    // edit in the right side
    m_pEdit = new QLineEdit(this);
    // m_pEdit->setAlignment(Qt::AlignRight);
    // Limit the range of float as [-999999.999999, 999999.999999]
    setRange(-1000000, 1000000);
    setRegExp("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,6})?$|(^\\t?$)");
    setSlideStep(0.2f);
    setValue(fValue);
    pLayout->addWidget(m_pEdit, nValueStretch);

    QObject::connect(m_pSliderLabel, SIGNAL(slide(int)), this, SLOT(changeValueBySlider(int)));
    QObject::connect(m_pEdit, SIGNAL(textEdited(QString)), this, SLOT(changeEditText(QString)));
    QObject::connect(m_pEdit, SIGNAL(editingFinished()), this, SLOT(showFromLeft()));
}

BBLineEditFactory::~BBLineEditFactory()
{
    BB_SAFE_DELETE(m_pSliderLabel);
    BB_SAFE_DELETE(m_pEdit);
}

void BBLineEditFactory::setRange(float fMin, float fMax)
{
    m_fMinValue = fMin;
    m_fMaxValue = fMax;
}

void BBLineEditFactory::setRegExp(const QString &pattern)
{
    // limit input
    QRegExp re(pattern);
    QRegExpValidator *pValidator = new QRegExpValidator(re, m_pEdit);
    m_pEdit->setValidator(pValidator);
}

QString BBLineEditFactory::setValue(float fValue)
{
    // do not use scientific notation, and do not show the extra 0 at the end
    QString text = QString("%1").arg(fValue, 0, 'f').replace(QRegExp("(\\.){0,1}0+$"), "");
    m_pEdit->setText(text);
    showFromLeft();
    return text;
}

void BBLineEditFactory::changeEditText(const QString &text)
{
    valueChanged(text.toFloat());
    valueChanged(m_pSliderLabel->text(), text.toFloat());
}

void BBLineEditFactory::changeValueBySlider(int nDeltaX)
{
    float fValue = m_pEdit->text().toFloat();
    fValue += nDeltaX * m_fSlideStep;
    // Limit scope
    if (fValue >= m_fMaxValue)
        fValue = m_fMaxValue;
    if (fValue <= m_fMinValue)
        fValue = m_fMinValue;
    QString text = setValue(fValue);
    // trigger textEdited signal, change corresponding object in scene
    m_pEdit->textEdited(text);
}

void BBLineEditFactory::showFromLeft()
{
    // Display from the first place, otherwise you won’t see the minus sign
    m_pEdit->setCursorPosition(0);
}


/**
 * @brief BBVector2DFactory::BBVector2DFactory
 * @param value
 * @param pParent
 */
BBVector2DFactory::BBVector2DFactory(const QVector2D &value, QWidget *pParent)
    : QWidget(pParent)
{
    m_Value = value;
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);

    m_pEditX = new BBLineEditFactory("X", value.x(), this);
    m_pEditY = new BBLineEditFactory("Y", value.y(), this);
    pLayout->addWidget(m_pEditX);
    pLayout->addWidget(m_pEditY);

    QObject::connect(m_pEditX, SIGNAL(valueChanged(float)), this, SLOT(setX(float)));
    QObject::connect(m_pEditY, SIGNAL(valueChanged(float)), this, SLOT(setY(float)));
}

BBVector2DFactory::~BBVector2DFactory()
{
    BB_SAFE_DELETE(m_pEditX);
    BB_SAFE_DELETE(m_pEditY);
}

void BBVector2DFactory::setValue(const QVector2D &value)
{
    m_Value = value;
    m_pEditX->setValue(value.x());
    m_pEditY->setValue(value.y());
}

void BBVector2DFactory::setX(float x)
{
    m_Value.setX(x);
    valueChanged(m_Value);
}

void BBVector2DFactory::setY(float y)
{
    m_Value.setY(y);
    valueChanged(m_Value);
}


/**
 * @brief BBVector3DFactory::BBVector3DFactory
 * @param value
 * @param pParent
 */
BBVector3DFactory::BBVector3DFactory(const QVector3D &value, QWidget *pParent)
    : QWidget(pParent)
{
    m_Value = value;
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);

    m_pEditX = new BBLineEditFactory("X", value.x(), this);
    m_pEditY = new BBLineEditFactory("Y", value.y(), this);
    m_pEditZ = new BBLineEditFactory("Z", value.z(), this);
    pLayout->addWidget(m_pEditX);
    pLayout->addWidget(m_pEditY);
    pLayout->addWidget(m_pEditZ);

    QObject::connect(m_pEditX, SIGNAL(valueChanged(float)), this, SLOT(setX(float)));
    QObject::connect(m_pEditY, SIGNAL(valueChanged(float)), this, SLOT(setY(float)));
    QObject::connect(m_pEditZ, SIGNAL(valueChanged(float)), this, SLOT(setZ(float)));
}

BBVector3DFactory::~BBVector3DFactory()
{
    BB_SAFE_DELETE(m_pEditX);
    BB_SAFE_DELETE(m_pEditY);
    BB_SAFE_DELETE(m_pEditZ);
}

void BBVector3DFactory::setValue(const QVector3D &value)
{
    m_Value = value;
    m_pEditX->setValue(value.x());
    m_pEditY->setValue(value.y());
    m_pEditZ->setValue(value.z());
}

void BBVector3DFactory::setX(float x)
{
    m_Value.setX(x);
    valueChanged(m_Value);
}

void BBVector3DFactory::setY(float y)
{
    m_Value.setY(y);
    valueChanged(m_Value);
}

void BBVector3DFactory::setZ(float z)
{
    m_Value.setZ(z);
    valueChanged(m_Value);
}


/**
 * @brief BBEnumFactory::BBEnumFactory
 * @param name
 * @param comboBoxItems
 * @param currentText
 * @param pParent
 * @param labelStretch
 * @param comboBoxStretch
 */
BBEnumFactory::BBEnumFactory(const QString &name, const QStringList &comboBoxItems,
                             const QString &currentText, QWidget *pParent, int labelStretch, int comboBoxStretch)
    : QWidget(pParent)
{
    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->setMargin(0);
    m_pLabel = new QLabel(name, this);
    m_pLabel->setFocusPolicy(Qt::NoFocus);
    pLayout->addWidget(m_pLabel, 0, 0, 1, labelStretch);

    m_pComboBox = new QComboBox(this);
    m_pComboBox->addItems(comboBoxItems);
    // With this sentence, qss can take effect
    m_pComboBox->setView(new QListView());
    m_pComboBox->setCurrentText(currentText);
    pLayout->addWidget(m_pComboBox, 0, 1, 1, comboBoxStretch);

    QObject::connect(m_pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrentItem(int)));
    QObject::connect(m_pComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCurrentItem(QString)));
}

BBEnumFactory::~BBEnumFactory()
{
    BB_SAFE_DELETE(m_pLabel);
    BB_SAFE_DELETE(m_pComboBox);
}

int BBEnumFactory::getCurrentItemIndex()
{
    return m_pComboBox->currentIndex();
}

void BBEnumFactory::changeCurrentItem(int nIndex)
{
    emit currentItemChanged(nIndex);
}

void BBEnumFactory::changeCurrentItem(const QString &text)
{
    emit currentItemChanged(text);
}


/**
 * @brief BBEnumExpansionFactory::BBEnumExpansionFactory
 * @param name
 * @param comboBoxItems
 * @param buttonText
 * @param currentText
 * @param pParent
 * @param labelStretch
 * @param comboBoxStretch
 */
BBEnumExpansionFactory::BBEnumExpansionFactory(const QString &name, const QStringList &comboBoxItems, const QString &buttonText,
                                               const QString &currentText, QWidget *pParent, int labelStretch, int comboBoxStretch)
    : BBEnumFactory(name, comboBoxItems, currentText, pParent, labelStretch, comboBoxStretch)
{
    QWidget *pWidget = new QWidget(this);
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setMargin(0);

    m_pTrigger = new QCheckBox(pWidget);
    pLayout->addWidget(m_pTrigger);

    m_pButton = new QPushButton(buttonText, pWidget);
    m_pButton->setStyleSheet("QPushButton { border: none; border-radius: 2px; padding-left: 3px; padding-right: 3px; color: #d6dfeb; font: 9pt \"Arial\"; background: #0ebf9c; }"
                             "QPushButton:hover { background: #8c0ebf9c; }");
    pLayout->addWidget(m_pButton);

    QGridLayout *pFactoryLayout = (QGridLayout*)layout();
    pFactoryLayout->addWidget(pWidget, 1, 1, 1, 1, Qt::AlignLeft);

    QObject::connect(m_pTrigger, SIGNAL(clicked(bool)), this, SLOT(clickTrigger(bool)));
    QObject::connect(m_pButton, SIGNAL(clicked()), this, SLOT(clickButton()));
}

BBEnumExpansionFactory::~BBEnumExpansionFactory()
{
    BB_SAFE_DELETE(m_pTrigger);
    BB_SAFE_DELETE(m_pButton);
}

void BBEnumExpansionFactory::enableTrigger(bool bEnable)
{
    m_pTrigger->setVisible(bEnable);
}

void BBEnumExpansionFactory::enableButton(bool bEnable)
{
    m_pButton->setVisible(bEnable);
}

void BBEnumExpansionFactory::clickButton()
{
    emit buttonClicked();
}

void BBEnumExpansionFactory::clickTrigger(bool bClicked)
{
    emit triggerClicked(bClicked);
}


/**
 * @brief BBColorFactory::BBColorFactory
 * @param r
 * @param g
 * @param b
 * @param a
 * @param pParent
 */
BBColorFactory::BBColorFactory(float r, float g, float b, float a, QWidget *pParent)
    : BBColorFactory(new float[4] {r, g, b, a}, pParent)
{

}

BBColorFactory::BBColorFactory(const std::string &uniformName, float r, float g, float b, float a, QWidget *pParent)
    : BBColorFactory(r, g, b, a, pParent)
{
    m_UniformName = uniformName;
}

BBColorFactory::BBColorFactory(float *color, QWidget *pParent)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    QPushButton *pDropper = new QPushButton(this);
    pDropper->setStyleSheet("image: url(../../resources/icons/eyedropper.png);");
    pLayout->addWidget(pDropper, 0);
    m_pColorButton = new BBColorButton(this);
    pLayout->addWidget(m_pColorButton, 1);

    QObject::connect(pDropper, SIGNAL(clicked()), this, SLOT(catchColor()));

    // default
    m_pColorButton->setColor(color);
}

BBColorFactory::~BBColorFactory()
{
    BB_SAFE_DELETE(m_pColorButton);
}

void BBColorFactory::catchColor()
{
    // Create a dialog box that is the same as the screen, select the color on it
    BBScreenDialog dialog;
    // After the selection is completed, the color button is set to the selected color
    QObject::connect(&dialog, SIGNAL(setColor(float, float, float)),
                     this, SLOT(finishCatchColor(float, float, float)));

    dialog.exec();
}

void BBColorFactory::finishCatchColor(float r, float g, float b)
{
    m_pColorButton->setColor(r * 255, g * 255, b * 255);
    emit colorChanged(r, g, b, 1.0f, m_UniformName);
}


/**
 * @brief BBLightColorFactory::BBLightColorFactory
 * @param pLight
 * @param pParent
 */
BBLightColorFactory::BBLightColorFactory(BBLight *pLight, QWidget *pParent)
    : BBColorFactory(pLight->getDiffuseColor(), pParent)
{
    m_pLight = pLight;
}

void BBLightColorFactory::finishCatchColor(float r, float g, float b)
{
    BBColorFactory::finishCatchColor(r, g, b);
    m_pLight->setDiffuseColor(r, g, b);
}


/**
 * @brief BBIconFactory::BBIconFactory
 * @param pParent
 */
BBIconFactory::BBIconFactory(QWidget *pParent)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);

    QWidget *pLeft = new QWidget(this);
    m_pLeftLayout = new QGridLayout(pLeft);
    m_pLeftLayout->setMargin(0);

    QWidget *pBottomLeft = new QWidget(pLeft);
    QHBoxLayout *pBottomLeftLayout = new QHBoxLayout(pBottomLeft);
    pBottomLeftLayout->setMargin(0);
    // Button
    m_pRemoveButton = new QPushButton(pBottomLeft);
    m_pRemoveButton->setStyleSheet("image: url(../../resources/icons/return.png);");
    pBottomLeftLayout->addWidget(m_pRemoveButton, 0);
    m_pSelectButton = new QPushButton(pBottomLeft);
    m_pSelectButton->setStyleSheet("image: url(../../resources/icons/more2.png);");
    pBottomLeftLayout->addWidget(m_pSelectButton, 0);
    // name
    m_pNameEdit = new QLineEdit(pBottomLeft);
    m_pNameEdit->setEnabled(false);
    pBottomLeftLayout->addWidget(m_pNameEdit, 1);
    m_pLeftLayout->addWidget(pBottomLeft, 2, 0, 1, 1, Qt::AlignBottom);

    pLayout->addWidget(pLeft, 1);

    // picture in the right
    QWidget *pRight = new QWidget(this);
    pRight->setStyleSheet("border: none; border-radius: 2px; background: rgb(60, 64, 75);");
    QHBoxLayout *pFrameLayout = new QHBoxLayout(pRight);
    pFrameLayout->setMargin(1);
    m_pIconLabel = new BBIconLabel(pRight);
    pFrameLayout->addWidget(m_pIconLabel);
    pLayout->addWidget(pRight);

    QObject::connect(m_pIconLabel, SIGNAL(currentFilePathChanged(QString)), this, SLOT(changeCurrentFilePath(QString)));
}

BBIconFactory::~BBIconFactory()
{
    BB_SAFE_DELETE(m_pLeftLayout);
    BB_SAFE_DELETE(m_pRemoveButton);
    BB_SAFE_DELETE(m_pSelectButton);
    BB_SAFE_DELETE(m_pIconLabel);
    BB_SAFE_DELETE(m_pNameEdit);
}

void BBIconFactory::setContent(const QString &filePath)
{
    if (filePath.isEmpty())
    {
        // there is nothing
        m_pIconLabel->setText("None");
        m_pNameEdit->setText("");
    }
    else
    {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists())
        {
            m_pIconLabel->setText("");
            m_pNameEdit->setText(fileInfo.fileName());
        }
        else
        {
            m_pIconLabel->setText("Missing");
            m_pNameEdit->setText("");
        }
    }
    m_pIconLabel->setIcon(filePath);
}


/**
 * @brief BBTextureFactory::BBTextureFactory
 * @param pParent
 */
BBTextureFactory::BBTextureFactory(const QString &uniformName, const QString &originalIconPath, QWidget *pParent, int nIndex)
    : BBIconFactory(pParent)
{
    m_UniformName = uniformName;
    m_nIndex = nIndex;
    m_pIconLabel->setFilter(BBFileSystemDataManager::m_TextureSuffixs);
    setContent(originalIconPath);

    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName("Tiling");
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->addWidget(new QLabel("Tiling"), 1);
    m_pTilingFactory = new BBVector2DFactory(QVector2D(0, 0), this);
    pLayout->addWidget(m_pTilingFactory, 2);
    m_pLeftLayout->addWidget(pWidget, 0, 0, 1, 1, Qt::AlignTop);

    pWidget = new QWidget(this);
    pWidget->setObjectName("Offset");
    pLayout = new QHBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->addWidget(new QLabel("Offset"), 1);
    m_pOffsetFactory = new BBVector2DFactory(QVector2D(0, 0), this);
    pLayout->addWidget(m_pOffsetFactory, 2);
    m_pLeftLayout->addWidget(pWidget, 1, 0, 1, 1, Qt::AlignTop);

    enableTilingAndOffset(false);

    QObject::connect(m_pTilingFactory, SIGNAL(valueChanged(QVector2D)), this, SLOT(changeTiling(QVector2D)));
    QObject::connect(m_pOffsetFactory, SIGNAL(valueChanged(QVector2D)), this, SLOT(changeOffset(QVector2D)));
}

void BBTextureFactory::enableTilingAndOffset(bool bEnable)
{
    findChild<QWidget*>("Tiling")->setVisible(bEnable);
    findChild<QWidget*>("Offset")->setVisible(bEnable);
}

void BBTextureFactory::setTiling(float fTilingX, float fTilingY)
{
    m_pTilingFactory->setValue(QVector2D(fTilingX, fTilingY));
}

void BBTextureFactory::setOffset(float fOffsetX, float fOffsetY)
{
    m_pOffsetFactory->setValue(QVector2D(fOffsetX, fOffsetY));
}

void BBTextureFactory::changeCurrentFilePath(const QString &filePath)
{
    setContent(filePath);
    emit setSampler2D(m_UniformName, filePath, m_nIndex);
}

void BBTextureFactory::changeTiling(const QVector2D &value)
{
    emit setTilingAndOffset(LOCATION_TILINGANDOFFSET_PREFIX + m_UniformName, value.x(), value.y(), m_pOffsetFactory->getX(), m_pOffsetFactory->getY());
}

void BBTextureFactory::changeOffset(const QVector2D &value)
{
    emit setTilingAndOffset(LOCATION_TILINGANDOFFSET_PREFIX + m_UniformName, m_pTilingFactory->getX(), m_pTilingFactory->getY(), value.x(), value.y());
}


/**
 * @brief BBCubeMapFactory::BBCubeMapFactory
 * @param uniformName
 * @param originalIconPath
 * @param pParent
 */
BBCubeMapFactory::BBCubeMapFactory(const QString &uniformName, const QString originalIconPath[], QWidget *pParent)
    : QWidget(pParent)
{
    for (int i = 0; i < 6; i++)
    {
        m_ResourcePaths[i] = originalIconPath[i];
    }

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);

    QString subName[6] = {"_Positive_X", "_Negative_X", "_Positive_Y", "_Negative_Y", "_Positive_Z", "_Negative_Z"};
    for (int i = 0; i < 6; i++)
    {
        QWidget *pTextureFactoryWidget = new QWidget(this);
        QHBoxLayout *pTextureFactoryLayout = new QHBoxLayout(pTextureFactoryWidget);
        pTextureFactoryLayout->setMargin(0);
        pTextureFactoryLayout->addWidget(new QLabel(uniformName + subName[i]), 1, Qt::AlignTop);
        m_pTextureFactory[i] = new BBTextureFactory(uniformName, originalIconPath[i], this, i);
        pTextureFactoryLayout->addWidget(m_pTextureFactory[i], 1);
        pLayout->addWidget(pTextureFactoryWidget);

        QObject::connect(m_pTextureFactory[i], SIGNAL(setSampler2D(QString, QString, int)),
                         this, SLOT(changeCurrentFilePath(QString, QString, int)));
    }
}

BBCubeMapFactory::~BBCubeMapFactory()
{
    for (int i = 0; i < 6; i++)
    {
        BB_SAFE_DELETE(m_pTextureFactory[i]);
    }
}

void BBCubeMapFactory::changeCurrentFilePath(const QString &uniformName, const QString &texturePath, int nFaceIndex)
{
    m_pTextureFactory[nFaceIndex]->setContent(texturePath);
    m_ResourcePaths[nFaceIndex] = texturePath;
    emit setSamplerCube(uniformName, m_ResourcePaths);
}


/**
 * @brief BBDragAcceptedFactory::BBDragAcceptedFactory
 * @param pParent
 */
BBDragAcceptedFactory::BBDragAcceptedFactory(const QString &iconPath, const QString &filePath, QWidget *pParent)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);

    m_pIconLabel = new QPushButton(this);
    m_pIconLabel->setStyleSheet("image: url(" + iconPath + ");");
    pLayout->addWidget(m_pIconLabel);
    m_pDragAcceptedEdit = new BBDragAcceptedEdit(this);
    pLayout->addWidget(m_pDragAcceptedEdit, 1);
    changeCurrentFilePath(filePath);

    QObject::connect(m_pIconLabel, SIGNAL(clicked()), this, SLOT(clickIcon()));
    QObject::connect(m_pDragAcceptedEdit, SIGNAL(currentFilePathChanged(QString)),
                     this, SLOT(changeCurrentFilePath(QString)));
}

BBDragAcceptedFactory::~BBDragAcceptedFactory()
{
    BB_SAFE_DELETE(m_pIconLabel);
    BB_SAFE_DELETE(m_pDragAcceptedEdit);
}

void BBDragAcceptedFactory::setFilter(const QStringList &acceptableSuffixs)
{
    m_pDragAcceptedEdit->setFilter(acceptableSuffixs);
}

void BBDragAcceptedFactory::changeCurrentFilePath(const QString &filePath)
{
    if (filePath.isEmpty())
    {
        m_pDragAcceptedEdit->setText("None");
    }
    else
    {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists())
        {
            m_pDragAcceptedEdit->setText(fileInfo.baseName());
            emit currentFilePathChanged(filePath);
        }
        else
        {
            m_pDragAcceptedEdit->setText("Missing");
        }
    }
}

void BBDragAcceptedFactory::clickIcon()
{
    emit iconClicked();
}


/**
 * @brief BBSliderFactory::BBSliderFactory
 * @param nValue
 * @param nMin
 * @param nMax
 * @param pParent
 */
BBSliderFactory::BBSliderFactory(int nValue, int nMin, int nMax, QWidget *pParent)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    m_pSlider = new QSlider(Qt::Horizontal, this);
    pLayout->addWidget(m_pSlider);
    m_pEditor = new QLineEdit(this);
    m_pEditor->setMaximumWidth(50);
    QRegExp re("[0-9]+$");
    QValidator *pValidator = new QRegExpValidator(re, m_pEditor);
    m_pEditor->setValidator(pValidator);
    pLayout->addWidget(m_pEditor);

    setRange(nMin, nMax);
    setValue(nValue);

    QObject::connect(m_pSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue(int)));
    QObject::connect(m_pEditor, SIGNAL(textChanged(QString)), this, SLOT(changeEditorValue(QString)));
}

BBSliderFactory::~BBSliderFactory()
{
    BB_SAFE_DELETE(m_pSlider);
    BB_SAFE_DELETE(m_pEditor);
}

void BBSliderFactory::setRange(int nMin, int nMax)
{
    m_nMin = nMin;
    m_nMax = nMax;
    m_pSlider->setRange(nMin, nMax);
}

void BBSliderFactory::setValue(int value)
{
    m_pSlider->setValue(value);
    m_pEditor->setText(QString::number(value));
}

void BBSliderFactory::changeSliderValue(int value)
{
    m_pEditor->setText(QString::number(value));
    valueChanged(value);
}

void BBSliderFactory::changeEditorValue(const QString &value)
{
    int nValue = value.toInt();
    m_pSlider->setValue(nValue);
    if (!value.isEmpty())
    {
        if (nValue > m_nMax)
            nValue = m_nMax;
        if (nValue < m_nMin)
            nValue = m_nMin;
        m_pEditor->setText(QString::number(nValue));
    }
}

