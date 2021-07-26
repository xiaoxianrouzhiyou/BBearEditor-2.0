#include "BBRendererManager.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>
#include "Render/BBMaterial.h"
#include "Render/BBPreviewOpenGLWidget.h"
#include "Render/BBRenderPass.h"
#include "Render/BBTexture.h"


BBPreviewOpenGLWidget* BBRendererManager::m_pPreviewOpenGLWidget = nullptr;
QMap<QString, BBMaterial*> BBRendererManager::m_CachedMaterials;
BBMaterial* BBRendererManager::m_pDeferredRenderingMaterial[3] = {0};

QStringList BBRendererManager::loadVShaderList()
{
    return loadShaderList("*.vert");
}

QStringList BBRendererManager::loadFShaderList()
{
    return loadShaderList("*.frag");
}

void BBRendererManager::saveDefaultMaterial(const QString &filePath)
{
    BBSerializer::BBMaterial material;
    material.set_shadername("diffuse");
    material.set_vshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.vert));
    material.set_fshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.frag));
    serialize(material, filePath);
}

BBMaterial* BBRendererManager::loadMaterial(const QString &filePath)
{
//    auto it = m_CachedMaterials.find(filePath);
//    if (it != m_CachedMaterials.end())
//    {
//        return it.value();
//    }
    BBMaterial* pMaterial = new BBMaterial();
    loadMaterialContent(filePath, pMaterial);
    m_CachedMaterials.insert(filePath, pMaterial);
    return pMaterial;
}

QString BBRendererManager::getMaterialPath(BBMaterial *pMaterial)
{
    return m_CachedMaterials.key(pMaterial);
}

void BBRendererManager::changeVShader(BBMaterial *pMaterial, const QString &name)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);
    material.set_vshaderpath(getShaderFilePath(name).toStdString().c_str());
    serialize(material, materialPath);
}

void BBRendererManager::changeFShader(BBMaterial *pMaterial, const QString &name)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);
    material.set_fshaderpath(getShaderFilePath(name).toStdString().c_str());
    serialize(material, materialPath);
}

void BBRendererManager::changeTexture(BBMaterial *pMaterial, const QString &textureName, const QString &texturePath)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.add_texturename(textureName.toStdString().c_str());
    material.add_texturepath(texturePath.toStdString().c_str());

    serialize(material, materialPath);
}

void BBRendererManager::changeFloat(BBMaterial *pMaterial, const QString &floatName, float fValue)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.add_floatname(floatName.toStdString().c_str());
    material.add_floatvalue(fValue);

    serialize(material, materialPath);
}

QString BBRendererManager::getShaderFilePath(const QString &name)
{
    QString filePath = BB_PATH_RESOURCE_SHADER() + name;
    return filePath;
}

BBMaterial* BBRendererManager::getDeferredRenderingMaterial(int nIndex)
{
    if (!m_pDeferredRenderingMaterial[0])
    {
        createDeferredRenderingMaterial();
    }
    return m_pDeferredRenderingMaterial[nIndex];
}

BBMaterial* BBRendererManager::createCoordinateUIMaterial()
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("coordinate2D", BB_PATH_RESOURCE_SHADER(coordinate2D.vert), BB_PATH_RESOURCE_SHADER(coordinate.frag));
    pMaterial->getBaseRenderPass()->setBlendState(true);
    pMaterial->getBaseRenderPass()->setZFunc(GL_ALWAYS);
    pMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);
    return pMaterial;
}

BBMaterial* BBRendererManager::createUIMaterial()
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("UI", BB_PATH_RESOURCE_SHADER(UI.vert), BB_PATH_RESOURCE_SHADER(UI.frag));
    pMaterial->getBaseRenderPass()->setBlendState(true);
    pMaterial->setVector4(LOCATION_CANVAS, 800.0f, 600.0, 0.0f, 0.0f);
    return pMaterial;
}

BBMaterial* BBRendererManager::createStencilUIMaterial()
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("stencilUI", BB_PATH_RESOURCE_SHADER(stencilUI.vert), BB_PATH_RESOURCE_SHADER(stencilUI.frag));
    pMaterial->setBlendState(true);
    pMaterial->setZTestState(false);
    pMaterial->setZMask(false);
    pMaterial->setStencilMask(true);
    pMaterial->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return pMaterial;
}

void BBRendererManager::serialize(BBSerializer::BBMaterial material, const QString &filePath)
{
    int nLength = material.ByteSizeLong() + 1;
    char szBuffer[nLength] = {0};
    material.SerializeToArray(szBuffer, nLength);
    BBUtils::saveToFile(filePath.toStdString().c_str(), szBuffer, nLength);
}

BBSerializer::BBMaterial BBRendererManager::deserialize(const QString &filePath)
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

QStringList BBRendererManager::loadShaderList(const QString &filter)
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

void BBRendererManager::loadMaterialContent(const QString &filePath, BBMaterial *pMaterial)
{
    BBSerializer::BBMaterial material = deserialize(filePath);
    pMaterial->init(material.shadername().data(),
                    QString::fromStdString(material.vshaderpath()),
                    QString::fromStdString(material.fshaderpath()));

    // render state
    pMaterial->setBlendState(true);
    pMaterial->setBlendFunc(GL_ONE, GL_ONE);

    // uniform
    int nTextureCount = material.texturename_size();
    for (int i = 0; i < nTextureCount; i++)
    {
        BBTexture texture;
        QString texturePath = QString::fromStdString(material.texturepath(i));
        pMaterial->setSampler2D(material.texturename(i), texture.createTexture2D(texturePath), texturePath);
    }

    int nFloatCount = material.floatname_size();
    for (int i = 0; i < nFloatCount; i++)
    {
        pMaterial->setFloat(material.floatname(i), material.floatvalue(i));
    }

    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    pMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);
}

void BBRendererManager::createDeferredRenderingMaterial()
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
