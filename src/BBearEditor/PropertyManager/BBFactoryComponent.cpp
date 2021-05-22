#include "BBFactoryComponent.h"
#include "BBUtils.h"
#include <QPixmap>
#include <QMouseEvent>


BBSliderLabel::BBSliderLabel(QWidget *pParent)
    : QPushButton(pParent)
{
    m_bPressed = false;
    setMouseTracking(true);
}

void BBSliderLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPixmap pix(QString(BB_PATH_RESOURCE_ICON) + "arrows_stretch_horizontal.png");
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setCursor(QCursor(pix));

    if (m_bPressed)
    {
        QPoint currentPos = event->globalPos();
        int nDeltaX = currentPos.x() - m_LastPos.x();
        m_LastPos = currentPos;
        if (nDeltaX != 0)
        {
            slide(nDeltaX);
        }
    }
}

void BBSliderLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_LastPos = event->globalPos();
    }
}

void BBSliderLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = false;
        m_LastPos = QPoint(0, 0);
    }
}
