#include "BBPropertyFactory.h"
#include <QHBoxLayout>
#include "Utils/BBUtils.h"
#include "BBFactoryComponent.h"
#include <QLineEdit>
#include <QRegExpValidator>
#include <QListView>
#include <QLabel>
#include <QComboBox>


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
                             const QString &currentText, QWidget *pParent,
                             int labelStretch, int comboBoxStretch)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    m_pLabel = new QLabel(name, this);
    m_pLabel->setFocusPolicy(Qt::NoFocus);
    pLayout->addWidget(m_pLabel, labelStretch);
    m_pComboBox = new QComboBox(this);
    m_pComboBox->addItems(comboBoxItems);
    // With this sentence, qss can take effect
    m_pComboBox->setView(new QListView());
    m_pComboBox->setCurrentText(currentText);
    pLayout->addWidget(m_pComboBox, comboBoxStretch);

    QObject::connect(m_pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrentItem(int)));
}

BBEnumFactory::~BBEnumFactory()
{
    BB_SAFE_DELETE(m_pLabel);
    BB_SAFE_DELETE(m_pComboBox);
}

void BBEnumFactory::changeCurrentItem(int nIndex)
{
    emit currentItemChanged(nIndex);
}


