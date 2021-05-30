#ifndef BBFILESYSTEMDOCKWIDGET_H
#define BBFILESYSTEMDOCKWIDGET_H

#include <QDockWidget>


class BBFileSystemData;


namespace Ui {
class BBFileSystemDockWidget;
}

class BBFileSystemDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit BBFileSystemDockWidget(QWidget *pParent = 0);
    ~BBFileSystemDockWidget();

    void createProject();
    void openProject();

private:
    BBFileSystemData *m_pData;

    Ui::BBFileSystemDockWidget *m_pUi;
};

#endif // BBFILESYSTEMDOCKWIDGET_H
