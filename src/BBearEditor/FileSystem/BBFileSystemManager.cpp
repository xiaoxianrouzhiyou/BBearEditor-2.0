#include "BBFileSystemManager.h"
#include "BBOpenGLWidget.h"
#include "BBFileSystemDataManager.h"
#include "BBFolderTreeWidget.h"
#include "BBFileListWidget.h"
#include "BBFilePathBarWidget.h"


BBFileSystemManager::BBFileSystemManager(BBFolderTreeWidget *pFolderTreeWidget,
                                         BBFileListWidget *pFileListWidget,
                                         BBFilePathBarWidget *pFilePathBarWidget)
{
    m_pPreviewOpenGLWidget = new BBOpenGLWidget;
    m_pPreviewOpenGLWidget->resize(256, 256);
    // Cannot be used without a context shared with the toplevel.
    m_pPreviewOpenGLWidget->show();
    m_pPreviewOpenGLWidget->hide();

    m_pFolderTreeWidget = pFolderTreeWidget;
    m_pFileListWidget = pFileListWidget;
    m_pFilePathBarWidget = pFilePathBarWidget;

    m_pDataManager = new BBFileSystemDataManager();
    m_pDataManager->bindPreviewOpenGLWidget(m_pPreviewOpenGLWidget);
}

BBFileSystemManager::~BBFileSystemManager()
{
    BB_SAFE_DELETE(m_pPreviewOpenGLWidget);
    BB_SAFE_DELETE(m_pDataManager);
}

void BBFileSystemManager::createProject()
{
//    //新项目预创建直射光
//    GameObject *object = ui->openGLWidget->scene.createLight("directional light.png", 650, 300);
//    //结点加入层级视图
//    ui->treeHierarchy->addGameObjectSlot(object);
//    //取消直射光的选中状态
//    ui->treeHierarchy->cancelSelectedItems();
}

void BBFileSystemManager::openProject()
{
    // load file system
    m_pDataManager->load();
    updateFolderTree();
    updateFileList(BBConstant::BB_PATH_PROJECT_USER, NULL, NULL);
    updateFolderPathBar(BBConstant::BB_PATH_PROJECT_USER);
}

void BBFileSystemManager::clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem)
{
    updateFileList(filePath, pItem, NULL);
    updateFolderPathBar(filePath);
}

void BBFileSystemManager::doubleClickItemInFileList(const QString &filePath)
{
    if (!m_pDataManager->openFile(filePath))
    {
        // when this is a folder, need to update and access new folder
        updateFolderTree(filePath);
        updateFileList(filePath, NULL);
        updateFolderPathBar(filePath);
    }
}

void BBFileSystemManager::clickItemInFolderPathBar(const QString &filePath)
{
    updateFolderTree(filePath);
    updateFileList(filePath, NULL);
    updateFolderPathBar(filePath);
}

void BBFileSystemManager::newFolder(const QString &parentPath, const BBSignalSender &eSender)
{
    QTreeWidgetItem *pFolderItem = NULL;
    QListWidgetItem *pFileItem = NULL;
    if (m_pDataManager->newFolder(parentPath, pFolderItem, pFileItem))
    {
        updateFolderTree();
        // if list is showing parentPath
        if (m_pFileListWidget->getCurrentParentPath() == parentPath)
        {
            updateFileList(parentPath, pFileItem);
        }
        if (eSender == BBSignalSender::FolderTree)
        {
            m_pFolderTreeWidget->setCurrentItem(pFolderItem);
        }
    }
}

void BBFileSystemManager::showInFolder(const QString &filePath)
{
    m_pDataManager->showInFolder(filePath);
}

void BBFileSystemManager::renameInFolderTree(QTreeWidgetItem *pParentFolderItem,
                                             const QString &oldPath,
                                             const QString &newPath)
{
    QListWidgetItem *pFileItem = m_pDataManager->getFileItem(pParentFolderItem, oldPath);
    BB_PROCESS_ERROR_RETURN(pFileItem);
    rename(pParentFolderItem, pFileItem, oldPath, newPath);
}

