#include "BBFolderTreeWidget.h"
#include <QMenu>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QDragMoveEvent>
#include <QMimeData>
#include "BBFileSystemDataManager.h"


BBFolderTreeWidget::BBFolderTreeWidget(QWidget *pParent)
    : BBTreeWidget(pParent), m_eSenderTag(BBSignalSender::FolderTree)
{
//    //初始时 需要加载材质
//    isLoadMaterial = true;

    setMenu();

    QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                     this, SLOT(clickItem(QTreeWidgetItem*, int)));
}

void BBFolderTreeWidget::removeTopLevelItems()
{
    recordItemExpansionState();
    while (topLevelItemCount() > 0)
    {
        takeTopLevelItem(0);
    }
}

void BBFolderTreeWidget::loadTopLevelItems(const QList<QTreeWidgetItem*> &items)
{
    addTopLevelItems(items);
    resumeItemExpansionState();
    sortItems(0, Qt::AscendingOrder);
}

void BBFolderTreeWidget::expandCurrentViewedItem(QTreeWidgetItem *pItem)
{
    setCurrentItem(pItem);
    setItemExpanded(pItem, true);
}

void BBFolderTreeWidget::setSelectedItems(const QList<QTreeWidgetItem*> &items)
{
    // the items share the same parent
//    setItemExpanded(items.first()->parent(), true);
    for (int i = 0; i < items.count(); i++)
    {
        setItemSelected(items.at(i), true);
    }
}

void BBFolderTreeWidget::pressRootButton()
{
    setCurrentItem(NULL);
    // show the files in the root dir in the file list on the right
    // use NULL to indicate root item
    updateCorrespondingWidget(NULL);
}

void BBFolderTreeWidget::pressSettingButton()
{
    setCurrentItem(NULL);
    m_pMenu->exec(cursor().pos());
}

void BBFolderTreeWidget::clickItem(QTreeWidgetItem *pItem, int nColumn)
{
    Q_UNUSED(nColumn);
    updateCorrespondingWidget(pItem);
}

void BBFolderTreeWidget::newFolder()
{
    QTreeWidgetItem *pParent = currentItem();
    QString parentPath = BBFileSystemDataManager::getAbsolutePath(pParent);
    emit newFolder(parentPath, m_eSenderTag);
    // Open the edit box to let the user set name
     openRenameEditor();
}

void BBFolderTreeWidget::newSceneAction()
{
    QTreeWidgetItem *pItem = currentItem();
    QString parentPath = BBFileSystemDataManager::getAbsolutePath(pItem);
    emit newScene(parentPath);
}

void BBFolderTreeWidget::showInFolder()
{
    QTreeWidgetItem *pItem = currentItem();
    QString filePath = BBFileSystemDataManager::getAbsolutePath(pItem);
    emit showInFolder(filePath);
}

void BBFolderTreeWidget::copyAction()
{

}

void BBFolderTreeWidget::pasteAction()
{

}

void BBFolderTreeWidget::finishRename()
{
    BB_PROCESS_ERROR_RETURN(m_pEditingItem);

    // whether name is changed
    // new name cannot be null
    QString oldname = m_pEditingItem->text(0);
    QString newName = m_pRenameEditor->text();
    if (oldname != newName && !newName.isEmpty())
    {
        QTreeWidgetItem *pParentFolderItem = m_pEditingItem->parent();
        QString parentPath = BBFileSystemDataManager::getAbsolutePath(pParentFolderItem);
        BBTreeWidget::finishRename();

        emit rename(pParentFolderItem, parentPath + "/" + oldname, parentPath + "/" + newName);
        sortItems(0, Qt::AscendingOrder);
    }
    else
    {
        BBTreeWidget::finishRename();
    }
}

void BBFolderTreeWidget::deleteAction()
{
    BBTreeWidget::deleteAction();
    emit finishDeleteAction();
}

void BBFolderTreeWidget::setMenu()
{
    // first level menu
    m_pMenu = new QMenu(this);
    QAction *pActionNewFolder = new QAction(tr("New Folder"));

    // second level menu
    QMenu *pMenuNewAsset = new QMenu(tr("New Asset"), m_pMenu);
    QWidgetAction *pActionNewScene = createWidgetAction(pMenuNewAsset, BB_PATH_RESOURCE_ICON(scene.png), tr("Scene"));

    QAction *pActionShowInFolder = new QAction(tr("Show In Folder"));
    QAction *pActionCopy = new QAction(tr("Copy"));
    pActionCopy->setShortcut(QKeySequence(tr("Ctrl+C")));
    QAction *pActionPaste = new QAction(tr("Paste"));
    pActionPaste->setShortcut(QKeySequence(tr("Ctrl+V")));
    QAction *pActionRename = new QAction(tr("Rename"));
#if defined(Q_OS_WIN32)
    pActionRename->setShortcut(Qt::Key_F2);
#elif defined(Q_OS_MAC)
    pActionRename->setShortcut(Qt::Key_Return);
#endif
    QAction *pActionDelete = new QAction(tr("Delete"));

    pMenuNewAsset->addAction(pActionNewScene);
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, BB_PATH_RESOURCE_ICON(material.png), tr("Material")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, BB_PATH_RESOURCE_ICON(animation.png), tr("Animation")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, BB_PATH_RESOURCE_ICON(particle.png), tr("Particle")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, BB_PATH_RESOURCE_ICON(script.png), tr("Script")));

    // first level menu
    m_pMenu->addAction(pActionNewFolder);
    m_pMenu->addMenu(pMenuNewAsset);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionShowInFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionCopy);
    m_pMenu->addAction(pActionPaste);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionRename);
    m_pMenu->addAction(pActionDelete);

    QObject::connect(pActionNewFolder, SIGNAL(triggered()), this, SLOT(newFolder()));
    QObject::connect(pActionNewScene, SIGNAL(triggered()), this, SLOT(newSceneAction()));
    QObject::connect(pActionShowInFolder, SIGNAL(triggered()), this, SLOT(showInFolder()));
    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openRenameEditor()));
    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

