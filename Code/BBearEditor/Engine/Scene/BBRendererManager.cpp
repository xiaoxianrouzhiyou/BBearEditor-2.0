#include "BBRendererManager.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>
#include "Render/BBMaterial.h"
#include "Render/BBPreviewOpenGLWidget.h"
#include "Render/BBRenderPass.h"
#include "Render/Texture/BBTexture.h"
#include <QUuid>


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
    QUuid id = QUuid::createUuid();
    material.set_shadername(id.toString().toStdString().c_str());
    material.set_vshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.vert));
    material.set_fshaderpath(BB_PATH_RESOURCE_SHADER(diffuse.frag));
    serialize(material, filePath);
}

BBMaterial* BBRendererManager::loadMaterial(const QString &filePath)
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

void BBRendererManager::changeSampler2D(BBMaterial *pMaterial, const QString &uniformName, const QString &texturePath)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.add_texturename(uniformName.toStdString().c_str());
    material.add_texturepath(texturePath.toStdString().c_str());

    serialize(material, materialPath);
}

void BBRendererManager::changeSamplerCube(BBMaterial *pMaterial, const QString &uniformName, const QString resourcePaths[])
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_cubemapname(uniformName.toStdString().c_str());
    BBSerializer::BBCubeMap *pCubeMap = material.mutable_cubemappaths();
    pCubeMap->set_positivex(resourcePaths[0].toStdString().c_str());
    pCubeMap->set_negativex(resourcePaths[1].toStdString().c_str());
    pCubeMap->set_positivey(resourcePaths[2].toStdString().c_str());
    pCubeMap->set_negativey(resourcePaths[3].toStdString().c_str());
    pCubeMap->set_positivez(resourcePaths[4].toStdString().c_str());
    pCubeMap->set_negativez(resourcePaths[5].toStdString().c_str());

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

void BBRendererManager::changeVector4(BBMaterial *pMaterial, const std::string &name, float *fValue)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.add_vec4name(name);
    BBSerializer::BBVector4f *pVec4 = material.add_vec4value();
    pVec4->set_x(fValue[0]);
    pVec4->set_y(fValue[1]);
    pVec4->set_z(fValue[2]);
    pVec4->set_w(fValue[3]);

    serialize(material, materialPath);
}

void BBRendererManager::changeBlendState(BBMaterial *pMaterial, bool bEnable)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_blendstate(bEnable);

    serialize(material, materialPath);
}

void BBRendererManager::changeSRCBlendFunc(BBMaterial *pMaterial, int src)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_srcblendfunc(src);

    serialize(material, materialPath);
}

void BBRendererManager::changeDSTBlendFunc(BBMaterial *pMaterial, int dst)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_dstblendfunc(dst);

    serialize(material, materialPath);
}

void BBRendererManager::changeCullState(BBMaterial *pMaterial, bool bEnable)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_cullstate(bEnable);

    serialize(material, materialPath);
}

void BBRendererManager::changeCullFace(BBMaterial *pMaterial, int face)
{
    QString materialPath = m_CachedMaterials.key(pMaterial);
    BB_PROCESS_ERROR_RETURN(!materialPath.isEmpty());
    BBSerializer::BBMaterial material = deserialize(materialPath);

    material.set_cullface(face);

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
    pMaterial->setVector4(LOCATION_CAMERA_PARAMETERS0, 800.0f, 600.0f, 0.0f, 0.0f);
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
            // relative to BB_PATH_RESOURCE_SHADER
            QString name = QDir(BB_PATH_RESOURCE_SHADER()).relativeFilePath(info.absolutePath());
            if (name.length() == 1)
            {
                // in the shader dir root
                name = info.baseName();
            }
            else
            {
                name = name + "/" + info.baseName();
            }
            nameList.append(name);
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
    pMaterial->setBlendState(material.blendstate());
    pMaterial->setBlendFunc(BBUtils::getBlendFunc(material.srcblendfunc()), BBUtils::getBlendFunc(material.dstblendfunc()));

    pMaterial->setCullState(material.cullstate());
    pMaterial->setCullFace(material.cullface());

    // uniform
    int nTextureCount = material.texturename_size();
    for (int i = 0; i < nTextureCount; i++)
    {
        BBTexture texture;
        QString texturePath = QString::fromStdString(material.texturepath(i));
        pMaterial->setSampler2D(material.texturename(i), texture.createTexture2D(texturePath), texturePath);
    }

    BBTexture texture;
    BBSerializer::BBCubeMap cubeMapPath = material.cubemappaths();
    QString paths[6] = {QString::fromStdString(cubeMapPath.positivex()),
                        QString::fromStdString(cubeMapPath.negativex()),
                        QString::fromStdString(cubeMapPath.positivey()),
                        QString::fromStdString(cubeMapPath.negativey()),
                        QString::fromStdString(cubeMapPath.positivez()),
                        QString::fromStdString(cubeMapPath.negativez())};
    pMaterial->setSamplerCube(material.cubemapname(), texture.createTextureCube(paths), paths);

    int nFloatCount = material.floatname_size();
    for (int i = 0; i < nFloatCount; i++)
    {
        pMaterial->setFloat(material.floatname(i), material.floatvalue(i));
    }

    int nVec4Count = material.vec4name_size();
    for (int i = 0; i < nVec4Count; i++)
    {
        BBSerializer::BBVector4f vec4 = material.vec4value(i);
        pMaterial->setVector4(material.vec4name(i), vec4.x(), vec4.y(), vec4.z(), vec4.w());
    }

    // test
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
