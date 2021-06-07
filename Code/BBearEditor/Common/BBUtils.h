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


class QTreeWidgetItem;
class QListWidgetItem;


namespace BBFileSystem
{
    enum BBSignalSender
    {
        FolderTree = 0,
        FileList = 1
    };

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

}


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
};

#endif // BBUTILS_H
