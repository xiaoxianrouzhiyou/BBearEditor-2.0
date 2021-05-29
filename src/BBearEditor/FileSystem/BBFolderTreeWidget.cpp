#include "BBFolderTreeWidget.h"
#include <QDir>
#include <QQueue>
#include <QMenu>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>


BBFolderTreeWidget::BBFolderTreeWidget(QWidget *pParent)
    : BBTreeWidget(pParent)
{
    m_pCurrentShowFolderContentItem = NULL;
//    //初始时 需要加载材质
//    isLoadMaterial = true;

    setMenu();

    QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                     this, SLOT(clickItem(QTreeWidgetItem*, int)));
}

void BBFolderTreeWidget::loadProject()
{
    // it is also invoked when the tree needs to be rebuilt
    // m_pCurrentShowFolderContentItem will be cleared, record corresponding path
    // find new m_pCurrentShowFolderContentItem, after rebuilding
    QString currentShowFolderContentPath;
    if (m_pCurrentShowFolderContentItem)
    {
        currentShowFolderContentPath = getAbsolutePath(m_pCurrentShowFolderContentItem);
    }
    else
    {
        currentShowFolderContentPath = BBConstant::BB_PATH_PROJECT_USER;
    }
    // clear tree, and create new tree
    clear();
    QDir dir(BBConstant::BB_PATH_PROJECT_USER);
    if (dir.exists())
    {
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        // The path is used to traverse the subfolders
        // The item is used to insert
        struct Node
        {
            QString path;
            QTreeWidgetItem *pItem;
            Node(QString path, QTreeWidgetItem *pItem)
            {
                this->path = path;
                this->pItem = pItem;
            }
        };
        // The queue of the parent node of the node to be created
        QQueue<Node> queue;
        // Traverse the contents folder
        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach (QFileInfo fileInfo, fileInfoList)
        {
            // is folder
            if (fileInfo.isDir())
            {
                // add item at the top level
                QTreeWidgetItem *pItem = new QTreeWidgetItem({fileInfo.fileName()});
                pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
                addTopLevelItem(pItem);
                // push queue, used for adding child item
                queue.enqueue(Node(fileInfo.absoluteFilePath(), pItem));
            }
            else
            {
                QString suffix = BBUtils::getFileSuffix(fileInfo);
                if (suffix == "mtl")
                {
//                    loadMaterial(fileInfo.absoluteFilePath());
                }
            }
        }
        // breadth-first traverse subfolders
        while (!queue.isEmpty())
        {
            Node node = queue.dequeue();
            dir = QDir(node.path);
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
            fileInfoList = dir.entryInfoList();
            foreach (QFileInfo fileInfo, fileInfoList)
            {
                // is folder
                if (fileInfo.isDir())
                {
                    // is not at the top level
                    QTreeWidgetItem *pItem = new QTreeWidgetItem({fileInfo.fileName()});
                    pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
                    node.pItem->addChild(pItem);
                    // push queue, used for adding child item
                    queue.enqueue(Node(fileInfo.absoluteFilePath(), pItem));
                }
                else
                {
                    QString suffix = BBUtils::getFileSuffix(fileInfo);
                    if (suffix == "mtl")
                    {
//                        loadMaterial(fileInfo.absoluteFilePath());
                    }
                }
            }
        }
    }
    else
    {
        dir.mkpath(dir.absolutePath());
    }

    sortItems(0, Qt::AscendingOrder);

    // find new m_pCurrentShowFolderContentItem, after rebuilding
    if (m_pCurrentShowFolderContentItem)
    {
        m_pCurrentShowFolderContentItem = getItemByPath(currentShowFolderContentPath);
        setItemExpanded(m_pCurrentShowFolderContentItem, true);
    }
    setCurrentItem(m_pCurrentShowFolderContentItem);
    showFolderContent(currentShowFolderContentPath);
//    //之后重新加载工程时 无需加载材质
//    isLoadMaterial = false;
}

void BBFolderTreeWidget::pressRootButton()
{
    setCurrentItem(NULL);
    // show the files in the root dir in the file list on the right
    showFolderContent(BBConstant::BB_PATH_PROJECT_USER);
    // use NULL to indicate root item
    m_pCurrentShowFolderContentItem = NULL;
}

void BBFolderTreeWidget::clickItem(QTreeWidgetItem *pItem, int nColumn)
{
    updateCorrespondingWidget(pItem);
}

void BBFolderTreeWidget::setCurrentItemByPath(const QString &folderPath)
{
    QTreeWidgetItem *pItem = getItemByPath(folderPath);
    setCurrentItem(pItem);
    setItemExpanded(pItem, true);
    updateCorrespondingWidget(pItem);
}

