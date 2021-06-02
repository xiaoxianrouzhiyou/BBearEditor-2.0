#ifndef BBFILESYSTEMDATAMANAGER_H
#define BBFILESYSTEMDATAMANAGER_H


#include <QMap>
#include <QIcon>
#include <QColor>
#include "BBUtils.h"


using namespace BBFileSystem;


class QTreeWidgetItem;
class QListWidgetItem;
class QFileInfo;
class BBOpenGLWidget;


struct BBFileInfo
{
    QString m_FileName;
    BBFileType m_eFileType;

    BBFileInfo(const QString &fileName, const BBFileType &eType)
    {
        m_FileName = fileName;
        m_eFileType = eType;
    }
};


typedef QMap<QListWidgetItem*, BBFileInfo*> BBFILE;


struct BBFOLDER
{
    // The path is used to traverse the subfolders
    // The item is used to insert
    QString path;
    QTreeWidgetItem *pItem;

    BBFOLDER(QString path, QTreeWidgetItem *pItem)
    {
        this->path = path;
        this->pItem = pItem;
    }
};


class BBFileSystemDataManager
{
public:
    BBFileSystemDataManager();
    ~BBFileSystemDataManager();

public:
    void bindPreviewOpenGLWidget(BBOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    void load();
    QList<QTreeWidgetItem*> getFolderTreeWidgetTopLevelItems();
    bool getFileListWidgetItems(QTreeWidgetItem *pItem, QList<QListWidgetItem*> &outItems, QList<QString> &outFileNames);
    QTreeWidgetItem* getItemByPath(const QString &absolutePath);
    QTreeWidgetItem* getParentFolderItem(const QString &filePath);
    QListWidgetItem* getFileItem(QTreeWidgetItem *pParentFolderItem, const QString &filePath);
    bool openFile(const QString &filePath);
    bool newFolder(const QString &parentPath, QTreeWidgetItem *&pFolderItem, QListWidgetItem *&pFileItem);
    bool showInFolder(const QString &filePath);
    bool rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                const QString &oldPath, const QString &newPath);

public:
    static QString getAbsolutePath(const QString &relativePath);
    static QString getAbsolutePath(QTreeWidgetItem *pItem);
    static QString getExclusiveFolderPath(const QString &parentPath, QString &fileName);
    static QString getExclusiveFolderPath(const QString &filePath);
    static QString getExclusiveFilePath(const QString &parentPath, QString &fileName);
    static QString getExclusiveFilePath(const QString &filePath);
    static QString getFileSuffix(const QFileInfo &fileInfo);
    static QString getFileSuffix(const QString &name);
    static QString getBaseName(const QString &name);
    static QString getFileNameByPath(const QString &filePath);
    static QString getParentPath(const QString &filePath);
    static QString getOverviewMapPath(const QString &sourcePath);

    static QList<QString> m_MeshSuffixs;
    static QList<QString> m_TextureSuffixs;
    static QList<QString> m_AudioSuffixs;
    static QList<QString> m_ScriptSuffixs;
    static QList<QString> m_MaterialSuffixs;

    static QString m_MeshFileLogoColor;
    static QString m_TextureFileLogoColor;
    static QString m_AudioFileLogoColor;
    static QString m_MaterialFileLogoColor;


private:
    void buildFileData(QQueue<BBFOLDER> &queue);
    BBFILE* loadFolderContent(const QString &parentPath);

    QString getEngineAuxiliaryFolderPath(const QString &sourcePath);
    QIcon getIcon(const QString &path);
    QIcon getTextureIcon(const QString &path);
    QIcon getMeshOverviewMap(const QString &sourcePath);
    void createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath);
    QColor getFileLogoColor(const BBFileType &eFileType);
    BBFileType getFileType(const QString &filePath);
    BBFILE* getFolderContent(QTreeWidgetItem *pItem);

    BBOpenGLWidget *m_pPreviewOpenGLWidget;
    BBFILE *m_pRootFileData;
    QMap<QTreeWidgetItem*, BBFILE*> m_TopLevelFileData;
    QMap<QTreeWidgetItem*, BBFILE*> m_FileData;
};

#endif // BBFILESYSTEMDATAMANAGER_H
