#include "BBFileSystemData.h"
#include "BBUtils.h"
#include <QDir>
#include <QQueue>
#include <QTreeWidgetItem>
#include <QListWidgetItem>


BBFileSystemData::BBFileSystemData()
{

}

BBFileSystemData::~BBFileSystemData()
{
    for (QMap<QTreeWidgetItem*, BBFILE>::Iterator it = m_TopLevelFileData.begin(); it != m_TopLevelFileData.end(); it++)
    {
        qDeleteAll(it.value());
        delete it.key();
    }
    for (QMap<QTreeWidgetItem*, BBFILE>::Iterator it = m_FileData.begin(); it != m_FileData.end(); it++)
    {
        qDeleteAll(it.value());
        // it.key() is child item
        // when delete top item of m_TopLevelFileData, their child items had been deleted
        // delete it.key();
    }
}


/**
 * @brief BBFileSystemData::loadProject read all files and build a tree
 */

void BBFileSystemData::load()
{
    QString rootPath = BBConstant::BB_PATH_PROJECT_USER;
    QDir dir(rootPath);
    if (dir.exists())
    {
        // The queue of the parent node of the node to be created
        QQueue<BBFOLDER> queue;
        // init
        queue.enqueue(BBFOLDER(rootPath, NULL));
        // Traverse the root folder
        buildFileData(queue);
        // breadth-first traverse subfolders
        while (!queue.isEmpty())
        {
            buildFileData(queue);
        }
    }
    else
    {
        dir.mkpath(dir.absolutePath());
    }
}

QList<QTreeWidgetItem*> BBFileSystemData::getFolderTreeWidgetTopLevelItems()
{
    QList<QTreeWidgetItem*> items;

    for (QMap<QTreeWidgetItem*, BBFILE>::Iterator it = m_TopLevelFileData.begin(); it != m_TopLevelFileData.end(); it++)
    {
        items.append(it.key());
    }

    return items;
}

void BBFileSystemData::buildFileData(QQueue<BBFOLDER> &queue)
{
    BBFOLDER folder = queue.dequeue();
    QDir dir(folder.path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        // is folder
        if (fileInfo.isDir())
        {
            QTreeWidgetItem *pItem = new QTreeWidgetItem({fileInfo.fileName()});
            pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
            BBFILE folderContent = getFolderContent(folder.path);
            if (folder.pItem)
            {
                // is not at the top level
                folder.pItem->addChild(pItem);
                m_FileData.insert(pItem, folderContent);
            }
            else
            {
                // add it at the top level
                m_TopLevelFileData.insert(pItem, folderContent);
            }
            // push queue, used for adding child item
            queue.enqueue(BBFOLDER(fileInfo.absoluteFilePath(), pItem));
        }
        else
        {
//            QString suffix = getFileSuffix(fileInfo);
//            if (suffix == "mtl")
//            {
//                loadMaterial(fileInfo.absoluteFilePath());
//            }
        }
    }
}

BBFILE BBFileSystemData::getFolderContent(const QString &parentPath)
{
    BBFILE map;
    return map;
}
