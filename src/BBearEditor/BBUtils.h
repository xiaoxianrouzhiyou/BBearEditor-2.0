#ifndef BBUTILS_H
#define BBUTILS_H

#define BB_SAFE_DELETE(p) do{ delete p; p = NULL;} while(0)
#define BB_SAFE_DELETE_ARRAY(p) do{ delete []p; p = NULL;} while(0)
#define BB_PROCESS_ERROR(p) if(!p) break
#define BB_SUCCEEDED(hr) ((HRESULT)(hr)>=0)

#define BB_MIMETYPE_BASEOBJECT "BaseObject"
#define BB_MIMETYPE_LIGHTOBJECT "LightObject"
#define BB_MIMETYPE_TREEWIDGET "TreeWidget"
#define BB_MIMETYPE_HIERARCHYTREEWIDGET "HierarchyTreeWidget"
#define BB_MIMETYPE_FILETREEWIDGET "FileTreeWidget"

#define BB_PATH_BASEOBJECTLIST "../BBearEditor/xmlfiles/baselist.xml"
#define BB_PATH_RESOURCE_MESH "../BBearEditor/resources/meshes/"
#define BB_PATH_RESOURCE_ICON "://resources/icons/"
#define BB_PATH_TERRAIN "terrain"

class BBUtils
{
public:
    BBUtils();
    ~BBUtils();
};

#endif // BBUTILS_H
