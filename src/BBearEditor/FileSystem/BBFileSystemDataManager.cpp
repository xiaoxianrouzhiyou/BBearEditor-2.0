#include "BBFileSystemDataManager.h"
#include <QDir>
#include <QQueue>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include "BBFileListWidget.h"
#include <QPainter>
#include "RenderManager/BBOpenGLWidget.h"
#include "BBScene.h"
#include "BBModel.h"
#include "BBTreeWidget.h"


QList<QString> BBFileSystemDataManager::m_MeshSuffixs = {"obj", "fbx"};
QList<QString> BBFileSystemDataManager::m_TextureSuffixs = {"png", "jpg", "jpeg", "bmp", "ico", "dds"};
QList<QString> BBFileSystemDataManager::m_AudioSuffixs = {"mp3", "wav"};
QList<QString> BBFileSystemDataManager::m_ScriptSuffixs = {"lua"};
QList<QString> BBFileSystemDataManager::m_MaterialSuffixs = {"mtl"};

QString BBFileSystemDataManager::m_MeshFileLogoColor = "#e85655";
QString BBFileSystemDataManager::m_TextureFileLogoColor = "#e49831";
QString BBFileSystemDataManager::m_AudioFileLogoColor = "#64abe4";
QString BBFileSystemDataManager::m_MaterialFileLogoColor = "#fab8b7";


BBFileSystemDataManager::BBFileSystemDataManager()
{
    m_pCurrentViewedItem = NULL;
    m_pRootFileData = new BBFILE();
}

BBFileSystemDataManager::~BBFileSystemDataManager()
{
    for (QMap<QTreeWidgetItem*, BBFILE*>::Iterator it = m_TopLevelFileData.begin(); it != m_TopLevelFileData.end(); it++)
    {
        qDeleteAll(*it.value());
        delete it.value();
        delete it.key();
    }
    for (QMap<QTreeWidgetItem*, BBFILE*>::Iterator it = m_FileData.begin(); it != m_FileData.end(); it++)
    {
        qDeleteAll(*it.value());
        delete it.value();
        // it.key() is child item
        // when delete top item of m_TopLevelFileData, their child items had been deleted
        // delete it.key();
    }
    qDeleteAll(*m_pRootFileData);
    delete m_pRootFileData;
}


/**
 * @brief BBFileSystemDataManager::loadProject read all files and build a tree
 */

void BBFileSystemDataManager::load()
{
    QString rootPath = BBConstant::BB_PATH_PROJECT_USER;
    QDir dir(rootPath);
    if (dir.exists())
    {
        buildFileData(rootPath, NULL, m_pRootFileData);
    }
    else
    {
        dir.mkpath(dir.absolutePath());
    }
}

QList<QTreeWidgetItem*> BBFileSystemDataManager::getFolderTreeWidgetTopLevelItems()
{
    QList<QTreeWidgetItem*> items;

    for (QMap<QTreeWidgetItem*, BBFILE*>::Iterator it = m_TopLevelFileData.begin(); it != m_TopLevelFileData.end(); it++)
    {
        items.append(it.key());
    }

    return items;
}

bool BBFileSystemDataManager::getFileListWidgetItems(QTreeWidgetItem *pItem, BBFILE *&pOutFolderContent)
{
    pOutFolderContent = getFolderContent(pItem);
    return true;
}

/**
 * @brief BBFileSystemDataManager::getItemByPath       Find the corresponding tree item according to the path
 * @param absolutePath
 * @return
 */
