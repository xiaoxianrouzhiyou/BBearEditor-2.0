#ifndef BBFILESYSTEMDOCKWIDGET_H
#define BBFILESYSTEMDOCKWIDGET_H

#include <QDockWidget>
#include "Utils/BBUtils.h"
#include <QUrl>


using namespace BBFileSystem;


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
    void removeCurrentItem();
    void clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem);
    void clickItemInFileList(const QString &filePath, const BBFileType &eType);
    void doubleClickItemInFileList(const QString &filePath);
    void changeCurrentItemInFileList(BBFileType eCurrentType, BBFileType ePreviousType);
    void clickItemInFolderPathBar(const QString &filePath);

    void newFolder(const QString &parentPath, const BBSignalSender &eSender);
    void newFile(const QString &parentPath, int nType);

    void saveScene();

    void showInFolder(const QString &filePath);

    void renameInFolderTree(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath);
    void renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath);

    void deleteFolderInFolderTree(QTreeWidgetItem *pItem);
    void deleteFilesInFileList(const QList<QListWidgetItem*> &items);

    void importAsset(const QString &parentPath, const QList<QUrl> &urls);

    void moveFolders(const QList<QTreeWidgetItem*> &items, QTreeWidgetItem *pNewParentItem, bool bCopy);
    void moveFolders(const QList<QString> &oldFilePaths, const QString &newParentPath, bool bCopy);
    void moveFiles(const QList<QString> &oldFilePaths, QTreeWidgetItem *pNewParentItem, bool bCopy);
    void moveFiles(const QList<QListWidgetItem*> &items, const QString &oldParentPath,
                   const QString &newParentPath, bool bCopy);

    void updateAll();

signals:
    void updateFolderPathBar();
    void updateFolderPathBar(const QString &filePath);
    void showMaterialPreview(const QString &filePath);
    void showMaterialInPropertyManager(const QString &filePath);
    void removeMaterialPreview();
    void removeCurrentItemInHierarchyTree();

private:
    void setConnect();

    Ui::BBFileSystemDockWidget *m_pUi;
    BBFileSystemManager *m_pFileSystemManager;
};

#endif // BBFILESYSTEMDOCKWIDGET_H
