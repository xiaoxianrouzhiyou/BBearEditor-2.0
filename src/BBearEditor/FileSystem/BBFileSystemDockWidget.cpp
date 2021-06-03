#include "BBFileSystemDockWidget.h"
#include "ui_BBFileSystemDockWidget.h"
#include "BBFileSystemManager.h"


BBFileSystemDockWidget::BBFileSystemDockWidget(QWidget *pParent)
    : QDockWidget(pParent),
      m_pUi(new Ui::BBFileSystemDockWidget)
{
    m_pUi->setupUi(this);

    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));
    // Stretch the width of the file list
    m_pUi->splitterProject->setStretchFactor(0, 1);
    m_pUi->splitterProject->setStretchFactor(1, 2);

    setConnect();

    m_pFileSystemManager = new BBFileSystemManager(m_pUi->treeFolder, m_pUi->listFile, m_pUi->barFilePath);
}

BBFileSystemDockWidget::~BBFileSystemDockWidget()
{
    BB_SAFE_DELETE(m_pUi);
    BB_SAFE_DELETE(m_pFileSystemManager);
}

void BBFileSystemDockWidget::createProject()
{
    m_pFileSystemManager->createProject();
}

void BBFileSystemDockWidget::openProject()
{
    m_pFileSystemManager->openProject();
}

void BBFileSystemDockWidget::clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem)
{
    m_pFileSystemManager->clickItemInFolderTree(filePath, pItem);
}

void BBFileSystemDockWidget::doubleClickItemInFileList(const QString &filePath)
{
    m_pFileSystemManager->doubleClickItemInFileList(filePath);
}

void BBFileSystemDockWidget::clickItemInFolderPathBar(const QString &filePath)
{
    m_pFileSystemManager->clickItemInFolderPathBar(filePath);
}

void BBFileSystemDockWidget::newFolder(const QString &parentPath, const BBSignalSender &eSender)
{
    m_pFileSystemManager->newFolder(parentPath, eSender);
}

void BBFileSystemDockWidget::showInFolder(const QString &filePath)
{
    m_pFileSystemManager->showInFolder(filePath);
}

void BBFileSystemDockWidget::renameInFolderTree(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath)
{
    m_pFileSystemManager->renameInFolderTree(pParentFolderItem, oldPath, newPath);
}

void BBFileSystemDockWidget::renameInFileList(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath)
{
    m_pFileSystemManager->renameInFileList(pFileItem, oldPath, newPath);
}

void BBFileSystemDockWidget::deleteFolderInFolderTree(QTreeWidgetItem *pItem)
{
    m_pFileSystemManager->deleteFolderInFolderTree(pItem);
}

void BBFileSystemDockWidget::deleteFilesInFileList(const QList<QListWidgetItem*> &items)
{
    m_pFileSystemManager->deleteFilesInFileList(items);
}

void BBFileSystemDockWidget::importAsset(const QString &parentPath, const QList<QUrl> &urls)
{
    m_pFileSystemManager->importAsset(parentPath, urls);
}

void BBFileSystemDockWidget::updateAll()
{
    m_pFileSystemManager->updateAll();
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
    // delete files
    QObject::connect(m_pUi->treeFolder, SIGNAL(deleteFolder(QTreeWidgetItem*)),
                     this, SLOT(deleteFolderInFolderTree(QTreeWidgetItem*)));
    QObject::connect(m_pUi->treeFolder, SIGNAL(finishDeleteAction()),
                     this, SLOT(updateAll()));
    QObject::connect(m_pUi->listFile, SIGNAL(deleteFiles(QList<QListWidgetItem*>)),
                     this, SLOT(deleteFilesInFileList(QList<QListWidgetItem*>)));
    // import external assets
    QObject::connect(m_pUi->listFile, SIGNAL(importAsset(QString, QList<QUrl>)),
                     this, SLOT(importAsset(QString, QList<QUrl>)));
}