QTreeWidgetItem* BBFileSystemDataManager::getFolderItemByPath(const QString &absolutePath)
{
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
        for (QMap<QTreeWidgetItem*, BBFILE*>::Iterator it = m_TopLevelFileData.begin(); it != m_TopLevelFileData.end(); it++)
        {
            QString name = ((QTreeWidgetItem*) it.key())->text(0);
            if (name == list.at(0))
            {
                pItem = it.key();
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

QTreeWidgetItem* BBFileSystemDataManager::getParentFolderItem(const QString &filePath)
{
    // cannot use getItemByPath(filePath) and ->parent(), since filePath may be a file but not a folder
    return getFolderItemByPath(getParentPath(filePath));
}

/**
 * @brief BBFileSystemDataManager::getFileItem          find list item corresponding it in its parent
 * @param pFolderItem
 * @return
 */
QListWidgetItem* BBFileSystemDataManager::getFileItem(QTreeWidgetItem *pFolderItem)
{
    QTreeWidgetItem *pParentFolderItem = pFolderItem->parent();
    QString filePath = getAbsolutePath(pFolderItem);
    return getFileItem(pParentFolderItem, filePath);
}

QListWidgetItem* BBFileSystemDataManager::getFileItem(QTreeWidgetItem *pParentFolderItem, const QString &filePath)
{
    BBFILE *pFolderContent = getFolderContent(pParentFolderItem);
    QString fileName = getFileNameByPath(filePath);
    for (BBFILE::Iterator it = pFolderContent->begin(); it != pFolderContent->end(); it++)
    {
        BBFileInfo *pFileInfo = it.value();
        if (pFileInfo->m_FileName == fileName)
        {
            return it.key();
        }
    }
    return NULL;
}

/**
 * @brief BBFileSystemDataManager::openFile
 * @param filePath
 * @return                              returning false means that this is a folder
 */
bool BBFileSystemDataManager::openFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if (fileInfo.isDir())
    {
        return false;
    }
    else
    {
        if (getFileType(filePath) == BBFileType::Script)
        {
            // open file
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    }
    return true;
}

/**
 * @brief BBFileSystemDataManager::newFolder
 * @param parentPath
 * @param pFolderItem                           current item in the folder tree after creating new folder
 * @param pFileList                             current item in the file list after creating new folder
 * @return
 */
bool BBFileSystemDataManager::newFolder(const QString &parentPath, QTreeWidgetItem *&pFolderItem, QListWidgetItem *&pOutFileItem)
{
    QString fileName = "new folder";
    QString filePath = getExclusiveFolderPath(parentPath, fileName);

    QDir dir;
    BB_PROCESS_ERROR_RETURN_FALSE(dir.mkdir(filePath));

    // add its own folder tree item
    pFolderItem = new QTreeWidgetItem({fileName});
    pFolderItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
    // find the tree item of its parent
    QTreeWidgetItem *pParent = getFolderItemByPath(parentPath);
    if (pParent)
    {
        // the map has nothing, since this is a new folder
        m_FileData.insert(pFolderItem, new BBFILE());
        pParent->addChild(pFolderItem);
    }
    else
    {
        m_TopLevelFileData.insert(pFolderItem, new BBFILE());
    }    

    // add file list item at the beginning of list of its parent
    pOutFileItem = new QListWidgetItem({fileName});
    pOutFileItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    pOutFileItem->setSizeHint(BBFileListWidget::m_ItemSize);
    pOutFileItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
    BBFileInfo *pInfo = new BBFileInfo(fileName, BBFileType::Dir);
    // find the BBFILE corresponding the tree item of its parent
    BBFILE *pParentContent = getFolderContent(pParent);
    pParentContent->insert(pOutFileItem, pInfo);
    return true;
}

bool BBFileSystemDataManager::showInFolder(const QString &filePath)
{
    BB_PROCESS_ERROR_RETURN_FALSE(!filePath.isEmpty());
    QProcess process;
    // just identify "\\"
    QString legalPath = filePath;
    legalPath.replace("/", "\\");
    QString cmd = QString("explorer.exe /select,%1").arg(legalPath);
    qDebug() << cmd;
    return process.startDetached(cmd);
}

bool BBFileSystemDataManager::rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                              const QString &oldPath, const QString &newPath)
{
    QString newName;
    QString legalNewPath;
    // find parent folder item of pFileItem
    BBFILE *pFolderContent = getFolderContent(pParentFolderItem);
    BBFileInfo *pFileInfo = pFolderContent->value(pFileItem);
    if (pFileInfo->m_eFileType == BBFileType::Dir)
    {
        QTreeWidgetItem *pFolderItem = getFolderItemByPath(oldPath);
        BB_PROCESS_ERROR_RETURN_FALSE(pFolderItem);

        legalNewPath = getExclusiveFolderPath(newPath);
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::rename(oldPath, legalNewPath));

        // rename corresponding folder in the engine folder
        QFile::rename(getEngineAuxiliaryFolderPath(oldPath), getEngineAuxiliaryFolderPath(legalNewPath));

        // need to rename folder tree item
        newName = getFileNameByPath(legalNewPath);
        pFolderItem->setText(0, newName);
    }
    else
    {
        legalNewPath = getExclusiveFilePath(newPath);
        newName = getFileNameByPath(legalNewPath);
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::rename(oldPath, legalNewPath));
        if (pFileInfo->m_eFileType == BBFileType::Mesh)
        {
            // rename overview map
            QFile::rename(getOverviewMapPath(oldPath), getOverviewMapPath(legalNewPath));
        }
        else if (pFileInfo->m_eFileType == BBFileType::Material)
        {
//            //材质文件 需要修改文件路径与材质对象的映射
//            Material::rename(oldPath, newPath);
        }

    }

    // handle list item
    pFileInfo->m_FileName = newName;
    pFileItem->setText(getBaseName(newName));

//    //如果该文件在剪贴板中 移除
//    if (clipBoardPaths.contains(oldPath))
//    {
//        clipBoardPaths.removeOne(oldPath);
//    }
    return true;
}

bool BBFileSystemDataManager::deleteFolder(QTreeWidgetItem *pItem)
{
    BB_PROCESS_ERROR_RETURN_FALSE(pItem);

    // if showing its content, clear, and show root folder
    if (pItem == m_pCurrentViewedItem)
    {
        m_pCurrentViewedItem = NULL;
    }

    // find list item corresponding it in its parent
    QTreeWidgetItem *pParentFolderItem = pItem->parent();
    QString filePath = getAbsolutePath(pItem);
    QString parentPath = getParentPath(filePath);
    QListWidgetItem *pFileItem = getFileItem(pParentFolderItem, filePath);
    QList<QListWidgetItem*> items;
    items.append(pFileItem);

    return deleteFiles(pParentFolderItem, parentPath, items);
}

