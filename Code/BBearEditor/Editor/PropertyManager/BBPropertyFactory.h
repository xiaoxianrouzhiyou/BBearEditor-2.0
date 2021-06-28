#ifndef BBPROPERTYFACTORY_H
#define BBPROPERTYFACTORY_H

#include <QWidget>
#include <QVector3D>


class BBSliderLabel;
class QLineEdit;
class QLabel;
class QComboBox;
class BBColorButton;
class BBLight;
class QPushButton;
class BBPictureLabel;


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

private:
    BBSliderLabel *m_pSliderLabel;
    QLineEdit *m_pEdit;
    float m_fMaxValue;
    float m_fMinValue;
    float m_fSlideStep;
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
                  const QString &currentText = "", QWidget *pParent = 0,
                  int labelStretch = 0, int comboBoxStretch = 1);
    ~BBEnumFactory();

private slots:
    void changeCurrentItem(int nIndex);

signals:
    void currentItemChanged(int nIndex);

private:
    QLabel *m_pLabel;
    QComboBox *m_pComboBox;
};


/**
 * @brief The BBColorFactory class          dropper and color box
 */
class BBColorFactory : public QWidget
{
    Q_OBJECT

public:
    BBColorFactory(float *color, QWidget *pParent = 0);
    ~BBColorFactory();

protected slots:
    void catchColor();
    virtual void finishCatchColor(float r, float g, float b);

signals:
    void colorChanged(float r, float g, float b);

protected:
    BBColorButton *m_pColorButton;
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


class BBPictureFactory : public QWidget
{
    Q_OBJECT

public:
    BBPictureFactory(QWidget *pParent = 0);
    ~BBPictureFactory();

protected:
    QPushButton *m_pRemoveButton;
    QPushButton *m_pSelectButton;
    BBPictureLabel *m_pPictureLabel;
};


class BBTextureFactory : public BBPictureFactory
{
    Q_OBJECT

public:
    BBTextureFactory(const QString &uniformName, const QString &originalPicturePath = "", QWidget *pParent = 0);

public slots:
    void changeCurrentFilePath(const QString &filePath);

signals:
    void setSampler2D(const QString &uniformName, const QString &texturePath);

private:
    QString m_UniformName;
};

#endif // BBPROPERTYFACTORY_H



