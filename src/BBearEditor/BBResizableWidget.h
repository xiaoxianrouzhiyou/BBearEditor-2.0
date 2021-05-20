#ifndef BBRESIZABLEWIDGET_H
#define BBRESIZABLEWIDGET_H

#include <QWidget>

class BBResizableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BBResizableWidget(QWidget *parent = nullptr);
    QSize sizeHint() const;
    void updateSizeHint(const QSize &size);

private:
    QSize m_Size;
};

#endif // BBRESIZABLEWIDGET_H