void BBFolderTreeWidget::newFolder()
{
    QTreeWidgetItem *pParent = currentItem();
    QString parentPath = getAbsolutePath(pParent);
    QString fileName = "new folder";
    QString filePath = BBUtils::getExclusiveFolderPath(parentPath, fileName);

    QDir dir;
    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath));

    // create tree item
    QTreeWidgetItem *pItem = new QTreeWidgetItem({fileName});
    pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));

    if (pParent)
    {
        pParent->addChild(pItem);
        setItemExpanded(pParent, true);
    }
    else
    {
        addTopLevelItem(pItem);
    }
    setCurrentItem(pItem);
    updateCorrespondingWidget(m_pCurrentShowFolderContentItem);
    // Open the edit box to let the user set name
    openRenameEditor();
}

void BBFolderTreeWidget::addItem(const QString &parentPath, const QString &name)
{
    QTreeWidgetItem *pParent = getItemByPath(parentPath);

    QTreeWidgetItem *pItem = new QTreeWidgetItem({name});
    pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));

    if (pParent)
    {
        pParent->addChild(pItem);
        setItemExpanded(pParent, true);
    }
    else
    {
        addTopLevelItem(pItem);
    }
    sortItems(0, Qt::AscendingOrder);
}

void BBFolderTreeWidget::updateItem(const QString &oldName, const QString &newName)
{
    // the name of child of m_pCurrentShowFolderContentItem is changed
    if (m_pCurrentShowFolderContentItem)
    {
        for (int i = 0; i < m_pCurrentShowFolderContentItem->childCount(); i++)
        {
            QTreeWidgetItem *pItem = m_pCurrentShowFolderContentItem->child(i);
            if (pItem->text(0) == oldName)
            {
                pItem->setText(0, newName);
                break;
            }
        }
    }
    else
    {
        // at the top level
        for (int i = 0; i < topLevelItemCount(); i++)
        {
            QTreeWidgetItem *pItem = topLevelItem(i);
            if (pItem->text(0) == oldName)
            {
                pItem->setText(0, newName);
                break;
            }
        }
    }
    sortItems(0, Qt::AscendingOrder);
}

void BBFolderTreeWidget::deleteItem(const QString &folderPath)
{
    QTreeWidgetItem *pItem = getItemByPath(folderPath);
    // remove from clipBoard
//    if (clipBoardItems.contains(pItem))
//    {
//        clipBoardItems.removeOne(pItem);
//    }
    BB_SAFE_DELETE(pItem);
}

void BBFolderTreeWidget::showInFolder()
{
    QTreeWidgetItem *pItem = currentItem();
    QString filePath = getAbsolutePath(pItem);

    BB_PROCESS_ERROR_RETURN(BBUtils::showInFolder(filePath));
}

void BBFolderTreeWidget::finishRename()
{
    BB_PROCESS_ERROR_RETURN(m_pEditingItem);

    // whether name is changed
    // new name cannot be null
    QString name = m_pRenameEditor->text();
    QString preName = m_pEditingItem->text(0);
    if (preName != name && !name.isEmpty())
    {
        // previous path
        QString previous = getAbsolutePath(m_pEditingItem);
        QString parentPath = previous.mid(0, previous.lastIndexOf('/'));
        // Check if there are the same names
        QString current = BBUtils::getExclusiveFolderPath(parentPath, name);

        QFile::rename(previous, current);
        m_pEditingItem->setText(0, name);

        // After renaming, remove it from the clipboard
        if (m_ClipBoardItems.contains(m_pEditingItem))
        {
            m_ClipBoardItems.removeOne(m_pEditingItem);
        }
        // After renaming, sort
        sortItems(0, Qt::AscendingOrder);

        updateCorrespondingWidget(m_pEditingItem);
    }
    BBTreeWidget::finishRename();
}

void BBFolderTreeWidget::deleteAction()
{
    BBTreeWidget::deleteAction();

    updateCorrespondingWidget(m_pCurrentShowFolderContentItem);

//    //刷新材质文件的映射 被删除的材质文件的映射不再占用内存
//    Material::updateMap();
//    //清空属性栏 包括场景 层级视图选中
//    cancelHierarchyTreeSelectedItems();
//    clearPropertyWidget();
}

QString BBFolderTreeWidget::getAbsolutePath(const QString &relativePath)
{
    QString absolutePath = BBConstant::BB_PATH_PROJECT_USER + "/" + relativePath;
    // remove "/" that is at the end
    absolutePath = absolutePath.mid(0, absolutePath.length() - 1);
    return absolutePath;
}

