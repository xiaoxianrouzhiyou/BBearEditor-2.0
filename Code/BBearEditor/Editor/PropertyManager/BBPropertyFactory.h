#ifndef BBPROPERTYFACTORY_H
#define BBPROPERTYFACTORY_H

#include <QWidget>
#include <QVector2D>
#include <QVector3D>


class BBSliderLabel;
class QLineEdit;
class QLabel;
class QComboBox;
class BBColorButton;
class BBLight;
class QPushButton;
class BBIconLabel;
class BBDragAcceptedEdit;
class BBRenderableObject;
class QSlider;
class QGridLayout;
class QVBoxLayout;
class QCheckBox;


/**
 * @brief The BBLineEditFactory class       QLineEdit with the BBSliderLabel of name on the left
 */
class BBLineEditFactory : public QWidget
{
    Q_OBJECT

public:
    explicit BBLineEditFactory(const QString &name, float fValue, QWidget *pParent = nullptr,
                               int nNameStretch = 0, int nValueStretch = 0);
    ~BBLineEditFactory();

    void setRange(float fMin, float fMax);
    void setRegExp(const QString &pattern);
    void setSlideStep(float fStep) { m_fSlideStep = fStep; }
    QString setValue(float fValue);

private slots:
    void changeEditText(const QString &text);
    void changeValueBySlider(int nDeltaX);
    void showFromLeft();

signals:
    void valueChanged(float fValue);
    void valueChanged(const QString &name, float fValue);

private:
    BBSliderLabel *m_pSliderLabel;
    QLineEdit *m_pEdit;
    float m_fMaxValue;
    float m_fMinValue;
    float m_fSlideStep;
};


class BBVector2DFactory : public QWidget
{
    Q_OBJECT

public:
    BBVector2DFactory(const QVector2D &value = QVector2D(0, 0), QWidget *pParent = nullptr);
    ~BBVector2DFactory();

    void setValue(const QVector2D &value);

    float getX() { return m_Value.x(); }
    float getY() { return m_Value.y(); }

public slots:
    void setX(float x);
    void setY(float y);

signals:
    void valueChanged(const QVector2D &value);

private:
    BBLineEditFactory *m_pEditX;
    BBLineEditFactory *m_pEditY;
    QVector2D m_Value;
};


/**
 * @brief The BBVector3DFactory class       Composed of three BBLineEditFactory
 */
class BBVector3DFactory : public QWidget
{
    Q_OBJECT

public:
    BBVector3DFactory(const QVector3D &value = QVector3D(0, 0, 0), QWidget *pParent = nullptr);
    ~BBVector3DFactory();

    void setValue(const QVector3D &value);

public slots:
    void setX(float x);
    void setY(float y);
    void setZ(float z);

signals:
    void valueChanged(const QVector3D &value);

private:
    BBLineEditFactory *m_pEditX;
    BBLineEditFactory *m_pEditY;
    BBLineEditFactory *m_pEditZ;
    QVector3D m_Value;
};


/**
 * @brief The BBEnumFactory class           name showed in the left side and enum box showed in the right side
 */
class BBEnumFactory : public QWidget
{
    Q_OBJECT

public:
    BBEnumFactory(const QString &name, const QStringList &comboBoxItems,
                  const QString &currentText = "", QWidget *pParent = 0, int labelStretch = 1, int comboBoxStretch = 3);
    ~BBEnumFactory();

    int getCurrentItemIndex();

private slots:
    void changeCurrentItem(int nIndex);
    void changeCurrentItem(const QString &text);

signals:
    void currentItemChanged(int nIndex);
    void currentItemChanged(const QString &text);

private:
    QLabel *m_pLabel;
    QComboBox *m_pComboBox;
};


/**
 * @brief The BBEnumExpansionFactory class              BBEnumFactory and button ...
 */
class BBEnumExpansionFactory : public BBEnumFactory
{
    Q_OBJECT

public:
    BBEnumExpansionFactory(const QString &name, const QStringList &comboBoxItems, const QString &buttonText,
                           const QString &currentText = "", QWidget *pParent = 0, int labelStretch = 0, int comboBoxStretch = 1);

    ~BBEnumExpansionFactory();

    void enableTrigger(bool bEnable);
    void enableButton(bool bEnable);

private slots:
    void clickButton();
    void clickTrigger(bool bClicked);

signals:
    void buttonClicked();
    void triggerClicked(bool bClicked);

private:
    QCheckBox *m_pTrigger;
    QPushButton *m_pButton;
};


/**
 * @brief The BBColorFactory class          dropper and color box
 */
