#include "BBMaterialFileManager.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>
#include "Serializer/BBMaterial.pb.h"
#include "Render/BBMaterial.h"


QMap<QString, BBMaterial*> BBMaterialFileManager::m_CachedMaterials;

BBMaterialFileManager::BBMaterialFileManager()
{

}

QStringList BBMaterialFileManager::loadVShaderList()
{
    return loadShaderList("*.vert");
}

QStringList BBMaterialFileManager::loadFShaderList()
{
    return loadShaderList("*.frag");
}

void BBMaterialFileManager::saveDefaultMaterial(const QString &filePath)
{
    BBSerializer::BBMaterial material;
    material.set_shadername("diffuse");
    material.set_vshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.vert));
    material.set_fshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.frag));
    int nLength = material.ByteSizeLong() + 1;
    char szBuffer[nLength] = {0};
    material.SerializeToArray(szBuffer, nLength);
    BBUtils::saveToFile(filePath.toStdString().c_str(), szBuffer, nLength);
}

BBMaterial* BBMaterialFileManager::loadMaterial(const QString &filePath)
{
    auto it = m_CachedMaterials.find(filePath);
    if (it != m_CachedMaterials.end())
    {
        return it.value();
    }
    BBMaterial* pMaterial = new BBMaterial();
    loadMaterialContent(filePath, pMaterial);
    m_CachedMaterials.insert(filePath, pMaterial);
    return pMaterial;
}

QStringList BBMaterialFileManager::loadShaderList(const QString &filter)
{
    QStringList nameList;
    QDir dir;
    QStringList filters;
    filters << filter;
    dir.setPath(BB_PATH_RESOURCE_SHADER());
    dir.setNameFilters(filters);
    QDirIterator iter(dir, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo info = iter.fileInfo();
        if (info.isFile())
        {
            nameList.append(info.baseName());
        }
    }
    return nameList;
}

void BBMaterialFileManager::loadMaterialContent(const QString &filePath, BBMaterial *pMaterial)
{
    int nLength = -1;
    char *pData = BBUtils::loadFileContent(filePath.toStdString().c_str(), nLength);
    BB_PROCESS_ERROR_RETURN(pData);
    BBSerializer::BBMaterial material;
    material.ParseFromArray(pData, nLength);
    pMaterial->init(material.shadername().data(),
                    QString::fromStdString(material.vshaderpath()),
                    QString::fromStdString(material.fshaderpath()));
}