QString BBFolderTreeWidget::getAbsolutePath(QTreeWidgetItem *pItem)
{
    return getAbsolutePath(getLevelPath(pItem));
}

QTreeWidgetItem* BBFolderTreeWidget::getItemByPath(const QString &absolutePath)
{
    // Find the corresponding tree item according to the path
    QString path = absolutePath.mid(BBConstant::BB_PATH_PROJECT_USER.length());
    if (path.length() == 0)
    {
        // "contents" folder
        return NULL;
    }
    else
    {
        QTreeWidgetItem *pItem = NULL;
        // remove "/" at the beginning
        path = path.mid(1);
        QStringList list = path.split('/');
        // Find the item that is at the top level corresponds to item 0 in the list
        // Folders at the same level cannot have the same name
        for (int i = 0; i < topLevelItemCount(); i++)
        {
            if (topLevelItem(i)->text(0) == list.at(0))
            {
                pItem = topLevelItem(i);
                break;
            }
        }
        // Start from item 1 to find non-top items
        for (int i = 1; i < list.count(); i++)
        {
            for (int j = 0; j < pItem->childCount(); j++)
            {
                QTreeWidgetItem *pChild = pItem->child(j);
                if (list.at(i) == pChild->text(0))
                {
                    pItem = pChild;
                    break;
                }
            }
        }
        return pItem;
    }
}

void BBFolderTreeWidget::updateCorrespondingWidget(QTreeWidgetItem *pItem)
{
    QString folderPath = getAbsolutePath(pItem);
    showFolderContent(folderPath);
    m_pCurrentShowFolderContentItem = pItem;
}

void BBFolderTreeWidget::setMenu()
{
    // first level menu
    m_pMenu = new QMenu(this);
    QAction *pActionNewFolder = new QAction(tr("New Folder"));
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
    // second level menu
    QMenu *pMenuNewAsset = new QMenu(tr("New Asset"), m_pMenu);
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "scene.png", tr("Scene")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "material.png", tr("Material")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "animation.png", tr("Animation")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "particle.png", tr("Particle")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "script.png", tr("Script")));
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
    // delete folder
    QString path = getAbsolutePath(pItem);
    QDir dir(path);
    BB_PROCESS_ERROR_RETURN(dir.removeRecursively());

    // delete corresponding folder in the engine folder
    dir = QDir(BBUtils::getEngineAuxiliaryFolderPath(path));
    BB_PROCESS_ERROR_RETURN(dir.removeRecursively());

    // if showing its content, clear, and show root folder
    if (pItem == m_pCurrentShowFolderContentItem)
    {
        m_pCurrentShowFolderContentItem = NULL;
    }

    BBTreeWidget::deleteOne(pItem);
}



//void ProjectTree::updateCurrentShowFolderContentItem(QString path)
//{
//    currentShowFolderContentItem = getItemByPath(path);
//}

//void ProjectTree::addFolderItemInTree(QString parentPath, QString name)
//{
//    QTreeWidgetItem *parent = getItemByPath(parentPath);
//    QTreeWidgetItem *item = new QTreeWidgetItem({name});
//    item->setIcon(0, QIcon(":/icon/resources/icons/folder5.png"));
//    if (parent)
//    {
//        parent->addChild(item);
//    }
//    else
//    {
//        //是top结点
//        addTopLevelItem(item);
//    }
//    //添加好新的结点后 排序
//    sortItems(0, Qt::AscendingOrder);
//}

//void ProjectTree::moveFolderItemInTree(QString prePath, QString newPath)
//{
//    //原来的结点
//    QTreeWidgetItem *item = getItemByPath(prePath);
//    //新位置的parent
//    int index = newPath.lastIndexOf('/');
//    QTreeWidgetItem *parent = getItemByPath(newPath.mid(0, index));
//    //新名字
//    QString name = newPath.mid(index + 1);
//    //删去原来的结点
//    if (item->parent())
//    {
//        item->parent()->removeChild(item);
//    }
//    else
//    {
//        takeTopLevelItem(indexOfTopLevelItem(item));
//    }
//    //在parent下接上结点
//    if (parent)
//    {
//        parent->addChild(item);
//    }
//    else
//    {
//        addTopLevelItem(item);
//    }
//    //重命名
//    item->setText(0, name);
//    //重新排序
//    sortItems(0, Qt::AscendingOrder);
//}



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

