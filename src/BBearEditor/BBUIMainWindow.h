#ifndef BBUIMAINWINDOW_H
#define BBUIMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class BBUIMainWindow;
}

class BBUIMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BBUIMainWindow(QWidget *parent = 0);
    ~BBUIMainWindow();

private:
    void setGameObjectDockWidget();
    Ui::BBUIMainWindow *m_pUi;
};

#endif // BBUIMAINWINDOW_H