class BBColorFactory : public QWidget
{
    Q_OBJECT

public:
    BBColorFactory(float r, float g, float b, float a, QWidget *pParent = 0);
    BBColorFactory(const std::string &uniformName, float r, float g, float b, float a, QWidget *pParent = 0);
    BBColorFactory(float *color, QWidget *pParent = 0);
    ~BBColorFactory();

protected slots:
    void catchColor();
    virtual void finishCatchColor(float r, float g, float b);

signals:
    void colorChanged(float r, float g, float b, float a = 1.0f, const std::string &uniformName = "");

protected:
    BBColorButton *m_pColorButton;
    std::string m_UniformName;
};


class BBLightColorFactory : public BBColorFactory
{
    Q_OBJECT

public:
    BBLightColorFactory(BBLight *pLight, QWidget *pParent = 0);

private slots:
    void finishCatchColor(float r, float g, float b) override;

private:
    BBLight *m_pLight;
};


class BBIconFactory : public QWidget
{
    Q_OBJECT

public:
    BBIconFactory(QWidget *pParent = 0);
    ~BBIconFactory();

    void setContent(const QString &filePath);

public slots:
    virtual void changeCurrentFilePath(const QString &filePath) = 0;

protected:
    // Widget except icon
    QGridLayout *m_pLeftLayout;
    QPushButton *m_pRemoveButton;
    QPushButton *m_pSelectButton;
    BBIconLabel *m_pIconLabel;
    QLineEdit *m_pNameEdit;
};


class BBTextureFactory : public BBIconFactory
{
    Q_OBJECT

public:
    BBTextureFactory(const QString &uniformName, const QString &originalIconPath = "", QWidget *pParent = 0, int nIndex = 0);

    void enableTilingAndOffset(bool bEnable);
    void setTiling(float fTilingX, float fTilingY);
    void setOffset(float fOffsetX, float fOffsetY);

public slots:
    void changeCurrentFilePath(const QString &filePath) override;
    void changeTiling(const QVector2D &value);
    void changeOffset(const QVector2D &value);

signals:
    void setSampler2D(const QString &uniformName, const QString &texturePath, int nIndex = 0);
    void setTilingAndOffset(const QString &uniformName, float fTilingX, float fTilingY, float fOffsetX, float fOffsetY);

private:
    BBVector2DFactory *m_pTilingFactory;
    BBVector2DFactory *m_pOffsetFactory;

    QString m_UniformName;
    // if it is in a group
    int m_nIndex;
};


class BBCubeMapFactory : public QWidget
{
    Q_OBJECT

public:
    BBCubeMapFactory(const QString &uniformName, const QString originalIconPath[] = {}, QWidget *pParent = 0);
    ~BBCubeMapFactory();

public slots:
    void changeCurrentFilePath(const QString &uniformName, const QString &texturePath, int nFaceIndex);

signals:
    void setSamplerCube(const QString &uniformName, QString *pResourcePaths);

private:
    BBTextureFactory *m_pTextureFactory[6];
    QString m_ResourcePaths[6];
};


class BBDragAcceptedFactory : public QWidget
{
    Q_OBJECT

public:
    BBDragAcceptedFactory(const QString &iconPath, const QString &filePath = "", QWidget *pParent = 0);
    ~BBDragAcceptedFactory();

    void setFilter(const QStringList &acceptableSuffixs);

public slots:
    void changeCurrentFilePath(const QString &filePath);
    void clickIcon();

signals:
    void currentFilePathChanged(const QString &filePath);
    void iconClicked();

private:
    QPushButton *m_pIconLabel;
    BBDragAcceptedEdit *m_pDragAcceptedEdit;
};


/**
 * @brief The BBSliderFactory class         slider in the left and editor in the right
 */
class BBSliderFactory : public QWidget
{
    Q_OBJECT

public:
    BBSliderFactory(int nValue = 50, int nMin = 0, int nMax = 100, QWidget *pParent = 0);
    ~BBSliderFactory();

    void setRange(int nMin, int nMax);
    void setValue(int value);

private slots:
    void changeSliderValue(int value);
    void changeEditorValue(const QString &value);

signals:
    void valueChanged(int value);

private:
    int m_nMin;
    int m_nMax;
    QSlider *m_pSlider;
    QLineEdit *m_pEditor;
};


//class BBSliderFFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    BBSliderFFactory(float fValue = 0.5f, float fMin = 0.0f, float fMax = 1.0f, QWidget *pParent = 0);
//    ~BBSliderFFactory();

//    void setRange(float fMin, float fMax);
//    void setValue(float value);

//private slots:
//    void changeSliderValue(float value);
//    void changeEditorValue(const QString &value);

//signals:
//    void valueChanged(float value);

//private:
//    float m_fMin;
//    float m_fMax;
//    QSlider *m_pSlider;
//    QLineEdit *m_pEditor;
//};

#endif // BBPROPERTYFACTORY_H



