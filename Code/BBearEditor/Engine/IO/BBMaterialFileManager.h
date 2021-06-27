#ifndef BBMATERIALFILEMANAGER_H
#define BBMATERIALFILEMANAGER_H


#include <QStringList>

class BBMaterial;

class BBMaterialFileManager
{
public:
    BBMaterialFileManager();

public:
    static QStringList loadVShaderList();
    static QStringList loadFShaderList();
    static void saveDefaultMaterial(const QString &filePath);
    static BBMaterial* loadMaterial(const QString &filePath);

private:
    static QStringList loadShaderList(const QString &filter);
    static void loadMaterialContent(const QString &filePath, BBMaterial *pMaterial);

private:
    static QMap<QString, BBMaterial*> m_CachedMaterials;
};

#endif // BBMATERIALFILEMANAGER_H
