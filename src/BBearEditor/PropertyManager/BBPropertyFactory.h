#ifndef BBPROPERTYFACTORY_H
#define BBPROPERTYFACTORY_H

#include <QWidget>
#include <QVector3D>


class BBSliderLabel;
class QLineEdit;


// QLineEdit with the BBSliderLabel of name on the left
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


// Composed of three BBLineEditFactory
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


#endif // BBPROPERTYFACTORY_H



