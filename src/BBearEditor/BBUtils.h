#ifndef BBUTILS_H
#define BBUTILS_H

#define BB_SAFE_DELETE(p) do{ delete p; p = NULL; } while(0)
#define BB_SAFE_DELETE_ARRAY(p) do{ delete []p; p = NULL; } while(0)
#define BB_PROCESS_ERROR(p) if(!p) break
#define BB_END(p) if(p) break
#define BB_SUCCEEDED(hr) (hr>=0)

#define BB_MIMETYPE_BASEOBJECT "BaseObject"
#define BB_MIMETYPE_LIGHTOBJECT "LightObject"
#define BB_MIMETYPE_TREEWIDGET "TreeWidget"
#define BB_MIMETYPE_HIERARCHYTREEWIDGET "HierarchyTreeWidget"
#define BB_MIMETYPE_FILETREEWIDGET "FileTreeWidget"

#define BB_PATH_BASEOBJECTLIST "../../../BBearEditor/resources/xmlfiles/baselist.xml"
#define BB_PATH_RESOURCE "../../../BBearEditor/resources/"
#define BB_PATH_RESOURCE_MESH "../../../BBearEditor/resources/meshes/"
#define BB_PATH_RESOURCE_SHADER "../../../BBearEditor/resources/shaders/"
#define BB_PATH_RESOURCE_ICON "../../../BBearEditor/resources/icons/"
#define BB_PATH_TERRAIN "terrain"

#define BB_CONSTANT_UPDATE_RATE 16

#define BB_CLASSNAME_MODEL "Model"

#include <QString>
#include <QVector3D>

class BBUtils
{
public:
    static char *loadFileContent(const char *filePath, int &nFileSize);
};

#endif // BBUTILS_H
