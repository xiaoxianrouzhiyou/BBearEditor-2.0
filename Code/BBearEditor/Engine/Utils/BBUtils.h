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
#define BB_PATH_RESOURCE_TEXTURE(x) "../../../BBearEditor/resources/textures/"#x
#define BB_PATH_RESOURCE_MATERIAL "../../../BBearEditor/resources/materials/"
#define BB_PATH_TERRAIN "terrain"

#define BB_STYLE_IMAGE(x) "image: url(" x ");"

#define BB_CONSTANT_UPDATE_RATE 16

#define BB_CLASSNAME_MODEL "Model"
#define BB_CLASSNAME_MESH "Mesh"
#define BB_CLASSNAME_TERRAIN "Terrain"
#define BB_CLASSNAME_PARTICLE "Particle"
#define BB_CLASSNAME_DIRECTIONAL_LIGHT "DirectionalLight"
#define BB_CLASSNAME_POINT_LIGHT "PointLight"
#define BB_CLASSNAME_SPOT_LIGHT "SpotLight"
#define BB_CLASSNAME_CANVAS "Canvas"
#define BB_CLASSNAME_SPRITEOBJECT2D "SpriteObject2D"

#define BB_USER_NAME "BigBugBear"
#define BB_USER_PROJECT_NAME "BBearEditor"

#include <QString>
#include <QVector3D>
#include <QVector4D>
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

enum BBAxisName
{
    AxisNULL = 0x00,
    AxisX = 0x01,
    AxisY = 0x02,
    AxisZ = 0x04
};


// Type of bit operation result
Q_DECLARE_FLAGS(BBAxisFlags, BBAxisName)
// override | operator
Q_DECLARE_OPERATORS_FOR_FLAGS(BBAxisFlags)


enum BBCoordinateSystemSpaceMode
{
    Space2D = 0,
    Space3D = 1
};


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

    static QVector3D m_Red;
    static QVector4D m_RedTransparency;
    static QVector3D m_Green;
    static QVector4D m_GreenTransparency;
    static QVector3D m_Blue;
    static QVector4D m_BlueTransparency;
    static QVector3D m_Yellow;
    static QVector3D m_Gray;
    static QVector4D m_GrayTransparency;
    static QVector3D m_OrangeRed;
    static QVector3D m_LightGreen;
};

class BBUtils
{
public:
    static char *loadFileContent(const char *filePath, int &nFileSize);
    static bool saveToFile(const char *pFilePath, void *pBuffer, int nSize);
    static unsigned char* decodeBMP(unsigned char *pBmpFileData, int &nWidth, int &nHeight);

    static QString getBaseName(const QString &name);

    static QString getPathRelativeToExecutionDirectory(const QString &absolutePath);

    static unsigned int getBlendFunc(int nIndex);
    static QString getBlendFuncName(unsigned int func);
};

#endif // BBUTILS_H
