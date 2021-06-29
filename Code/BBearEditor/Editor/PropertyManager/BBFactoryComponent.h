#ifndef BBFACTORYCOMPONENT_H
#define BBFACTORYCOMPONENT_H

#include <QPushButton>
#include <QDialog>
#include <QLabel>


// When hovering, it shows an arrow cursor, press and then move left and right to adjust the parameter
class BBSliderLabel : public QPushButton
{
    Q_OBJECT

public:
    explicit BBSliderLabel(QWidget *pParent = nullptr);

private:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    QPoint m_LastPos;
    bool m_bPressed;

signals:
    void slide(int nDeltaX);
};


class BBColorButton : public QPushButton
{
    Q_OBJECT

public:
    explicit BBColorButton(QWidget *pParent = nullptr);
    ~BBColorButton();

    void setColor(int r, int g, int b, int a = 255);
    void setColor(float *color);

private slots:
    void click();

private:
    QWidget *m_pBlackBackGroundContent;
    QWidget *m_pWhiteBackGroundContent;
};


/**
 * @brief The BBScreenDialog class              Capture the full-screen for color picking of dropper
 */
class BBScreenDialog : public QDialog
{
    Q_OBJECT

public:
    BBScreenDialog(QWidget *pParent = 0);
    ~BBScreenDialog();

signals:
    void setColor(float r, float g, float b);

private:
    void setBackground();
    void mousePressEvent(QMouseEvent *event) override;

    QLabel *m_pBackground;
    QPixmap m_PixBackground;
    static int m_nCursorSize;
};


/**
 * @brief The BBIconLabel class
 */
class BBIconLabel : public QLabel
{
    Q_OBJECT

public:
    BBIconLabel(QWidget *pParent = 0);

    void setFilter(const QStringList &acceptableSuffixs) { m_Filter = acceptableSuffixs; }
    void setScaledPixmap(const QPixmap &pixmap);

signals:
    void currentFilePathChanged(const QString &path);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    QStringList m_Filter;
    QString m_CurrentFilePath;
    static QSize m_DefaultSize;
    static QSize m_ContentDefaultSize;
};


#endif // BBFACTORYCOMPONENT_H
