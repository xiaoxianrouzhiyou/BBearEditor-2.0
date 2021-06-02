#include "BBFileSystemDockWidget.h"
#include "ui_BBFileSystemDockWidget.h"
#include "BBUtils.h"
#include "BBFileSystemData.h"
#include "BBOpenGLWidget.h"


BBFileSystemDockWidget::BBFileSystemDockWidget(QWidget *pParent)
    : QDockWidget(pParent),
      m_pUi(new Ui::BBFileSystemDockWidget)
{
    m_pUi->setupUi(this);

    m_pPreviewOpenGLWidget = new BBOpenGLWidget;
    m_pPreviewOpenGLWidget->resize(256, 256);
    // Cannot be used without a context shared with the toplevel.
    m_pPreviewOpenGLWidget->show();
    m_pPreviewOpenGLWidget->hide();

    m_pData = new BBFileSystemData();
    m_pData->bindPreviewOpenGLWidget(m_pPreviewOpenGLWidget);

    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));

    // Stretch the width of the file list
    m_pUi->splitterProject->setStretchFactor(0, 1);
    m_pUi->splitterProject->setStretchFactor(1, 2);

    setConnect();
}

BBFileSystemDockWidget::~BBFileSystemDockWidget()
{
    BB_SAFE_DELETE(m_pUi);
    BB_SAFE_DELETE(m_pPreviewOpenGLWidget);
    BB_SAFE_DELETE(m_pData);
}

void BBFileSystemDockWidget::createProject()
{
//    //新项目预创建直射光
//    GameObject *object = ui->openGLWidget->scene.createLight("directional light.png", 650, 300);
//    //结点加入层级视图
//    ui->treeHierarchy->addGameObjectSlot(object);
//    //取消直射光的选中状态
//    ui->treeHierarchy->cancelSelectedItems();
}

void BBFileSystemDockWidget::openProject()
{
    // load file system
    m_pData->load();
    updateFolderTree();
    updateFileList(BBConstant::BB_PATH_PROJECT_USER);
    updateFolderPathBar(BBConstant::BB_PATH_PROJECT_USER);
}

void BBFileSystemDockWidget::clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem)
{
    updateFileList(filePath, pItem);
    updateFolderPathBar(filePath);
}

void BBFileSystemDockWidget::doubleClickItemInFileList(const QString &filePath)
{
    if (!m_pData->openFile(filePath))
    {
        // when this is a folder, need to update and access new folder
        updateFolderTree(filePath);
        updateFileList(filePath);
        updateFolderPathBar(filePath);
    }
}

void BBFileSystemDockWidget::clickItemInFolderPathBar(const QString &filePath)
{
    updateFolderTree(filePath);
    updateFileList(filePath);
    updateFolderPathBar(filePath);
}

void BBFileSystemDockWidget::newFolder(const QString &parentPath, const BBSignalSender &eSender)
{
    QTreeWidgetItem *pFolderItem = NULL;
    QListWidgetItem *pFileItem = NULL;
    if (m_pData->newFolder(parentPath, pFolderItem, pFileItem))
    {
        updateFolderTree();
        // if list is showing parentPath
        if (m_pUi->listFile->getCurrentParentPath() == parentPath)
        {
            updateFileList(parentPath, pFileItem);
        }
        if (eSender == BBSignalSender::FolderTree)
        {
            m_pUi->treeFolder->setCurrentItem(pFolderItem);
        }
    }
}

void BBFileSystemDockWidget::showInFolder(const QString &filePath)
{
    m_pData->showInFolder(filePath);
}

void BBFileSystemDockWidget::renameInFolderTree(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath)
{
    QListWidgetItem *pFileItem = m_pData->getFileItem(pParentFolderItem, oldPath);
    BB_PROCESS_ERROR_RETURN(pFileItem);
    rename(pParentFolderItem, pFileItem, oldPath, newPath);
}

void BBFileSystemDockWidget::renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath)
{
    QTreeWidgetItem *pParentFolderItem = m_pData->getParentFolderItem(oldPath);
    rename(pParentFolderItem, pFileItem, oldPath, newPath);
}

void BBFileSystemDockWidget::rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                                    const QString &oldPath, const QString &newPath)
{
    if (m_pData->rename(pParentFolderItem, pFileItem, oldPath, newPath))
    {
        // update
        updateFolderTree(); 
        // cannot use m_pUi->listFile->getCurrentParentPath(), which is an old path
        QString parentPath = BBFileSystemData::getAbsolutePath(m_pUi->listFile->getCurrentParentItem());
        updateFileList(parentPath, pFileItem);
        updateFolderPathBar(parentPath);
//        //重新显示属性栏的属性 名字更新
//        itemClicked(editingItem);
    }
}

