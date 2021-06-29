#ifndef BBMATERIALFILEMANAGER_H
#define BBMATERIALFILEMANAGER_H


#include <QStringList>

class BBMaterial;
class BBPreviewOpenGLWidget;

class BBMaterialFileManager
{
public:
    BBMaterialFileManager();

public:
    static void bindPreviewOpenGLWidget(BBPreviewOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    static QStringList loadVShaderList();
    static QStringList loadFShaderList();
    static void saveDefaultMaterial(const QString &filePath);
    static BBMaterial* loadMaterial(const QString &filePath);

private:
    static QStringList loadShaderList(const QString &filter);
    static void loadMaterialContent(const QString &filePath, BBMaterial *pMaterial);

private:
    static BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
    static QMap<QString, BBMaterial*> m_CachedMaterials;
};

#endif // BBMATERIALFILEMANAGER_H
