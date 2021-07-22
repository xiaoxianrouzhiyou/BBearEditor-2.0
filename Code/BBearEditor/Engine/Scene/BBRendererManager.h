#ifndef BBRENDERERMANAGER_H
#define BBRENDERERMANAGER_H


#include <QStringList>
#include "Serializer/BBMaterial.pb.h"

class BBMaterial;
class BBPreviewOpenGLWidget;

class BBRendererManager
{
public:
    static void bindPreviewOpenGLWidget(BBPreviewOpenGLWidget *pPreviewOpenGLWidget) { m_pPreviewOpenGLWidget = pPreviewOpenGLWidget; }
    static QStringList loadVShaderList();
    static QStringList loadFShaderList();
    static void saveDefaultMaterial(const QString &filePath);
    static BBMaterial* loadMaterial(const QString &filePath);
    static QString getMaterialPath(BBMaterial *pMaterial);

    /* change cached materials and save changes */
    static void changeVShader(BBMaterial *pMaterial, const QString &name);
    static void changeFShader(BBMaterial *pMaterial, const QString &name);
    static void changeTexture(BBMaterial *pMaterial, const QString &texturePath);

    static QString getShaderFilePath(const QString &name);

    static BBMaterial* getDeferredRenderingMaterial(int nIndex);
    static BBMaterial* createCoordinateUIMaterial();
    static BBMaterial* createUIMaterial();
    static BBMaterial* createStencilUIMaterial();

private:
    static void serialize(BBSerializer::BBMaterial material, const QString &filePath);
    static BBSerializer::BBMaterial deserialize(const QString &filePath);

    static QStringList loadShaderList(const QString &filter);
    static void loadMaterialContent(const QString &filePath, BBMaterial *pMaterial);

    static void createDeferredRenderingMaterial();

private:
    static BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
    static QMap<QString, BBMaterial*> m_CachedMaterials;

    static BBMaterial *m_pDeferredRenderingMaterial[3];
};

#endif // BBRENDERERMANAGER_H