bool BBFileSystemDataManager::deleteFiles(QTreeWidgetItem *pParentItem,
                                          const QString &parentPath,
                                          const QList<QListWidgetItem*> &items)
{
    for (int i = 0; i < items.count(); i++)
    {
        QListWidgetItem* pItem = items.at(i);
        BBFILE *pFolderContent = getFolderContent(pParentItem);
        BBFileInfo *pFileInfo = pFolderContent->value(pItem);
        QString path = parentPath + "/" + pFileInfo->m_FileName;
        if (pFileInfo->m_eFileType == BBFileType::Dir)
        {
            QTreeWidgetItem *pFolderItem = getFolderItemByPath(path);
            BB_PROCESS_ERROR_RETURN_FALSE(pFolderItem);

            QDir dir(path);
            BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
            // delete corresponding folder in the engine folder
            dir = QDir(getEngineAuxiliaryFolderPath(path));
            dir.removeRecursively();
//            //刷新材质文件的映射 被删除的材质文件的映射不再占用内存
//            Material::updateMap();

            // delete folder tree item
            deleteFolderItem(pFolderItem);
        }
        else
        {
            BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(path));

            if (pFileInfo->m_eFileType == BBFileType::Mesh)
            {
                // remove overview map
                QFile::remove(getOverviewMapPath(path));
            }
//            //材质文件 需要删除材质对象
//            else if (pFileInfo->m_eFileType == BBFileType::Material)
//            {
//                Material::deleteOne(path);
//            }
        }

        // handle list item
        pFolderContent->remove(pItem);
        BB_SAFE_DELETE(pFileInfo);
        BB_SAFE_DELETE(pItem);

        // remove from clipboard
//        if (clipBoardPaths.contains(path))
//        {
//            clipBoardPaths.removeOne(path);
//        }
    }

    return true;
}

bool BBFileSystemDataManager::importFiles(const QString &parentPath, const QList<QUrl> &urls)
{
    for (int i = 0; i < urls.length(); i++)
    {
        QString importedAssetPath = urls.at(i).toLocalFile();
        // if it is the folder, you need to traverse the sub-files
        // otherwise, directly operate
        QFileInfo fileInfo(importedAssetPath);
        if (fileInfo.isDir())
        {
            // create the folder
            QString rootFolderName = getFileNameByPath(importedAssetPath);
            QString rootFolderPath = getExclusiveFolderPath(parentPath, rootFolderName);

            QDir dir;
            BB_PROCESS_ERROR_RETURN_FALSE(dir.mkpath(rootFolderPath));
            // breadth-first traverse
            QQueue<QString> queue;
            queue.enqueue(importedAssetPath);
            while (!queue.isEmpty())
            {
                QDir dir(queue.dequeue());
                dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
                // the folder exists, traverse sub-files
                QFileInfoList fileInfoList = dir.entryInfoList();
                foreach (QFileInfo fileInfo, fileInfoList)
                {
                    // Keep the path of the sub file relative to the root folder
                    // and replace with the project file path in front
                    // There is no duplicate name problem
                    QString childPath = rootFolderPath + fileInfo.absoluteFilePath().mid(importedAssetPath.length());
                    if (fileInfo.isDir())
                    {
                        // folder enqueue, waiting to traverse its sub-files
                        queue.enqueue(fileInfo.absoluteFilePath());
                        BB_PROCESS_ERROR_RETURN_FALSE(dir.mkpath(childPath));
                    }
                    else
                    {
                        // handle the file
                        importFiles(fileInfo, childPath);
                    }
                }
            }
        }
        else
        {
            // handle the file
            QFileInfo fileInfo(importedAssetPath);
            importFiles(fileInfo, parentPath + "/" + fileInfo.fileName());
        }
    }
    // load folders' tree items and corresponding list items
    return loadImportedData(parentPath);
}

bool BBFileSystemDataManager::moveFolders(const QList<QTreeWidgetItem*> &items, QTreeWidgetItem *pNewParentItem,
                                          bool bCopy, QList<QListWidgetItem*> &outSelectedItems)
{
    // record map between item and its parent
    // the items that have the same parent can be handled at the same time
    QMap<QTreeWidgetItem*, QTreeWidgetItem*> map;
    for (int i = 0; i < items.count(); i++)
    {
        QTreeWidgetItem *pItem = items.at(i);
        QTreeWidgetItem *pOldParentItem = pItem->parent();
        // it just changes the order among the siblings, no need to perform follow-up operation
        if (pOldParentItem == pNewParentItem)
        {
            continue;
        }
        map.insertMulti(pOldParentItem, pItem);
    }
    QList<QTreeWidgetItem*> uniqueKeys = map.uniqueKeys();
    for (int i = 0; i < uniqueKeys.count(); i++)
    {
        QTreeWidgetItem *pOldParentItem = uniqueKeys.at(i);
        QList<QTreeWidgetItem*> sameDirItems = map.values(pOldParentItem);
        QList<QListWidgetItem*> fileItems;
        for (int j = 0; j < sameDirItems.count(); j++)
        {
            // find corresponding file item
            fileItems.append(getFileItem(sameDirItems.at(j)));
        }
        BB_PROCESS_ERROR_RETURN_FALSE(moveFiles(fileItems,
                                                getAbsolutePath(pOldParentItem), pOldParentItem,
                                                getAbsolutePath(pNewParentItem), pNewParentItem,
                                                bCopy));
        outSelectedItems.append(fileItems);
    }

    return true;
}

