#ifndef BBFILESYSTEMMANAGER_H
#define BBFILESYSTEMMANAGER_H


#include <QString>
#include "BBUtils.h"


using namespace BBFileSystem;


class BBOpenGLWidget;
class BBFolderTreeWidget;
class BBFileListWidget;
class BBFilePathBarWidget;
class BBFileSystemDataManager;


class BBFileSystemManager
{
public:
    BBFileSystemManager(BBFolderTreeWidget *pFolderTreeWidget,
                        BBFileListWidget *pFileListWidget,
                        BBFilePathBarWidget *pFilePathBarWidget);
    ~BBFileSystemManager();

    void createProject();
    void openProject();

    void clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem);
    void doubleClickItemInFileList(const QString &filePath);
    void clickItemInFolderPathBar(const QString &filePath);

    void newFolder(const QString &parentPath, const BBSignalSender &eSender);

    void showInFolder(const QString &filePath);

    void renameInFolderTree(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath);
    void renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath);

    void deleteFolderInFolderTree(QTreeWidgetItem *pItem);
    void deleteFilesInFileList(const QList<QListWidgetItem*> &items);

    void importAsset(const QString &parentPath, const QList<QUrl> &urls);

    void moveFiles(QList<QListWidgetItem*> items, const QString &oldParentPath,
                   const QString &newParentPath, bool bCopy);

    void updateAll();

private:
    void updateFolderTree();
    void updateFolderTree(const QString &filePath);
    void updateFileList(const QString &parentPath, QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pCurrentItem);
    void updateFileList(const QString &parentPath, QListWidgetItem *pCurrentItem);
    void updateFolderPathBar(const QString &filePath);

    void rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                const QString &oldPath, const QString &newPath);

    BBOpenGLWidget *m_pPreviewOpenGLWidget;
    BBFolderTreeWidget *m_pFolderTreeWidget;
    BBFileListWidget *m_pFileListWidget;
    BBFilePathBarWidget *m_pFilePathBarWidget;
    BBFileSystemDataManager *m_pDataManager;
};

#endif // BBFILESYSTEMMANAGER_H
