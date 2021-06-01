#ifndef BBFILESYSTEMDOCKWIDGET_H
#define BBFILESYSTEMDOCKWIDGET_H

#include <QDockWidget>


class QTreeWidgetItem;
class BBOpenGLWidget;
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

private slots:
    void clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem);
    void doubleClickItemInFileList(const QString &filePath);
    void clickItemInFolderPathBar(const QString &filePath);

    void newFolder(const QString &parentPath);

private:
    void setConnect();

    void updateFolderTree();
    void updateFolderTree(const QString &filePath);
    void updateFileList(const QString &filePath, QTreeWidgetItem *pItem);
    void updateFileList(const QString &filePath);
    void updateFolderPathBar(const QString &filePath);

    Ui::BBFileSystemDockWidget *m_pUi;
    BBOpenGLWidget *m_pPreviewOpenGLWidget;
    BBFileSystemData *m_pData; 
};

#endif // BBFILESYSTEMDOCKWIDGET_H
