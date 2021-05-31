#include "BBFileSystemData.h"
#include "BBUtils.h"
#include <QDir>
#include <QQueue>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include "BBFileListWidget.h"
#include <QPainter>
#include "BBOpenGLWidget.h"
#include "BBScene.h"
#include "BBModel.h"


QList<QString> BBFileSystemData::m_MeshSuffixs = {"obj", "fbx"};
QList<QString> BBFileSystemData::m_TextureSuffixs = {"png", "jpg", "jpeg", "bmp", "ico", "dds"};
QList<QString> BBFileSystemData::m_AudioSuffixs = {"mp3", "wav"};
QList<QString> BBFileSystemData::m_ScriptSuffixs = {"lua"};
QList<QString> BBFileSystemData::m_MaterialSuffixs = {"mtl"};

QString BBFileSystemData::m_MeshFileLogoColor = "#e85655";
QString BBFileSystemData::m_TextureFileLogoColor = "#e49831";
QString BBFileSystemData::m_AudioFileLogoColor = "#64abe4";
QString BBFileSystemData::m_MaterialFileLogoColor = "#fab8b7";


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

QList<QListWidgetItem*> BBFileSystemData::getFileListWidgetItems(QTreeWidgetItem *pItem)
{
    QList<QListWidgetItem*> items;
    BBFILE file;
    if (pItem == NULL)
    {
        file = m_RootFileData;
    }
    else if (pItem->parent())
    {
        file = m_FileData.value(pItem);
    }
    else
    {
        file = m_TopLevelFileData.value(pItem);
    }
    for (BBFILE::Iterator it = file.begin(); it != file.end(); it++)
    {
        items.append(it.key());
    }
    return items;
}


QString BBFileSystemData::getExclusiveFolderPath(const QString &parentPath, QString &fileName)
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

QString BBFileSystemData::getExclusiveFolderPath(const QString &filePath)
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

QString BBFileSystemData::getExclusiveFilePath(const QString &parentPath, QString &fileName)
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

QString BBFileSystemData::getExclusiveFilePath(const QString &filePath)
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

QString BBFileSystemData::getFileSuffix(const QFileInfo &fileInfo)
{
    return fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
}

QString BBFileSystemData::getFileSuffix(const QString &name)
{
    return name.mid(name.lastIndexOf('.') + 1);
}

QString BBFileSystemData::getBaseName(const QString &name)
{
    return name.mid(0, name.lastIndexOf('.'));
}

QString BBFileSystemData::getFileNameByPath(const QString &filePath)
{
    return filePath.mid(filePath.lastIndexOf('/') + 1);
}

QString BBFileSystemData::getParentPath(const QString &filePath)
{
    return filePath.mid(0, filePath.lastIndexOf('/'));
}

QString BBFileSystemData::getOverviewMapPath(const QString &sourcePath)
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
            BBFILE folderContent = loadFolderContent(fileInfo.absoluteFilePath());
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
//            if (m_MaterialSuffixs.contains(suffix))
//            {
//                loadMaterial(fileInfo.absoluteFilePath());
//            }
        }
    }
}

BBFILE BBFileSystemData::loadFolderContent(const QString &parentPath)
{
    BBFILE folderContent;
    QDir dir(parentPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        pItem->setSizeHint(BBFileListWidget::m_ItemSize);
        if (fileInfo.isDir())
        {
            // is folder
            pItem->setText(fileInfo.fileName());
            pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
            folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Dir));
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
                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Mesh));
            }
            else if (m_TextureSuffixs.contains(suffix))
            {
                // Picture files use themselves as icons
                pItem->setIcon(getTextureIcon(fileInfo.absoluteFilePath()));
                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Texture));
            }
            else if (m_AudioSuffixs.contains(suffix))
            {
                pItem->setIcon(getIcon(BBConstant::BB_PATH_RESOURCE_PICTURE + "audio.jpg"));
                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Audio));
            }
            else if (m_ScriptSuffixs.contains(suffix))
            {
                pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "lua.png"));
                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Script));
            }
            else if (m_MaterialSuffixs.contains(suffix))
            {
//                Material* material = Material::mtlMap.value(fileInfo.absoluteFilePath());
//                item->setIcon(QIcon(material->getPreview()));
//                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Material));
            }
            else
            {
                folderContent.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::Other));
            }
        }
    }
    return folderContent;
}

QString BBFileSystemData::getEngineAuxiliaryFolderPath(const QString &sourcePath)
{
    // the path relative to the engine folder is the same as the path relative to the contents folder
    QString relativePath = sourcePath.mid(BBConstant::BB_PATH_PROJECT_USER.length());

    return BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER + relativePath;
}

QIcon BBFileSystemData::getIcon(const QString &path)
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

QIcon BBFileSystemData::getTextureIcon(const QString &path)
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

QIcon BBFileSystemData::getMeshOverviewMap(const QString &sourcePath)
{
    // read icon from engine folder if it is created before
    // if it does not exist, create
    QString overviewMapPath = BBFileSystemData::getOverviewMapPath(sourcePath);
    QFile file(overviewMapPath);
    if (!file.exists())
    {
        createMeshOverviewMap(sourcePath, overviewMapPath);
    }
    return getIcon(overviewMapPath);
}

void BBFileSystemData::createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath)
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

QColor BBFileSystemData::getFileLogoColor(const BBFileType &eFileType)
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