QWidgetAction* BBFolderTreeWidget::createWidgetAction(QMenu *pParent, const QString &iconPath, const QString &name)
{
    QWidgetAction *pAction = new QWidgetAction(pParent);
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName("widgetAction");
    pWidget->setStyleSheet("#widgetAction:hover {background: #0ebf9c;}");
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setContentsMargins(6, 2, 6, 2);
    QLabel *pIcon = new QLabel(pWidget);
    QPixmap pix(iconPath);
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(13 * devicePixelRatio(), 13 * devicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pIcon->setPixmap(pix);
    pLayout->addWidget(pIcon);
    QLabel *pText = new QLabel(pWidget);
    pText->setText(name);
    pText->setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\";");
    pLayout->addWidget(pText, Qt::AlignLeft);
    pAction->setDefaultWidget(pWidget);
    return pAction;
}

void BBFolderTreeWidget::deleteOne(QTreeWidgetItem *pItem)
{
    emit deleteFolder(pItem); 
}

void BBFolderTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (!event->mimeData()->hasFormat(getMimeType()) && !event->mimeData()->hasFormat(BB_MIMETYPE_FILELISTWIDGET))
    {
        event->ignore();
        return;
    }
    BBTreeWidget::dragMoveEvent(event);
}

bool BBFolderTreeWidget::moveItem()
{
    BB_PROCESS_ERROR_RETURN_FALSE(m_pIndicatorItem);

    // drop position of moving item
    int index = -1;
    QTreeWidgetItem *pParent = getParentOfMovingItem(index);
    // The movable item has been filtered in startDrag
    QList<QTreeWidgetItem*> items = selectedItems();
    // Cannot move to an item that are moving and its descendants
    // parent and its ancestors cannot be included in items
    for (QTreeWidgetItem *pForeParent = pParent; pForeParent; pForeParent = pForeParent->parent())
    {
        BB_PROCESS_ERROR_RETURN_FALSE(!items.contains(pForeParent));
    }

    emit moveFolders(items, pParent, false);

    sortItems(0, Qt::AscendingOrder);
    return true;
}

bool BBFolderTreeWidget::moveItemFromFileList(const QMimeData *pMimeData)
{
    BB_PROCESS_ERROR_RETURN_FALSE(m_pIndicatorItem);

    // drop position of moving item
    int index = -1;
    QTreeWidgetItem *pParent = getParentOfMovingItem(index);

    // get all file paths from drag
    QList<QString> oldFilePaths;
    QByteArray data = pMimeData->data(BB_MIMETYPE_FILELISTWIDGET);
    QDataStream dataStream(&data, QIODevice::ReadOnly);
    QString oldFilePath;
    // the first is the path of current item, which we do not need
    // we need the paths of selected items which are saved behind
    dataStream >> oldFilePath;
    dataStream >> oldFilePath;
    while (!oldFilePath.isEmpty())
    {
        oldFilePaths.append(oldFilePath);
        dataStream >> oldFilePath;
    }

    emit moveFiles(oldFilePaths, pParent, false);

    sortItems(0, Qt::AscendingOrder);
    return true;
}

void BBFolderTreeWidget::updateCorrespondingWidget(QTreeWidgetItem *pItem)
{
    QString folderPath = BBFileSystemDataManager::getAbsolutePath(pItem);
    emit accessFolder(folderPath, pItem);
}

/**
 * @brief BBFolderTreeWidget::recordItemExpansionState      record expansion state of items before reconstructing the tree
 */
void BBFolderTreeWidget::recordItemExpansionState()
{
    // clear the last
    while (m_ExpandedItems.count() > 0)
    {
        m_ExpandedItems.takeFirst();
    }

    QTreeWidgetItemIterator it(this);
    for (; *it; it++)
    {
        if ((*it)->isExpanded())
        {
            m_ExpandedItems.append(*it);
        }
    }
}

