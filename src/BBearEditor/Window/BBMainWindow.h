#ifndef BBUIMAINWINDOW_H
#define BBUIMAINWINDOW_H

#include <QMainWindow>


class BBOpenGLWidget;
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
    void createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath);

signals:
    void pressMultipleSelectionKey(bool);

private:
    void setWindowLayout();
    void setGameObjectDockWidget();
    void setConnect();
    void initPreviewOpenGLWidget();

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    Ui::BBMainWindow *m_pUi;
    BBOpenGLWidget *m_pPreviewOpenGLWidget;
};

#endif // BBUIMAINWINDOW_H