//void ProjectTree::removeClipBoardRenameItem(QString path)
//{
//    clipBoardItems.removeOne(getItemByPath(path));
//}

//void ProjectTree::pressSettingButton()
//{
//    //去除树的选中项
//    setCurrentItem(NULL);
//    //弹出菜单
//    menu->exec(cursor().pos());
//}


//void ProjectTree::dragMoveEvent(QDragMoveEvent *event)
//{
//    if (!event->mimeData()->hasFormat(getMimeType()) && !event->mimeData()->hasFormat(FileList::getMimeType()))
//    {
//        event->ignore();
//        return;
//    }
//    BaseTree::dragMoveEvent(event);
//}

//bool ProjectTree::dragDropItem()
//{
//    //拖拽的结点 已经在startDrag中进行过过滤了
//    QList<QTreeWidgetItem*> dragItems = selectedItems();
//    //落下前计算出拖拽之前的选中项的层级位置 便于移动或者撤销移动
//    //清空上一次的dragItemLocations （或者保存到撤销队列中）
//    QList<QString> dragItemLocations;
//    for (int i = 0; i < dragItems.count(); i++)
//    {
//        dragItemLocations.append(getLevelPath(dragItems.at(i)));
//    }
//    //落下
//    if (BaseTree::dragDropItem())
//    {
//        for (int i = 0; i < dragItemLocations.count(); i++)
//        {
//            //落下前保存的拖动项dragItems 之前的路径
//            QString oldPath = getFilePath(dragItemLocations.at(i));
//            //落下后 计算现在的拖动项路径
//            QString newPath = getFilePath(getLevelPath(dragItems.at(i)));
//            //路径没有改变 兄弟之间换了顺序 无需后续操作
//            if (oldPath == newPath)
//                continue;
//            //去掉路径最后的斜杠
//            oldPath = oldPath.mid(0, oldPath.length() - 1);
//            newPath = newPath.mid(0, newPath.length() - 1);
//            //目标路径有重名文件夹 该文件夹名字后加(2)或(3) 类推
//            QDir *dir = new QDir();
//            if (dir->exists(newPath))
//            {
//                int j = 2;
//                while (dir->exists(newPath + "(" + QString::number(j) + ")"))
//                {
//                    j++;
//                }
//                newPath = newPath + "(" + QString::number(j) + ")";
//                //树中对应项也要修改名字
//                dragItems.at(i)->setText(0, dragItems.at(i)->text(0) + "(" + QString::number(j) + ")");
//            }
//            //文件夹拷贝
//            copyDirectoryFiles(oldPath, newPath);
//            //删除原来的文件夹
//            dir = new QDir(oldPath);
//            dir->removeRecursively();
//            //对应的meta文件夹也要移动
//            QString newMetaFolderPath = newPath.mid((projectPath + contentsFolderName).length());
//            newMetaFolderPath = projectEngineFolderPath + contentMetaFolderName + newMetaFolderPath;
//            QString oldMetaFolderPath = oldPath.mid((projectPath + contentsFolderName).length());
//            oldMetaFolderPath = projectEngineFolderPath + contentMetaFolderName + oldMetaFolderPath;
//            copyDirectoryFiles(oldMetaFolderPath, newMetaFolderPath);
//            dir = new QDir(oldMetaFolderPath);
//            dir->removeRecursively();
//        }
//        //树各项按名称排序
//        sortItems(0, Qt::AscendingOrder);
//        //如果当前显示的文件夹是移动的文件夹 需要更新路径 如果移入了当前显示的文件夹 列表增加项
//        //刷新右侧 显示currentShowFolderContentItem对应的文件夹 这个item可能被移动 路径发生了变化
//        QString updatePath = getFilePath(getLevelPath(currentShowFolderContentItem));
//        showFolderContent(updatePath.mid(0, updatePath.length() - 1));
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//bool ProjectTree::moveItemFromFileList(const QMimeData *mimeData)
//{
//    if (indicatorItem)
//    {
//        //拖拽的新路径 只含路径 不带自己的名字
//        QString destPath;
//        if (indicatorPos == IndicatorPos::RECT)
//        {
//            //成为indicatorItem的孩子
//            destPath = getFilePath(getLevelPath(indicatorItem));
//        }
//        else
//        {
//            //成为indicatorItem的兄弟
//            destPath = getFilePath(getLevelPath(indicatorItem->parent()));
//        }
//        //得到所有拖动文件的路径
//        QList<QString> sourceFilePaths;
//        QByteArray data = mimeData->data(FileList::getMimeType());
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString sourceFilePath;
//        //第一个存的是current项对应路径 不需要 需要之后selected对应的路径
//        dataStream >> sourceFilePath;
//        dataStream >> sourceFilePath;
//        while (!sourceFilePath.isEmpty())
//        {
//            sourceFilePaths.append(sourceFilePath);
//            //文件夹不能移到自己里面 也不能移到自己的子文件夹里面
//            if (destPath.mid(0, sourceFilePath.length()) == sourceFilePath)
//            {
//                return false;
//            }
//            //也不能自己成为自己的兄弟 移到自己的父文件夹里
//            if (sourceFilePath.mid(0, sourceFilePath.lastIndexOf('/') + 1) == destPath)
//            {
//                return false;
//            }
//            dataStream >> sourceFilePath;
//        }
//        //移动合法 将所有文件夹移到destPath里面
//        for (int i = 0; i < sourceFilePaths.count(); i++)
//        {
//            QString prePath = sourceFilePaths.at(i);
//            QString fileName = prePath.mid(prePath.lastIndexOf('/') + 1);
//            if (QFileInfo(prePath).isDir())
//            {
//                QString newPath = FileList::checkFolderDuplicateName(destPath + fileName);
//                //移动文件夹
//                copyDirectoryFiles(prePath, newPath);
//                //删除原来位置的文件夹
//                QDir *dir = new QDir(prePath);
//                dir->removeRecursively();
//                //对应的meta文件夹也要移动
//                QString oldMetaFolderPath = FileList::getMetaFilePath(prePath);
//                ProjectTree::copyDirectoryFiles(oldMetaFolderPath, FileList::getMetaFilePath(newPath));
//                dir = new QDir(oldMetaFolderPath);
//                dir->removeRecursively();
//                //文件夹树的结点也要移动
//                moveFolderItemInTree(prePath, newPath);
//            }
//            else
//            {
//                QString newPath = FileList::checkFileDuplicateName(destPath + fileName);
//                QFile::copy(prePath, newPath);
//                //删除原来位置的文件
//                //删除文件
//                if (QFile::remove(prePath))
//                {
//                    QString suffix = newPath.mid(newPath.lastIndexOf('.') + 1);
//                    //有些文件对应的meta文件夹也要移动
//                    if (suffix == "obj")
//                    {
//                        QString oldMetaFilePath = FileList::getMetaFilePath(prePath);
//                        QString newMetaFilePath = FileList::getMetaFilePath(newPath);
//                        //修改后缀为jpg
//                        int index = oldMetaFilePath.lastIndexOf('.');
//                        oldMetaFilePath = oldMetaFilePath.mid(0, index) + ".jpg";
//                        index = newMetaFilePath.lastIndexOf('.');
//                        newMetaFilePath = newMetaFilePath.mid(0, index) + ".jpg";
//                        QFile::copy(oldMetaFilePath, newMetaFilePath);
//                        QFile::remove(oldMetaFilePath);
//                    }
//                    //材质文件需要新建材质对象
//                    else if (suffix == "mtl")
//                    {
//                        new Material(newPath);
//                    }
//                }
//            }
//        }
//        //刷新列表 删去移走的项和映射
//        deleteFileItemInList();
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}


