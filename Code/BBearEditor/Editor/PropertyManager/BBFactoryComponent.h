#ifndef BBFACTORYCOMPONENT_H
#define BBFACTORYCOMPONENT_H

#include <QPushButton>


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

#endif // BBFACTORYCOMPONENT_H
