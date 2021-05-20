#include "BBResizableWidget.h"

BBResizableWidget::BBResizableWidget(QWidget *parent)
    : QWidget(parent)
{
    m_Size = geometry().size();
}

QSize BBResizableWidget::sizeHint() const
{
    return m_Size;
}

void BBResizableWidget::updateSizeHint(const QSize &size)
{
    m_Size = size;
    updateGeometry();
}