/**
 * @brief BBFolderTreeWidget::setItemExpansionState         set expansion state of all items for new tree reconstructed
 */
void BBFolderTreeWidget::resumeItemExpansionState()
{
    for(int i = 0; i < m_ExpandedItems.count(); i++)
    {
        m_ExpandedItems.at(i)->setExpanded(true);
    }
}





//void ProjectTree::copyAction()
//{
//    //重置剪贴板
//    BaseTree::copyAction();
//    //遍历剪贴板的项 算出对应的路径
//    QList<QString> folderPaths;
//    int count = clipBoardItems.count();
//    for (int i = 0; i < count; i++)
//    {
//        QString folderPath = getFilePath(getLevelPath(clipBoardItems.at(i)));
//        folderPath = folderPath.mid(0, folderPath.length() - 1);
//        folderPaths.append(folderPath);
//    }
//    //发送信号给文件列表 修改文件列表的剪贴板
//    copyToFileList(folderPaths);
//}

//void ProjectTree::copyByFileList(QList<QString> filePaths)
//{
//    //在文件列表中复制 修改文件夹树的剪贴板
//    //清空上一次剪贴板存下的复制内容
//    clipBoardItems.clear();
//    clipBoardFilePaths.clear();
//    //父亲孩子不会同时被选中
//    int count = filePaths.count();
//    for (int i = 0; i < count; i++)
//    {
//        QString path = filePaths.at(i);
//        QFileInfo *fileInfo = new QFileInfo(path);
//        if (fileInfo->isDir())
//        {
//            clipBoardItems.append(getItemByPath(path));
//        }
//        else
//        {
//            //不是文件夹的文件没有对应树结点 存入路径 粘贴时只进行文件粘贴操作 不进行树节点的粘贴操作
//            clipBoardFilePaths.append(path);
//        }
//    }
//}

//void ProjectTree::pasteAction()
//{
//    //清空用于存粘贴的副本文件夹名字 用于高亮显示
//    pastedFolderNames.clear();
//    BaseTree::pasteAction();
//}

//void ProjectTree::pasteEnd()
//{
//    //粘贴结束后调用 粘贴不合法时 不会调用
//    //树重新排序
//    sortItems(0, Qt::AscendingOrder);
//    //处理剪贴板中的非文件夹文件 同时传副本文件夹的路径 用于高亮显示
//    QString destPath = getFilePath(getLevelPath(currentItem()));
//    pasteFile(clipBoardFilePaths, destPath.mid(0, destPath.length() - 1), pastedFolderNames);
//    //可能复制到当前显示的文件夹下了 更新文件列表 这个操作放在pasteFile里执行
//}

//void ProjectTree::pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript)
//{
//    //复制文件夹
//    QString sourcePath = getFilePath(getLevelPath(source));
//    sourcePath = sourcePath.mid(0, sourcePath.length() - 1);
//    QString transcriptPath = getFilePath(getLevelPath(transcript));
//    transcriptPath = transcriptPath.mid(0, transcriptPath.length() - 1);
//    copyDirectoryFiles(sourcePath, transcriptPath);
//    //复制对应meta文件夹
//    copyDirectoryFiles(FileList::getMetaFilePath(sourcePath), FileList::getMetaFilePath(transcriptPath));
//    //存副本文件夹名字 用于文件列表的高亮显示
//    pastedFolderNames.append(transcriptPath.mid(transcriptPath.lastIndexOf('/') + 1));
//}

//void ProjectTree::pasteItemWithoutPasteFile(QList<QString> clipBoardTranscriptFolderNames)
//{
//    //只粘贴树节点 不粘贴文件 粘贴文件的操作已经在FileList中执行了
//    //修改目的结点currentItem为FileList所指 下次在ProjectTree中粘贴 也使用这个目的结点
//    setCurrentItem(currentShowFolderContentItem);
//    //能执行到这个函数 粘贴操作一定合法
//    //去掉选中项的高亮
//    QList<QTreeWidgetItem*> selected = selectedItems();
//    int count = selected.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(selected.at(i), false);
//    }
//    //遍历剪贴板
//    count = clipBoardItems.count();
//    for (int i = 0; i < count; i++)
//    {
//        QTreeWidgetItem *item = clipBoardItems.at(i);
//        //拷贝结点
//        QTreeWidgetItem *transcript = item->clone();
//        //副本的名字可能重复 重命名
//        transcript->setText(0, clipBoardTranscriptFolderNames.at(i));
//        //在目的结点下接入拷贝的结点
//        if (currentShowFolderContentItem)
//        {
//            currentShowFolderContentItem->addChild(transcript);
//        }
//        else
//        {
//            addTopLevelItem(transcript);
//        }
//        //粘贴项高亮
//        setItemSelected(transcript, true);
//    }
//    //展开目的结点
//    if (currentShowFolderContentItem)
//        setItemExpanded(currentShowFolderContentItem, true);
//    //重新排序
//    sortItems(0, Qt::AscendingOrder);
//}
