#ifndef BBFILESYSTEMDOCKWIDGET_H
#define BBFILESYSTEMDOCKWIDGET_H

#include <QDockWidget>
#include "BBUtils.h"


using namespace BBFileSystem;


class QTreeWidgetItem;
class QListWidgetItem;
class BBFileSystemManager;


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

    void newFolder(const QString &parentPath, const BBSignalSender &eSender);

    void showInFolder(const QString &filePath);

    void renameInFolderTree(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath);
    void renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath);

signals:
    void updateFolderPathBar();
    void updateFolderPathBar(const QString &filePath);

private:
    void setConnect();

    Ui::BBFileSystemDockWidget *m_pUi;
    BBFileSystemManager *m_pFileSystemManager;
};

#endif // BBFILESYSTEMDOCKWIDGET_H
