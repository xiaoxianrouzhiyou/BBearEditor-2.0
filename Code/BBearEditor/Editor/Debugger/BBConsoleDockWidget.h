#ifndef BBCONSOLEDOCKWIDGET_H
#define BBCONSOLEDOCKWIDGET_H


#include <QDockWidget>


class BBConsoleDockWidget : public QDockWidget
{
public:
    BBConsoleDockWidget(QWidget *pParent = 0);
    ~BBConsoleDockWidget();

private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QString m_CurrentDragFilePath;
};

#endif // BBCONSOLEDOCKWIDGET_H
