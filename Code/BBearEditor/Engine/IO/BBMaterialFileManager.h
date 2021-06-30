#ifndef BBMATERIALFILEMANAGER_H
#define BBMATERIALFILEMANAGER_H


#include <QStringList>
#include "Serializer/BBMaterial.pb.h"

class BBMaterial;
class BBPreviewOpenGLWidget;

class BBMaterialFileManager
{
public:
    static void bindPreviewOpenGLWidget(BBPreviewOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    static QStringList loadVShaderList();
    static QStringList loadFShaderList();
    static void saveDefaultMaterial(const QString &filePath);
    static BBMaterial* loadMaterial(const QString &filePath);
    static QString getMaterialPath(BBMaterial *pMaterial);

public:
    /* change cached materials and save changes */
    static void changeVShader(BBMaterial *pMaterial, const QString &name);
    static void changeFShader(BBMaterial *pMaterial, const QString &name);

private:
    static void serialize(BBSerializer::BBMaterial material, const QString &filePath);
    static BBSerializer::BBMaterial deserialize(const QString &filePath);

    static std::string getShaderFilePath(const QString &name);

    static QStringList loadShaderList(const QString &filter);
    static void loadMaterialContent(const QString &filePath, BBMaterial *pMaterial);

private:
    static BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
    static QMap<QString, BBMaterial*> m_CachedMaterials;
};

#endif // BBMATERIALFILEMANAGER_H