void BBFileSystemManager::renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath)
{
    QTreeWidgetItem *pParentFolderItem = m_pDataManager->getParentFolderItem(oldPath);
    rename(pParentFolderItem, pFileItem, oldPath, newPath);
}

void BBFileSystemManager::deleteFolderInFolderTree(QTreeWidgetItem *pItem)
{
    BB_PROCESS_ERROR_RETURN(m_pDataManager->deleteFolder(pItem));
    m_pFileListWidget->updateCurrentInfos(pItem);

    // no need to update, since this is to handle single item
    // when delete action is over, we will update
}

void BBFileSystemManager::deleteFilesInFileList(const QList<QListWidgetItem*> &items)
{
    QTreeWidgetItem *pParentItem = m_pFileListWidget->getCurrentParentItem();
    QString parentPath = m_pFileListWidget->getCurrentParentPath();
    if (m_pDataManager->deleteFiles(pParentItem, parentPath, items))
    {
        updateFolderTree();
        updateFileList(parentPath, pParentItem, NULL);
    }
}

void BBFileSystemManager::importAsset(const QString &parentPath, const QList<QUrl> &urls)
{
    if (m_pDataManager->importAsset(parentPath, urls))
    {
        updateFolderTree();
        updateFileList(m_pFileListWidget->getCurrentParentPath(), m_pFileListWidget->getCurrentParentItem(), NULL);
    }
}

void BBFileSystemManager::updateAll()
{
    updateFolderTree();
    updateFileList(m_pFileListWidget->getCurrentParentPath(), m_pFileListWidget->currentItem());
    updateFolderPathBar(m_pFileListWidget->getCurrentParentPath());
}

/**
 * @brief BBFileSystemManager::updateFolderTree     load entire tree
 */
void BBFileSystemManager::updateFolderTree()
{
    m_pFolderTreeWidget->loadTopLevelItems(m_pDataManager->getFolderTreeWidgetTopLevelItems());
}

/**
 * @brief BBFileSystemManager::updateFolderTree     set current item of the tree
 * @param filePath
 */
void BBFileSystemManager::updateFolderTree(const QString &filePath)
{
    QTreeWidgetItem *pItem = m_pDataManager->getItemByPath(filePath);
    m_pFolderTreeWidget->setCurrentShowFolderContentItem(pItem);
}

void BBFileSystemManager::updateFileList(const QString &parentPath,
                                         QTreeWidgetItem *pParentFolderItem,
                                         QListWidgetItem *pCurrentItem)
{
    BBFILE *pFolderContent = NULL;
    m_pDataManager->getFileListWidgetItems(pParentFolderItem, pFolderContent);
    m_pFileListWidget->loadItems(parentPath, pParentFolderItem, pFolderContent, pCurrentItem);
}
void BBFileSystemManager::updateFileList(const QString &parentPath, QListWidgetItem *pCurrentItem)
{
    QTreeWidgetItem *pParentFolderItem = m_pDataManager->getItemByPath(parentPath);
    updateFileList(parentPath, pParentFolderItem, pCurrentItem);
}

void BBFileSystemManager::updateFolderPathBar(const QString &filePath)
{
    m_pFilePathBarWidget->showFolderPath(filePath);
}

void BBFileSystemManager::rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                                 const QString &oldPath, const QString &newPath)
{
    if (m_pDataManager->rename(pParentFolderItem, pFileItem, oldPath, newPath))
    {
        // update
        updateFolderTree();
        // cannot use m_pFileListWidget->getCurrentParentPath(), which is an old path
        QString parentPath = BBFileSystemDataManager::getAbsolutePath(m_pFileListWidget->getCurrentParentItem());
        updateFileList(parentPath, pFileItem);
        updateFolderPathBar(parentPath);
//        //重新显示属性栏的属性 名字更新
//        itemClicked(editingItem);
    }
}
