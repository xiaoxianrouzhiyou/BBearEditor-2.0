#include "BBMaterialFileManager.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>
#include "Render/BBMaterial.h"
#include "Render/BBPreviewOpenGLWidget.h"


BBPreviewOpenGLWidget* BBMaterialFileManager::m_pPreviewOpenGLWidget = nullptr;
QMap<QString, BBMaterial*> BBMaterialFileManager::m_CachedMaterials;
BBMaterial* BBMaterialFileManager::m_pDeferredRenderingMaterial = nullptr;

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
    serialize(material, filePath);
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

QString BBMaterialFileManager::getMaterialPath(BBMaterial *pMaterial)
{
    return m_CachedMaterials.key(pMaterial);
}

void BBMaterialFileManager::changeVShader(BBMaterial *pMaterial, const QString &name)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);
    material.set_vshaderpath(getShaderFilePath(name).toStdString().c_str());
    serialize(material, materialPath);
}

void BBMaterialFileManager::changeFShader(BBMaterial *pMaterial, const QString &name)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);
    material.set_fshaderpath(getShaderFilePath(name).toStdString().c_str());
    serialize(material, materialPath);
}

QString BBMaterialFileManager::getShaderFilePath(const QString &name)
{
    QString filePath = BB_PATH_RESOURCE_SHADER() + name;
    return filePath;
}

BBMaterial* BBMaterialFileManager::getDeferredRenderingMaterial()
{
    if (!m_pDeferredRenderingMaterial)
    {
        createDeferredRenderingMaterial();
    }
    return m_pDeferredRenderingMaterial;
}

void BBMaterialFileManager::serialize(BBSerializer::BBMaterial material, const QString &filePath)
{
    int nLength = material.ByteSizeLong() + 1;
    char szBuffer[nLength] = {0};
    material.SerializeToArray(szBuffer, nLength);
    BBUtils::saveToFile(filePath.toStdString().c_str(), szBuffer, nLength);
}

BBSerializer::BBMaterial BBMaterialFileManager::deserialize(const QString &filePath)
{
    BBSerializer::BBMaterial material;
    int nLength = -1;
    char *pData = BBUtils::loadFileContent(filePath.toStdString().c_str(), nLength);
    if (!pData)
        return material;
    material.ParseFromArray(pData, nLength);
    BB_SAFE_DELETE_ARRAY(pData);
    return material;
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
    BBSerializer::BBMaterial material = deserialize(filePath);
    pMaterial->init(material.shadername().data(),
                    QString::fromStdString(material.vshaderpath()),
                    QString::fromStdString(material.fshaderpath()));
    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    pMaterial->setVector4(NAME_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(NAME_LIGHT_COLOR, pLightColor);
}

void BBMaterialFileManager::createDeferredRenderingMaterial()
{
    m_pDeferredRenderingMaterial = new BBMaterial();
    m_pDeferredRenderingMaterial->init("DefferedPosition",
                                       BB_PATH_RESOURCE_SHADER(DefferedPosition.vert),
                                       BB_PATH_RESOURCE_SHADER(DefferedPosition.frag));
}