void BBFileSystemDockWidget::setConnect()
{
    // update selected folder
    QObject::connect(m_pUi->treeFolder, SIGNAL(accessFolder(QString, QTreeWidgetItem*)),
                     this, SLOT(clickItemInFolderTree(QString, QTreeWidgetItem*)));
    QObject::connect(m_pUi->listFile, SIGNAL(openFile(QString)),
                     this, SLOT(doubleClickItemInFileList(QString)));
    QObject::connect(m_pUi->barFilePath, SIGNAL(accessFolder(QString)),
                     this, SLOT(clickItemInFolderPathBar(QString)));
    // update folder path bar
    QObject::connect(this, SIGNAL(updateFolderPathBar(QString)),
                     m_pUi->barFilePath, SLOT(showFolderPath(QString)));
    // click buttons in the file system
    QObject::connect(m_pUi->buttonRootProject, SIGNAL(clicked()),
                     m_pUi->treeFolder, SLOT(pressRootButton()));
    QObject::connect(m_pUi->buttonMoreProject, SIGNAL(clicked()),
                     m_pUi->treeFolder, SLOT(pressSettingButton()));
    // scroll folder path bar
    QObject::connect(m_pUi->buttonMovePathLeft, SIGNAL(pressed()),
                     m_pUi->scrollAreaFilePath, SLOT(moveToLeft()));
    QObject::connect(m_pUi->buttonMovePathRight, SIGNAL(pressed()),
                     m_pUi->scrollAreaFilePath, SLOT(moveToRight()));
    // new folder
    QObject::connect(m_pUi->treeFolder, SIGNAL(newFolder(QString, BBSignalSender)),
                     this, SLOT(newFolder(QString, BBSignalSender)));
    QObject::connect(m_pUi->listFile, SIGNAL(newFolder(QString, BBSignalSender)),
                     this, SLOT(newFolder(QString, BBSignalSender)));
    // show in folder
    QObject::connect(m_pUi->treeFolder, SIGNAL(showInFolder(QString)),
                     this, SLOT(showInFolder(QString)));
    QObject::connect(m_pUi->listFile, SIGNAL(showInFolder(QString)),
                     this, SLOT(showInFolder(QString)));
    // rename
    QObject::connect(m_pUi->treeFolder, SIGNAL(rename(QTreeWidgetItem*, QString, QString)),
                     this, SLOT(renameInFolderTree(QTreeWidgetItem*, QString, QString)));
    QObject::connect(m_pUi->listFile, SIGNAL(rename(QListWidgetItem*, QString, QString)),
                     this, SLOT(renameInFileList(QListWidgetItem*, QString, QString)));
}

/**
 * @brief BBFileSystemDockWidget::updateFolderTree      load entire tree
 */
void BBFileSystemDockWidget::updateFolderTree()
{
    m_pUi->treeFolder->loadTopLevelItems(m_pData->getFolderTreeWidgetTopLevelItems());
}

/**
 * @brief BBFileSystemDockWidget::updateFolderTree      set current item of the tree
 * @param filePath
 */
void BBFileSystemDockWidget::updateFolderTree(const QString &filePath)
{
    QTreeWidgetItem *pItem = m_pData->getItemByPath(filePath);
    m_pUi->treeFolder->setCurrentShowFolderContentItem(pItem);
}

void BBFileSystemDockWidget::updateFileList(const QString &parentPath,
                                            QTreeWidgetItem *pParentFolderItem,
                                            QListWidgetItem *pCurrentItem)
{
    QList<QListWidgetItem*> items;
    QList<QString> fileNames;
    m_pData->getFileListWidgetItems(pParentFolderItem, items, fileNames);
    m_pUi->listFile->loadItems(parentPath, pParentFolderItem, items, fileNames, pCurrentItem);
}

void BBFileSystemDockWidget::updateFileList(const QString &parentPath, QListWidgetItem *pCurrentItem)
{
    QTreeWidgetItem *pParentFolderItem = m_pData->getItemByPath(parentPath);
    updateFileList(parentPath, pParentFolderItem, pCurrentItem);
}






//    // rename folder name in the file list, and update the name of corresponding item in the folder tree
//    QObject::connect(m_pUi->listFile, SIGNAL(renameItemInFolderTree(QString, QString)),
//                     m_pUi->treeFolder, SLOT(renameItem(QString, QString)));
//    // move item in the file list, and move corresponding item in the folder tree
//    QObject::connect(m_pUi->listFile, SIGNAL(moveItemInFolderTree(QString, QString)),
//                     m_pUi->treeFolder, SLOT(moveItem(QString, QString)));
//    // delete folder in the file list, and delete corresponding item in the folder tree
//    QObject::connect(m_pUi->listFile, SIGNAL(deleteItemInFolderTree(QString)),
//                     m_pUi->treeFolder, SLOT(deleteItem(QString)));
//    // rebuild folder tree
//    QObject::connect(m_pUi->listFile, SIGNAL(updateFolderTree()),
//                     m_pUi->treeFolder, SLOT(loadProject()));
//    // create mesh overview map
//    QObject::connect(m_pUi->listFile, SIGNAL(createMeshOverviewMap(QString, QString)),
//                     this, SLOT(createMeshOverviewMap(QString, QString)));