bool BBFileSystemDataManager::moveFolders(const QList<QString> &oldFilePaths, const QString &newParentPath, bool bCopy,
                                          QList<QListWidgetItem*> &outSelectedItems)
{
    QList<QTreeWidgetItem*> items;
    for (int i = 0; i < oldFilePaths.count(); i++)
    {
        QString filePath = oldFilePaths.at(i);
        // check whether the movement is legal
        BB_PROCESS_ERROR_RETURN_FALSE(isMovablePath(filePath, newParentPath));

        items.append(getFolderItemByPath(filePath));
    }
    moveFolders(items, getFolderItemByPath(newParentPath), bCopy, outSelectedItems);
}

bool BBFileSystemDataManager::moveFiles(const QList<QString> &oldFilePaths, QTreeWidgetItem *pNewParentItem, bool bCopy,
                                        QList<QTreeWidgetItem*> &outSelectedItems)
{
    // oldFilePaths share the same parent
    QString oldParentPath = getParentPath(oldFilePaths.first());
    QTreeWidgetItem *pOldParentItem = getFolderItemByPath(oldParentPath);
    QString newParentPath = getAbsolutePath(pNewParentItem);
    QList<QListWidgetItem*> fileItems;
    for (int i = 0; i < oldFilePaths.count(); i++)
    {
        QString filePath = oldFilePaths.at(i);
        // check whether the movement is legal
        BB_PROCESS_ERROR_RETURN_FALSE(isMovablePath(filePath, newParentPath));

        fileItems.append(getFileItem(pOldParentItem, filePath));
        outSelectedItems.append(getFolderItemByPath(filePath));
    }

    BB_PROCESS_ERROR_RETURN_FALSE(moveFiles(fileItems, oldParentPath, pOldParentItem, newParentPath, pNewParentItem, bCopy));

    return true;
}

bool BBFileSystemDataManager::moveFiles(const QList<QListWidgetItem*> &items,
                                        const QString &oldParentPath,
                                        const QString &newParentPath,
                                        bool bCopy)
{
    QTreeWidgetItem *pOldParentItem = getFolderItemByPath(oldParentPath);
    QTreeWidgetItem *pNewParentItem = getFolderItemByPath(newParentPath);
    return moveFiles(items, oldParentPath, pOldParentItem, newParentPath, pNewParentItem, bCopy);
}

/**
 * @brief BBFileSystemDataManager::moveFiles    move or copy items from old path into new path
 * @param items
 * @param pOldParentItem
 * @param oldParentPath
 * @param pNewParentItem
 * @param newParentPath
 * @param bCopy
 * @return
 */
bool BBFileSystemDataManager::moveFiles(const QList<QListWidgetItem*> &items,
                                        const QString &oldParentPath, QTreeWidgetItem *pOldParentItem,
                                        const QString &newParentPath, QTreeWidgetItem *pNewParentItem,
                                        bool bCopy)
{
    // move or copy BBFILE from pOldParentFolderContent into pNewParentFolderContent
    BBFILE *pOldParentFolderContent = getFolderContent(pOldParentItem);
    for (int i = 0; i < items.count(); i++)
    {
        QListWidgetItem *pFileItem = items.at(i);
        BBFileInfo *pFileInfo = pOldParentFolderContent->value(pFileItem);
        QString name = pFileInfo->m_FileName;
        QString oldPath = oldParentPath + "/" + name;
        QString newPath;

        if (pFileInfo->m_eFileType == BBFileType::Dir)
        {
            newPath = getExclusiveFolderPath(newParentPath, name);
            BB_PROCESS_ERROR_RETURN_FALSE(moveFolder(oldPath, newPath, bCopy));
            // need to move corresponding tree item
            QTreeWidgetItem *pFolderItem = getFolderItemByPath(oldPath);
            moveFolderItem(pFolderItem, pOldParentItem, pNewParentItem);

            // need to rename folder tree item
            pFolderItem->setText(0, name);
        }
        else
        {
            newPath = getExclusiveFilePath(newParentPath, name);
            BB_PROCESS_ERROR_RETURN_FALSE(moveFile(oldPath, newPath, pFileInfo->m_eFileType, bCopy));
        }

        // handle list item
        pFileInfo->m_FileName = name;
        pFileItem->setText(getBaseName(name));

//        //如果该文件在剪贴板中 移除
//        if (clipBoardPaths.contains(oldPath))
//        {
//            clipBoardPaths.removeOne(oldPath);
//        }

        if (bCopy)
        {
            // new those that is in the old path

        }
    }

    return true;
}

