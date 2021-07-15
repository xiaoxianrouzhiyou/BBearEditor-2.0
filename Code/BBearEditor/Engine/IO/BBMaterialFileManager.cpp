#include "BBMaterialFileManager.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>
#include "Render/BBMaterial.h"
#include "Render/BBPreviewOpenGLWidget.h"
#include "Render/BBRenderPass.h"


BBPreviewOpenGLWidget* BBMaterialFileManager::m_pPreviewOpenGLWidget = nullptr;
QMap<QString, BBMaterial*> BBMaterialFileManager::m_CachedMaterials;
BBMaterial* BBMaterialFileManager::m_pDeferredRenderingMaterial[3] = {0};
BBMaterial* BBMaterialFileManager::m_pCoordinateUIMaterial = nullptr;
BBMaterial* BBMaterialFileManager::m_pUIMaterial = nullptr;

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

BBMaterial* BBMaterialFileManager::getDeferredRenderingMaterial(int nIndex)
{
    if (!m_pDeferredRenderingMaterial[0])
    {
        createDeferredRenderingMaterial();
    }
    return m_pDeferredRenderingMaterial[nIndex];
}

BBMaterial* BBMaterialFileManager::getCoordinateUIMaterial()
{
    if (!m_pCoordinateUIMaterial)
    {
        createCoordinateUIMaterial();
    }
    return m_pCoordinateUIMaterial->clone();
}

BBMaterial* BBMaterialFileManager::getUIMaterial()
{
    if (!m_pUIMaterial)
    {
        createUIMaterial();
    }
    return m_pUIMaterial->clone();
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
    pMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);
}

void BBMaterialFileManager::createDeferredRenderingMaterial()
{
    m_pDeferredRenderingMaterial[0] = new BBMaterial();
    m_pDeferredRenderingMaterial[0]->init("DeferredPosition",
                                          BB_PATH_RESOURCE_SHADER(DeferredPosition.vert),
                                          BB_PATH_RESOURCE_SHADER(DeferredPosition.frag));
    m_pDeferredRenderingMaterial[1] = new BBMaterial();
    m_pDeferredRenderingMaterial[1]->init("DeferredNormal",
                                          BB_PATH_RESOURCE_SHADER(DeferredNormal.vert),
                                          BB_PATH_RESOURCE_SHADER(DeferredNormal.frag));
    m_pDeferredRenderingMaterial[2] = new BBMaterial();
    m_pDeferredRenderingMaterial[2]->init("DeferredColor",
                                          BB_PATH_RESOURCE_SHADER(DeferredColor.vert),
                                          BB_PATH_RESOURCE_SHADER(DeferredColor.frag));
    // default
    float *pTextureSettings = new float[4] {0.0f, 0.0f, 0.0f, 0.0f};
    m_pDeferredRenderingMaterial[2]->setVector4(LOCATION_TEXTURE_SETTING0, pTextureSettings);
}

void BBMaterialFileManager::createCoordinateUIMaterial()
{
    m_pCoordinateUIMaterial = new BBMaterial();
    m_pCoordinateUIMaterial->init("coordinate2D",
                                  BB_PATH_RESOURCE_SHADER(coordinate2D.vert),
                                  BB_PATH_RESOURCE_SHADER(coordinate.frag));
    m_pCoordinateUIMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCoordinateUIMaterial->getBaseRenderPass()->setZFunc(GL_ALWAYS);
    m_pCoordinateUIMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);
}

void BBMaterialFileManager::createUIMaterial()
{
    m_pUIMaterial = new BBMaterial();
    m_pUIMaterial->init("UI", BB_PATH_RESOURCE_SHADER(UI.vert), BB_PATH_RESOURCE_SHADER(UI.frag));
    m_pUIMaterial->getBaseRenderPass()->setBlendState(true);
    m_pUIMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);
}
