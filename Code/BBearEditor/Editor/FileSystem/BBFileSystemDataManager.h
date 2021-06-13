#ifndef BBFILESYSTEMDATAMANAGER_H
#define BBFILESYSTEMDATAMANAGER_H


#include <QMap>
#include <QIcon>
#include <QColor>
#include "BBUtils.h"


using namespace BBFileSystem;


class QFileInfo;
class BBOpenGLWidget;


class BBFileSystemDataManager
{
public:
    BBFileSystemDataManager();
    ~BBFileSystemDataManager();

public:
    void bindPreviewOpenGLWidget(BBOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    inline void setCurrentViewedItem(QTreeWidgetItem *pItem) { m_pCurrentViewedItem = pItem; }
    inline QTreeWidgetItem* getCurrentViewedItem() { return m_pCurrentViewedItem; }
    void load();
    QList<QTreeWidgetItem*> getFolderTreeWidgetTopLevelItems();
    bool getFileListWidgetItems(QTreeWidgetItem *pItem, BBFILE *&pOutFolderContent);
    QTreeWidgetItem* getFolderItemByPath(const QString &absolutePath);
    QTreeWidgetItem* getParentFolderItem(const QString &filePath);
    QListWidgetItem* getFileItem(QTreeWidgetItem *pFolderItem);
    QListWidgetItem* getFileItem(QTreeWidgetItem *pParentFolderItem, const QString &filePath);
    QListWidgetItem* getFileItem(QTreeWidgetItem *pParentFolderItem, const QString &filePath, BBFileInfo *&pOutFileInfo);
    inline QList<QListWidgetItem*> getSelectedFileItems() { return m_SelectedFileItems; }
    bool openFile(const QString &filePath);
    bool newFolder(const QString &parentPath, QTreeWidgetItem *&pOutFolderItem, QListWidgetItem *&pOutFileItem);
    bool newScene(const QString &parentPath, QListWidgetItem *&pOutFileItem,
                  QString fileName = BBConstant::BB_NAME_DEFAULT_SCENE);
    bool showInFolder(const QString &filePath);
    bool rename(QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pFileItem,
                const QString &oldPath, const QString &newPath);
    bool deleteFolder(QTreeWidgetItem *pItem);
    bool deleteFiles(QTreeWidgetItem *pParentItem, const QString &parentPath, const QList<QListWidgetItem*> &items);
    bool importFiles(const QString &parentPath, const QList<QUrl> &urls);
    bool moveFolders(const QList<QTreeWidgetItem*> &items, QTreeWidgetItem *pNewParentItem, bool bCopy,
                     QList<QListWidgetItem*> &outSelectedItems);
    bool moveFolders(const QList<QString> &oldFilePaths, const QString &newParentPath, bool bCopy,
                     QList<QListWidgetItem*> &outSelectedItems);
    bool moveFiles(const QList<QString> &oldFilePaths, QTreeWidgetItem *pNewParentItem, bool bCopy,
                   QList<QTreeWidgetItem*> &outSelectedItems);
    bool moveFiles(const QList<QListWidgetItem*> &items,
                   const QString &oldParentPath,
                   const QString &newParentPath,
                   bool bCopy);
    bool moveFiles(const QList<QListWidgetItem*> &items,
                   const QString &oldParentPath, QTreeWidgetItem *pOldParentItem,
                   const QString &newParentPath, QTreeWidgetItem *pNewParentItem,
                   bool bCopy);

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
    static QColor getFileLogoColor(const BBFileType &eFileType);
    static QString getEngineAuxiliaryFolderPath(const QString &sourcePath);
    static QIcon getIcon(const QString &path);
    static QIcon getTextureIcon(const QString &path);
    static bool copyFolder(const QString &fromDir, const QString &toDir);
    static bool isMovablePath(const QString &sourcePath, const QString &destParentPath);
    static bool moveFolder(const QString &oldPath, const QString &newPath, bool bCopy = false);
    static bool moveFile(const QString &oldPath, const QString &newPath, BBFileType eFileType, bool bCopy = false);

    static QList<QString> m_MeshSuffixs;
    static QList<QString> m_TextureSuffixs;
    static QList<QString> m_AudioSuffixs;
    static QList<QString> m_SceneSuffixs;
    static QList<QString> m_ScriptSuffixs;
    static QList<QString> m_MaterialSuffixs;

    static QString m_MeshFileLogoColor;
    static QString m_TextureFileLogoColor;
    static QString m_AudioFileLogoColor;
    static QString m_MaterialFileLogoColor;


private:
    void buildFileData(const QString &rootPath, QTreeWidgetItem *pRootItem, BBFILE *&pRootFileData,
                       const QList<QString> &nameFilter = QList<QString>());
    void buildFileData(QQueue<BBFOLDER> &queue, const QList<QString> &nameFilter = QList<QString>());
    BBFILE* loadFolderContent(const QString &parentPath, QList<QListWidgetItem*> &newItems,
                              const QList<QString> &nameFilter = QList<QString>());
    QListWidgetItem* addFileItem(const QFileInfo &fileInfo, BBFILE *&pOutFolderContent);

    QIcon getMeshOverviewMap(const QString &sourcePath);
    void createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath);
    BBFileType getFileType(const QString &filePath);
    BBFILE* getFolderContent(QTreeWidgetItem *pItem);
    bool deleteFolderItem(QTreeWidgetItem *pItem);
    bool moveFolderItem(QTreeWidgetItem *pFolderItem, QTreeWidgetItem *pOldParentItem, QTreeWidgetItem *pNewParentItem);
    bool importFiles(const QFileInfo &fileInfo, const QString &newPath);
    bool loadImportedData(const QString &parentPath);

    /* Scene */
    bool openScene(const QString &defaultSavedParentPath, const QString &openedPath);
    bool saveScene(const QString &defaultParentPath);

    /* Material */

    BBOpenGLWidget *m_pPreviewOpenGLWidget;

    BBFILE *m_pRootFileData;
    QMap<QTreeWidgetItem*, BBFILE*> m_TopLevelFileData;
    QMap<QTreeWidgetItem*, BBFILE*> m_FileData;

    QTreeWidgetItem *m_pCurrentViewedItem;
    QList<QListWidgetItem*> m_SelectedFileItems;
};

#endif // BBFILESYSTEMDATAMANAGER_H