QString BBFileSystemDataManager::getAbsolutePath(const QString &relativePath)
{
    if (relativePath.isEmpty())
    {
        return BBConstant::BB_PATH_PROJECT_USER;
    }
    else
    {
        return BBConstant::BB_PATH_PROJECT_USER + "/" + relativePath;
    }
}

QString BBFileSystemDataManager::getAbsolutePath(QTreeWidgetItem *pItem)
{
    return getAbsolutePath(BBTreeWidget::getLevelPath(pItem));
}

QString BBFileSystemDataManager::getExclusiveFolderPath(const QString &parentPath, QString &fileName)
{
    QDir dir;
    QString filePath = parentPath + "/" + fileName;
    if (dir.exists(filePath))
    {
        // if exist, number that is at the end will increase
        filePath += " ";
        int i = 2;
        while (dir.exists(filePath + QString::number(i)))
        {
            i++;
        }
        fileName = fileName + " " + QString::number(i);
        return filePath + QString::number(i);
    }
    else
    {
        // there is no the same name
        return filePath;
    }
}

QString BBFileSystemDataManager::getExclusiveFolderPath(const QString &filePath)
{
    QDir dir;
    if (dir.exists(filePath))
    {
        // if exist, number that is at the end will increase
        int i = 2;
        while (dir.exists(filePath + " " + QString::number(i)))
        {
            i++;
        }
        return filePath + " " + QString::number(i);
    }
    else
    {
        // there is no the same name
        return filePath;
    }
}

QString BBFileSystemDataManager::getExclusiveFilePath(const QString &parentPath, QString &fileName)
{
    QFile file;
    QString filePath = parentPath + "/" + fileName;
    if (file.exists(filePath))
    {
        QString suffix = getFileSuffix(fileName);
        QString baseName = getBaseName(fileName);
        filePath = parentPath + "/" + baseName + " ";
        int i = 2;
        while (file.exists(filePath + QString::number(i) + "." + suffix))
        {
            i++;
        }
        fileName = baseName + " " + QString::number(i) + "." + suffix;
        return filePath + QString::number(i) + "." + suffix;
    }
    else
    {
        return filePath;
    }
}

QString BBFileSystemDataManager::getExclusiveFilePath(const QString &filePath)
{
    QFile file;
    if (file.exists(filePath))
    {
        QString fileName = getFileNameByPath(filePath);
        QString suffix = getFileSuffix(fileName);
        // remove suffix
        QString newPath = filePath.mid(0, filePath.lastIndexOf('.'));
        newPath += " ";
        int i = 2;
        while (file.exists(newPath + QString::number(i) + "." + suffix))
        {
            i++;
        }
        return newPath + QString::number(i) + "." + suffix;
    }
    else
    {
        return filePath;
    }
}

QString BBFileSystemDataManager::getFileSuffix(const QFileInfo &fileInfo)
{
    return fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
}

QString BBFileSystemDataManager::getFileSuffix(const QString &name)
{
    int nIndex = name.lastIndexOf('.');
    if (nIndex < 0)
    {
        return "";
    }
    else
    {
        return name.mid(nIndex + 1);
    }
}

QString BBFileSystemDataManager::getBaseName(const QString &name)
{
    return name.mid(0, name.lastIndexOf('.'));
}

QString BBFileSystemDataManager::getFileNameByPath(const QString &filePath)
{
    return filePath.mid(filePath.lastIndexOf('/') + 1);
}

QString BBFileSystemDataManager::getParentPath(const QString &filePath)
{
    return filePath.mid(0, filePath.lastIndexOf('/'));
}

QString BBFileSystemDataManager::getOverviewMapPath(const QString &sourcePath)
{
    QString fileName = getFileNameByPath(sourcePath);
    QString suffix = getFileSuffix(fileName);
    QString baseName = getBaseName(fileName);
    if (m_MeshSuffixs.contains(suffix))
    {
        fileName = "mesh_" + baseName + ".jpg";
    }
    else if (m_MaterialSuffixs.contains(suffix))
    {
        fileName = "material_" + baseName + ".jpg";
    }

    // remove the name of sourcePath, there is a '/' at the end for the convenience of calculation
    QString relativePath = sourcePath.mid(0, sourcePath.lastIndexOf('/') + 1);
    // the path relative to the engine folder is the same as the path relative to the contents folder
    relativePath = relativePath.mid(BBConstant::BB_PATH_PROJECT_USER.length() + 1);

    relativePath = relativePath + fileName;

    return BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER + "/" + relativePath;
}

QColor BBFileSystemDataManager::getFileLogoColor(const BBFileType &eFileType)
{
    if (eFileType == BBFileType::Mesh)
    {
        return QColor(m_MeshFileLogoColor);
    }
    else if (eFileType == BBFileType::Texture)
    {
        return QColor(m_TextureFileLogoColor);
    }
    else if (eFileType == BBFileType::Audio)
    {
        return QColor(m_AudioFileLogoColor);
    }
    else if (eFileType == BBFileType::Material)
    {
        return QColor(m_MaterialFileLogoColor);
    }
    else
    {
        return nullptr;
    }
}

