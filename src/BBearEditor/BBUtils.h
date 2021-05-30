#ifndef BBUTILS_H
#define BBUTILS_H

#define BB_SAFE_DELETE(p) do{ delete p; p = NULL; } while(0)
#define BB_SAFE_DELETE_ARRAY(p) do{ delete []p; p = NULL; } while(0)
#define BB_PROCESS_ERROR(p) if(!p) break
#define BB_PROCESS_ERROR_RETURN(p) if(!p) return
#define BB_PROCESS_ERROR_RETURN_FALSE(p) if(!p) return false
#define BB_END(p) if(p) break
#define BB_SUCCEEDED(hr) (hr>=0)

#define BB_MIMETYPE_BASEOBJECT "BaseObject"
#define BB_MIMETYPE_LIGHTOBJECT "LightObject"
#define BB_MIMETYPE_TREEWIDGET "TreeWidget"
#define BB_MIMETYPE_HIERARCHYTREEWIDGET "HierarchyTreeWidget"
#define BB_MIMETYPE_FOLDERTREEWIDGET "FolderTreeWidget"
#define BB_MIMETYPE_FILELISTWIDGET "FileListWidget"

#define BB_PATH_BASEOBJECTLIST "../../../BBearEditor/resources/xmlfiles/baselist.xml"
#define BB_PATH_RESOURCE "../../../BBearEditor/resources/"
#define BB_PATH_RESOURCE_MESH "../../../BBearEditor/resources/meshes/"
#define BB_PATH_RESOURCE_SHADER "../../../BBearEditor/resources/shaders/"
#define BB_PATH_RESOURCE_ICON "../../../BBearEditor/resources/icons/"
#define BB_PATH_TERRAIN "terrain"

#define BB_CONSTANT_UPDATE_RATE 16

#define BB_CLASSNAME_MODEL "Model"

#define BB_USER_NAME "BigBugBear"
#define BB_USER_PROJECT_NAME "BBearEditor"

#include <QString>
#include <QVector3D>
#include <QDebug>


enum BBFileType
{
    dir = 0,
    mesh = 1,
    audio = 2,
    texture = 3,
    material = 4,
    script = 5,
    other = 6
};


class QFileInfo;

class BBConstant
{
public:
    static QString BB_NAME_PROJECT;
    static QString BB_PATH_PROJECT;
    static QString BB_PATH_PROJECT_ENGINE;
    static QString BB_PATH_PROJECT_USER;
    static QString BB_PATH_RESOURCE_PICTURE;
    static QString BB_NAME_FILE_SYSTEM_USER;
    static QString BB_NAME_FILE_SYSTEM_ENGINE;
    static QString BB_NAME_OVERVIEW_MAP;
};

class BBUtils
{
public:
    static char *loadFileContent(const char *filePath, int &nFileSize);
    static QString getExclusiveFolderPath(const QString &parentPath, QString &fileName);
    static QString getExclusiveFolderPath(const QString &filePath);
    static QString getExclusiveFilePath(const QString &parentPath, QString &fileName);
    static QString getExclusiveFilePath(const QString &filePath);
    static QString getFileSuffix(const QFileInfo &fileInfo);
    static QString getFileSuffix(const QString &name);
    static QString getBaseName(const QString &name);
    static QString getFileNameByPath(const QString &filePath);
    static QString getParentPath(const QString &filePath);
    static QString getEngineAuxiliaryFolderPath(const QString &sourcePath);
    static QString getOverviewMapPath(const QString &sourcePath);
    static bool showInFolder(const QString &filePath);
    static bool copyFolder(const QString &fromDir, const QString &toDir);
    static bool isMovablePath(const QString &sourcePath, const QString &destParentPath);
    static bool moveFolder(const QString &oldPath, const QString &newPath, bool bCopy = false);
    static bool moveFile(const QString &oldPath, const QString &newPath, BBFileType eFileType, bool bCopy = false);
};

#endif // BBUTILS_H
