#include "BBFileSystemDockWidget.h"
#include "ui_BBFileSystemDockWidget.h"
#include "BBUtils.h"
#include "BBFileSystemData.h"


BBFileSystemDockWidget::BBFileSystemDockWidget(QWidget *pParent)
    : QDockWidget(pParent),
      m_pUi(new Ui::BBFileSystemDockWidget)
{
    m_pUi->setupUi(this);

    m_pData = new BBFileSystemData();

    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));

    // Stretch the width of the file list
    m_pUi->splitterProject->setStretchFactor(0, 1);
    m_pUi->splitterProject->setStretchFactor(1, 2);

    setConnect();
}

BBFileSystemDockWidget::~BBFileSystemDockWidget()
{
    BB_SAFE_DELETE(m_pUi);
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
    updateFileList(NULL);
    updateFolderPathBar(BBConstant::BB_PATH_PROJECT_USER);
}

void BBFileSystemDockWidget::accessFolderInFolderTree(QTreeWidgetItem *pItem, const QString &filePath)
{
    updateFileList(pItem);
    updateFolderPathBar(filePath);
}

void BBFileSystemDockWidget::accessFolderInFileList(const QString &filePath)
{
    updateFolderTree(filePath);
    updateFolderPathBar(filePath);
}

void BBFileSystemDockWidget::accessFolderInFolderPathBar(const QString &filePath)
{
    updateFolderTree(filePath);
    updateFileList(filePath);
}

void BBFileSystemDockWidget::setConnect()
{
    // update selected folder
    QObject::connect(m_pUi->treeFolder, SIGNAL(accessFolder(QTreeWidgetItem*, QString)),
                     this, SLOT(accessFolderInFolderTree(QTreeWidgetItem*, QString)));
    QObject::connect(m_pUi->listFile, SIGNAL(accessFolder(QString)),
                     this, SLOT(accessFolderInFileList(QString)));
    QObject::connect(m_pUi->barFilePath, SIGNAL(accessFolder(QString)),
                     this, SLOT(accessFolderInFolderPathBar(QString)));
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
    m_pUi->treeFolder->setCurrentItemByPath(filePath);
}

void BBFileSystemDockWidget::updateFileList(QTreeWidgetItem *pItem)
{
    m_pUi->listFile->loadItems(m_pData->getFileListWidgetItems(pItem));
}

void BBFileSystemDockWidget::updateFileList(const QString &filePath)
{
    QTreeWidgetItem *pItem = m_pUi->treeFolder->getItemByPath(filePath);
    updateFileList(pItem);
}

void BBFileSystemDockWidget::updateFolderPathBar(const QString &filePath)
{
    m_pUi->barFilePath->showFolderPath(filePath);
}





//    // click an item in the file path bar or file list, and change selected item in the folder tree
//    QObject::connect(m_pUi->barFilePath, SIGNAL(showFolderContent(QString)),
//                     m_pUi->treeFolder, SLOT(setCurrentItemByPath(QString)));
//    QObject::connect(m_pUi->listFile, SIGNAL(clickFolderTreeItem(QString)),
//                     m_pUi->treeFolder, SLOT(setCurrentItemByPath(QString)));
//    // new folder in file list, and add corresponding item in folder tree
//    QObject::connect(m_pUi->listFile, SIGNAL(addItemInFolderTree(QString, QString)),
//                     m_pUi->treeFolder, SLOT(addItem(QString, QString)));
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