QString BBFileSystemDataManager::getEngineAuxiliaryFolderPath(const QString &sourcePath)
{
    // the path relative to the engine folder is the same as the path relative to the contents folder
    QString relativePath = sourcePath.mid(BBConstant::BB_PATH_PROJECT_USER.length());

    return BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER + relativePath;
}

QIcon BBFileSystemDataManager::getIcon(const QString &path)
{
    // Cut into a square
    QPixmap pix(path);
    if (pix.isNull())
    {
        return QIcon(QString(BB_PATH_RESOURCE_ICON) + "empty2");
    }
    else
    {
        int h = pix.height();
        int w = pix.width();
        int size = h < w ? h : w;
        pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);
        return QIcon(pix);
    }
}

QIcon BBFileSystemDataManager::getTextureIcon(const QString &path)
{
    QPixmap pix(path);
    int h = pix.height();
    int w = pix.width();
    int size = h < w ? h : w;
    pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);
    // Transparent pictures need to add background
    // When the image is smaller than the icon size, use the icon size. The image is showed in the center
    int nBackgroundSize = size > BBFileListWidget::m_ItemSize.width() ? size : BBFileListWidget::m_ItemSize.width();
    QPixmap background(nBackgroundSize, nBackgroundSize);
    background.fill(QColor("#d6dfeb"));
    QPainter painter(&background);
    painter.drawPixmap((nBackgroundSize - size) / 2, (nBackgroundSize - size) / 2, pix);
    painter.end();
    return QIcon(background);
}

bool BBFileSystemDataManager::copyFolder(const QString &fromDir, const QString &toDir)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists())
    {
        // if it does not exist, create it
        BB_PROCESS_ERROR_RETURN_FALSE(targetDir.mkpath(targetDir.absolutePath()));
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        // fileInfo.filePath() has /./
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir())
        {
            // recursive
            BB_PROCESS_ERROR_RETURN_FALSE(copyFolder(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
        }
        else
        {
            // copy files
            BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
//            //材质文件需要新建材质对象
//            QString suffix = fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
//            if (suffix == "mtl")
//            {
//                new Material(fileInfo.absoluteFilePath());
//            }
        }
    }
    return true;
}

bool BBFileSystemDataManager::isMovablePath(const QString &sourcePath, const QString &destParentPath)
{
    // Folders can’t be moved into oneself, nor can they be moved into their own subfolders
    BB_PROCESS_ERROR_RETURN_FALSE(!(destParentPath.mid(0, sourcePath.length()) == sourcePath));
    // It can’t become its own brother and move into its own parent folder
    BB_PROCESS_ERROR_RETURN_FALSE(!(getParentPath(sourcePath) == destParentPath));
    return true;
}

bool BBFileSystemDataManager::moveFolder(const QString &oldPath, const QString &newPath, bool bCopy)
{
    // newPath has been checked for duplicate name problem
    // copy folder
    BB_PROCESS_ERROR_RETURN_FALSE(copyFolder(oldPath, newPath));
    // handle corresponding folder in the engine folder
    QString oldAuxiliaryFolderPath = getEngineAuxiliaryFolderPath(oldPath);
    QString newAuxiliaryFolderPath = getEngineAuxiliaryFolderPath(newPath);
    copyFolder(oldAuxiliaryFolderPath, newAuxiliaryFolderPath);

    if (!bCopy)
    {
        // delete original folder
        QDir dir(oldPath);
        BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
        dir = QDir(oldAuxiliaryFolderPath);
        dir.removeRecursively();
    }
    return true;
}

bool BBFileSystemDataManager::moveFile(const QString &oldPath, const QString &newPath, BBFileType eFileType, bool bCopy)
{
    // newPath has been checked for duplicate name problem
    // copy file
    BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(oldPath, newPath));
    // handle corresponding folder in the engine folder
    QString oldOverviewMapPath;
    QString newOverviewMapPath;
    if (eFileType == BBFileType::Mesh)
    {
        oldOverviewMapPath = getOverviewMapPath(oldPath);
        newOverviewMapPath = getOverviewMapPath(newPath);
        QFile::copy(oldOverviewMapPath, newOverviewMapPath);
    }
//    //材质文件需要新建材质对象
//    else if (fileInfo->mFileType == FileType::material)
//    {
//        new Material(newPath);
//    }

    if (!bCopy)
    {
        // delete original file
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(oldPath));
        QFile::remove(oldOverviewMapPath);
    }
    return true;
}

/**
 * @brief BBFileSystemDataManager::buildFileData
 * @param rootPath
 * @param pRootItem
 * @param nameFilter                                the file item whose name is included in the nameFilter will not be created
 */
