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
#define BB_PATH_LIGHTOBJECTLIST "../../../BBearEditor/resources/xmlfiles/lightlist.xml"
#define BB_PATH_RESOURCE "../../../BBearEditor/resources/"
#define BB_PATH_RESOURCE_MESH(x) "../../../BBearEditor/resources/meshes/"#x
#define BB_PATH_RESOURCE_SHADER(x) "../../../BBearEditor/resources/shaders/"#x
#define BB_PATH_RESOURCE_ICON(x) "../../../BBearEditor/resources/icons/"#x
#define BB_PATH_RESOURCE_PICTURE(x) "../../../BBearEditor/resources/pictures/"#x
#define BB_PATH_TERRAIN "terrain"

#define BB_STYLE_IMAGE(x) "image: url(" x ");"

#define BB_CONSTANT_UPDATE_RATE 16

#define BB_CLASSNAME_MODEL "Model"
#define BB_CLASSNAME_MESH "Mesh"
#define BB_CLASSNAME_DIRECTIONAL_LIGHT "DirectionalLight"
#define BB_CLASSNAME_POINT_LIGHT "PointLight"
#define BB_CLASSNAME_SPOT_LIGHT "SpotLight"
#define BB_CLASSNAME_CANVAS "Canvas"

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
        Scene = 4,
        Material = 5,
        Script = 6,
        Other = 7
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


/**
 * @brief The BBStatic class        Ensure that static variables have been initialized before being used
 */
class BBStatic
{
public:
    template<int N, class T>
    static T& var()
    {
        static T instance;
        return instance;
    }

    ~BBStatic() {}
};


class BBConstant
{
public:
    static QString BB_NAME_PROJECT;
    static QString BB_PATH_PROJECT;
    static QString BB_PATH_PROJECT_ENGINE;
    static QString BB_PATH_PROJECT_USER;
    static QString BB_NAME_FILE_SYSTEM_USER;
    static QString BB_NAME_FILE_SYSTEM_ENGINE;
    static QString BB_NAME_OVERVIEW_MAP;
    static QString BB_NAME_DEFAULT_SCENE;
    static QString BB_NAME_DEFAULT_MATERIAL;
};

class BBUtils
{
public:
    static char *loadFileContent(const char *filePath, int &nFileSize);
    static bool saveToFile(const char *pFilePath, void *pBuffer, int nSize);
    static unsigned char* decodeBMP(unsigned char *pBmpFileData, int &nWidth, int &nHeight);
};

#endif // BBUTILS_H