//bool ProjectTree::copyDirectoryFiles(QString fromDir, QString toDir)
//{
//    //拷贝文件夹
//    QDir sourceDir(fromDir);
//    QDir targetDir(toDir);
//    if (!targetDir.exists())
//    {
//        //如果目标目录不存在，则进行创建
//        //mkdir在已有路径上创建文件夹 mkpath可以在没有的路径上创建多级文件夹
//        if (!targetDir.mkpath(targetDir.absolutePath()))
//        {
//            return false;
//        }
//    }
//    //遍历源文件夹
//    QFileInfoList fileInfoList = sourceDir.entryInfoList();
//    foreach (QFileInfo fileInfo, fileInfoList)
//    {
//        //fileInfo.filePath()会出现/./
//        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
//            continue;

//        if (fileInfo.isDir())
//        {
//            //当为目录时，递归的进行copy
//            if (!copyDirectoryFiles(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())))
//                return false;
//        }
//        else
//        {
//            //进行文件copy
//            if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())))
//            {
//                return false;
//            }
//            else
//            {
//                //材质文件需要新建材质对象
//                QString suffix = fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
//                if (suffix == "mtl")
//                {
//                    new Material(fileInfo.absoluteFilePath());
//                }
//            }
//        }
//    }
//    return true;
//}


//void ProjectTree::loadMaterial(QString filePath)
//{
//    if (isLoadMaterial)
//    {
//        new Material(filePath);
//    }
//}