void BBFileSystemDataManager::buildFileData(const QString &rootPath, QTreeWidgetItem *pRootItem, BBFILE *&pRootFileData,
                                            const QList<QString> &nameFilter)
{
    QList<QListWidgetItem*> newItems;
    // the content of root folder
    BBFILE *pRootFolderContent = loadFolderContent(rootPath, newItems, nameFilter);

    while (m_SelectedFileItems.count() > 0)
    {
        m_SelectedFileItems.takeFirst();
    }
    // record and then select these items newly created in the file list
    m_SelectedFileItems.append(newItems);

    pRootFileData->unite(*pRootFolderContent);
    BB_SAFE_DELETE(pRootFolderContent);
    // The queue of the parent node of the node to be created
    QQueue<BBFOLDER> queue;
    // init
    queue.enqueue(BBFOLDER(rootPath, pRootItem));
    // Traverse the root folder
    buildFileData(queue, nameFilter);
    // breadth-first traverse subfolders
    while (!queue.isEmpty())
    {
        buildFileData(queue, nameFilter);
    }
}

/**
 * @brief BBFileSystemDataManager::buildFileData
 * @param queue
 * @param nameFilter
 */
void BBFileSystemDataManager::buildFileData(QQueue<BBFOLDER> &queue, const QList<QString> &nameFilter)
{
    BBFOLDER folder = queue.dequeue();
    QDir dir(folder.path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        if (nameFilter.contains(fileInfo.fileName()))
        {
            continue;
        }
        // is folder
        if (fileInfo.isDir())
        {
            QTreeWidgetItem *pItem = new QTreeWidgetItem({fileInfo.fileName()});
            pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
            QList<QListWidgetItem*> newItems;
            BBFILE *pFolderContent = loadFolderContent(fileInfo.absoluteFilePath(), newItems);
            if (folder.pItem)
            {
                // is not at the top level
                folder.pItem->addChild(pItem);
                m_FileData.insert(pItem, pFolderContent);
            }
            else
            {
                // add it at the top level
                m_TopLevelFileData.insert(pItem, pFolderContent);
            }
            // push queue, used for adding child item
            queue.enqueue(BBFOLDER(fileInfo.absoluteFilePath(), pItem));
        }
        else
        {
//            QString suffix = getFileSuffix(fileInfo);
//            if (m_MaterialSuffixs.contains(suffix))
//            {
//                loadMaterial(fileInfo.absoluteFilePath());
//            }
        }
    }
}

BBFILE* BBFileSystemDataManager::loadFolderContent(const QString &parentPath,
                                                   QList<QListWidgetItem*> &newItems,
                                                   const QList<QString> &nameFilter)
{
    BBFILE *pFolderContent = new BBFILE();
    QDir dir(parentPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        if (nameFilter.contains(fileInfo.fileName()))
        {
            continue;
        }

        QListWidgetItem *pItem = new QListWidgetItem;
        newItems.append(pItem);
        pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        pItem->setSizeHint(BBFileListWidget::m_ItemSize);
        if (fileInfo.isDir())
        {
            // is folder
            pItem->setText(fileInfo.fileName());
            pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
            pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Dir));
        }
        else
        {
            // is file
            pItem->setText(fileInfo.baseName());
            QString suffix = fileInfo.suffix();
            if (m_MeshSuffixs.contains(suffix))
            {
                QString sourcePath = fileInfo.absoluteFilePath();
                QIcon icon = getMeshOverviewMap(sourcePath);
                pItem->setIcon(icon);
                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Mesh));
            }
            else if (m_TextureSuffixs.contains(suffix))
            {
                // Picture files use themselves as icons
                pItem->setIcon(getTextureIcon(fileInfo.absoluteFilePath()));
                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Texture));
            }
            else if (m_AudioSuffixs.contains(suffix))
            {
                pItem->setIcon(getIcon(BBConstant::BB_PATH_RESOURCE_PICTURE + "audio.jpg"));
                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Audio));
            }
            else if (m_ScriptSuffixs.contains(suffix))
            {
                pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "lua.png"));
                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Script));
            }
            else if (m_MaterialSuffixs.contains(suffix))
            {
//                Material* material = Material::mtlMap.value(fileInfo.absoluteFilePath());
//                item->setIcon(QIcon(material->getPreview()));
//                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Material));
            }
            else
            {
                pFolderContent->insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Other));
            }
        }
    }
    return pFolderContent;
}

QIcon BBFileSystemDataManager::getMeshOverviewMap(const QString &sourcePath)
{
    // read icon from engine folder if it is created before
    // if it does not exist, create
    QString overviewMapPath = getOverviewMapPath(sourcePath);
    QFile file(overviewMapPath);
    if (!file.exists())
    {
        createMeshOverviewMap(sourcePath, overviewMapPath);
    }
    return getIcon(overviewMapPath);
}

