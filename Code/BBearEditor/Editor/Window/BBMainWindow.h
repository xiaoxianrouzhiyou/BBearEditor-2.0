#ifndef BBUIMAINWINDOW_H
#define BBUIMAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class BBMainWindow;
}

class BBMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BBMainWindow(QWidget *parent = 0);
    virtual ~BBMainWindow();

private slots:
    void createProject();
    void openProject();

signals:
    void pressMultipleSelectionKey(bool);

private:
    void setWindowLayout();
    void setGameObjectDockWidget();
    void setConnect();

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    Ui::BBMainWindow *m_pUi;
};

#endif // BBUIMAINWINDOW_H
