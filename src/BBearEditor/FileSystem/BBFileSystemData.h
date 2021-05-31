#ifndef BBFILESYSTEMDATA_H
#define BBFILESYSTEMDATA_H


#include <QMap>
#include <QIcon>
#include <QColor>


class QTreeWidgetItem;
class QListWidgetItem;
class QFileInfo;
class BBOpenGLWidget;


enum BBFileType
{
    Dir = 0,
    Mesh = 1,
    Audio = 2,
    Texture = 3,
    Material = 4,
    Script = 5,
    Other = 6
};


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


class BBFileSystemData
{
public:
    BBFileSystemData();
    ~BBFileSystemData();

    void bindPreviewOpenGLWidget(BBOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    void load();
    QList<QTreeWidgetItem*> getFolderTreeWidgetTopLevelItems();
    QList<QListWidgetItem*> getFileListWidgetItems(QTreeWidgetItem *pItem);

public:
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
    BBFILE loadFolderContent(const QString &parentPath);

    QString getEngineAuxiliaryFolderPath(const QString &sourcePath);
    QIcon getIcon(const QString &path);
    QIcon getTextureIcon(const QString &path);
    QIcon getMeshOverviewMap(const QString &sourcePath);
    void createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath);
    QColor getFileLogoColor(const BBFileType &eFileType);

    BBOpenGLWidget *m_pPreviewOpenGLWidget;
    BBFILE m_RootFileData;
    QMap<QTreeWidgetItem*, BBFILE> m_TopLevelFileData;
    QMap<QTreeWidgetItem*, BBFILE> m_FileData;
};

#endif // BBFILESYSTEMDATA_H
