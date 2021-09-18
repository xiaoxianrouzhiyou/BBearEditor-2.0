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
    void showGlobalSettingsProperty();
    void showOfflineRendererProperty();
    void switchGameObjectPage(int nIndex);
    void useToolFBX2BBear();

signals:
    void pressMultipleSelectionKey(bool);

private:
    void setWindowLayout();
    void setMenu();
    void setGameObjectDockWidget();
    void setPreview();
    void setConnect();

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    Ui::BBMainWindow *m_pUi;
};

#endif // BBUIMAINWINDOW_H