void BBFileSystemDataManager::createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath)
{
    // set default skybox
    // m_pPreviewOpenGLWidget->getScene()->setSkyBox(QString(BB_PATH_RESOURCE) + "skyboxs/3/");
    // preview of mesh
    BBGameObject *pModel = m_pPreviewOpenGLWidget->getScene()->createModelForPreview(sourcePath);
    // Take a screenshot of the overview map as an icon
    QPixmap pix = m_pPreviewOpenGLWidget->grab();
    // Check whether the folder that the overviewMapPath belongs to exists and create it if it does not exist
    QString parentPath = QFileInfo(overviewMapPath).absolutePath();
    QDir dir(parentPath);
    if (!dir.exists())
        dir.mkpath(parentPath);
    pix.save(overviewMapPath);
    // remove the mesh
    m_pPreviewOpenGLWidget->getScene()->deleteGameObject(pModel);
}

BBFileType BBFileSystemDataManager::getFileType(const QString &filePath)
{
    QTreeWidgetItem *pItem = getFolderItemByPath(filePath);
    // to do ... m_RootFileData  m_TopLevelFileData  m_FileData
    return BBFileType::Other;
}

BBFILE* BBFileSystemDataManager::getFolderContent(QTreeWidgetItem *pItem)
{
    BBFILE *pFolderContent;
    if (pItem == NULL)
    {
        pFolderContent = m_pRootFileData;
    }
    else if (pItem->parent())
    {
        pFolderContent = m_FileData.value(pItem);
    }
    else
    {
        pFolderContent = m_TopLevelFileData.value(pItem);
    }
    return pFolderContent;
}

bool BBFileSystemDataManager::deleteFolderItem(QTreeWidgetItem *pItem)
{
    // release BBFILE corresponding the pItem
    BBFILE *pFolderContent = getFolderContent(pItem);
    qDeleteAll(*pFolderContent);

    if (pItem)
    {
        if (pItem->parent())
        {
            m_FileData.remove(pItem);
        }
        else
        {
            m_TopLevelFileData.remove(pItem);
        }
    }

    BB_SAFE_DELETE(pFolderContent);
    BB_SAFE_DELETE(pItem);
    return true;
}

bool BBFileSystemDataManager::moveFolderItem(QTreeWidgetItem *pFolderItem,
                                             QTreeWidgetItem *pOldParentItem, QTreeWidgetItem *pNewParentItem)
{
    // also need to move corresponding list item
    // must perform it before changing the parent of pFolderItem
    QListWidgetItem *pFileItem = getFileItem(pFolderItem);
    BBFILE *pOldParentFolderContent = getFolderContent(pOldParentItem);
    BBFILE *pNewParentFolderContent = getFolderContent(pNewParentItem);
    BBFileInfo *pFileInfo = pOldParentFolderContent->take(pFileItem);
    pNewParentFolderContent->insert(pFileItem, pFileInfo);

    if (pOldParentItem)
    {
        if (pNewParentItem)
        {
            // pItem was not in the top level before
            // pItem is not in the top level now
            pOldParentItem->removeChild(pFolderItem);
            pNewParentItem->addChild(pFolderItem);
        }
        else
        {
            // pItem was not in the top level before
            // pItem is in the top level now
            pOldParentItem->removeChild(pFolderItem);
            BBFILE *pFolderContent = m_FileData.take(pFolderItem);
            m_TopLevelFileData.insert(pFolderItem, pFolderContent);
        }
    }
    else
    {
        if (pNewParentItem)
        {
            // pItem was in the top level before
            // pItem is not in the top level now
            pNewParentItem->addChild(pFolderItem);
            BBFILE *pFolderContent = m_TopLevelFileData.take(pFolderItem);
            m_FileData.insert(pFolderItem, pFolderContent);
        }
        // else
        // {
            // pItem was in the top level before
            // pItem is in the top level now
        // }
    }
}

bool BBFileSystemDataManager::importFiles(const QFileInfo &fileInfo, const QString &newPath)
{
    QString suffix = fileInfo.suffix();
    if (m_TextureSuffixs.contains(suffix) || m_ScriptSuffixs.contains(suffix))
    {
        // import directly
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(fileInfo.absoluteFilePath(), getExclusiveFilePath(newPath)));
    }
    else if (m_MeshSuffixs.contains(suffix))
    {
        QString targetPath = getExclusiveFilePath(newPath);
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(fileInfo.absoluteFilePath(), targetPath));
        createMeshOverviewMap(targetPath, getOverviewMapPath(targetPath));
    }
    return true;
}

/**
 * @brief BBFileSystemDataManager::loadImportedAsset
 *        Traverse the child files of parentPath and construct corresponding items for the newly imported files
 * @param parentPath
 * @return
 */
bool BBFileSystemDataManager::loadImportedData(const QString &parentPath)
{
    QTreeWidgetItem *pParentFolderItem = getFolderItemByPath(parentPath);
    BBFILE *pFolderContent = getFolderContent(pParentFolderItem);
    BB_PROCESS_ERROR_RETURN_FALSE(pFolderContent);
    // the names of all sub files
    QList<QString> names;
    for (BBFILE::Iterator it = pFolderContent->begin(); it != pFolderContent->end(); it++)
    {
        names.append(it.value()->m_FileName);
    }
    buildFileData(parentPath, pParentFolderItem, pFolderContent, names);

    return true;
}
