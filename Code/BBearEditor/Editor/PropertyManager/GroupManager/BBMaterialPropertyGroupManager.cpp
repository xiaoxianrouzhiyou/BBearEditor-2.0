#include "BBMaterialPropertyGroupManager.h"
#include "Utils/BBUtils.h"
#include "Render/Texture/BBTexture.h"
#include "Render/BBPreviewOpenGLWidget.h"
#include "Scene/BBRendererManager.h"
#include "Render/BBMaterial.h"
#include <QCheckBox>
#include "../BBPropertyFactory.h"
#include "Render/BBMaterialProperty.h"


BBMaterialPropertyGroupManager::BBMaterialPropertyGroupManager(BBMaterial *pMaterial,
                                                               BBPreviewOpenGLWidget *pPreviewOpenGLWidget,
                                                               QWidget *pParent)
    : BBGroupManager("Main", BB_PATH_RESOURCE_ICON(render.png), pParent)
{
    m_pMaterial = pMaterial;
    m_pPreviewOpenGLWidget = pPreviewOpenGLWidget;

    addBlendStateItem();
    addBlendFuncItem();
    addMargin(10);
    // read BBMaterialProperty
    addPropertyItems();
}

BBMaterialPropertyGroupManager::~BBMaterialPropertyGroupManager()
{

}

void BBMaterialPropertyGroupManager::addBlendStateItem()
{
    QCheckBox *pBlendState = new QCheckBox(this);
    addFactory("Blend State", pBlendState, 1, Qt::AlignRight);

    // original value
    bool bBlendState = m_pMaterial->getBlendState();
    pBlendState->setChecked(bBlendState);
    QObject::connect(pBlendState, SIGNAL(clicked(bool)), this, SLOT(enableBlendState(bool)));
}

void BBMaterialPropertyGroupManager::addBlendFuncItem()
{
    QStringList items = {"GL_ZERO",
                         "GL_ONE",
                         "GL_SRC_COLOR",
                         "GL_ONE_MINUS_SRC_COLOR",
                         "GL_SRC_ALPHA",
                         "GL_ONE_MINUS_SRC_ALPHA",
                         "GL_DST_ALPHA",
                         "GL_ONE_MINUS_DST_ALPHA"};
    BBEnumFactory *pSRCBlendFunc = new BBEnumFactory("src", items, BBUtils::getBlendFuncName(m_pMaterial->getSRCBlendFunc()), this);
    BBEnumFactory *pDSTBlendFunc = new BBEnumFactory("dst", items, BBUtils::getBlendFuncName(m_pMaterial->getDSTBlendFunc()), this);
    QObject::connect(pSRCBlendFunc, SIGNAL(currentItemChanged(int)), this, SLOT(switchSRCBlendFunc(int)));
    QObject::connect(pDSTBlendFunc, SIGNAL(currentItemChanged(int)), this, SLOT(switchDSTBlendFunc(int)));
    QWidget *pWidget = addFactory("Blend Func", pSRCBlendFunc, pDSTBlendFunc);
    pWidget->setVisible(m_pMaterial->getBlendState());
}

void BBMaterialPropertyGroupManager::enableBlendState(bool bEnable)
{
    m_pMaterial->setBlendState(bEnable);
    QWidget* pWidget = findChild<QWidget*>("Blend Func");
    if (pWidget)
    {
        m_pContainer->setVisible(false);
        pWidget->setVisible(bEnable);
        m_pContainer->setVisible(true);
    }
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeBlendState(m_pMaterial, bEnable);
}

void BBMaterialPropertyGroupManager::switchSRCBlendFunc(int nIndex)
{
    m_pMaterial->setSRCBlendFunc(BBUtils::getBlendFunc(nIndex));
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeSRCBlendFunc(m_pMaterial, nIndex);
}

void BBMaterialPropertyGroupManager::switchDSTBlendFunc(int nIndex)
{
    m_pMaterial->setDSTBlendFunc(BBUtils::getBlendFunc(nIndex));
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeDSTBlendFunc(m_pMaterial, nIndex);
}

void BBMaterialPropertyGroupManager::addPropertyItems()
{
    QList<std::string> names;
    QList<BBMaterialProperty*> properties;
    m_pMaterial->getEditableProperties(names, properties);
    for (int i = 0; i < properties.count(); i++)
    {
        BBMaterialUniformPropertyType eType = properties[i]->getType();
        QString name = QString::fromStdString(names[i]);
        if (eType == BBMaterialUniformPropertyType::Vector4)
        {
            BBVector4MaterialProperty *pProperty = (BBVector4MaterialProperty*)properties[i];
            BBVector4MaterialPropertyFactoryType eFactoryType = pProperty->getFactoryType();
            if (eFactoryType == BBVector4MaterialPropertyFactoryType::Color)
            {
                const float *color = pProperty->getPropertyValue();
                BBColorFactory *pColorFactory = new BBColorFactory(names[i], color[0], color[1], color[2], color[3], this);
                addFactory(pProperty->getNameInPropertyManager(), pColorFactory, 1);
                QObject::connect(pColorFactory, SIGNAL(colorChanged(float, float, float, float, std::string)),
                                 this, SLOT(setColor(float, float, float, float, std::string)));
            }
            else
            {

            }
        }
        else if (eType == BBMaterialUniformPropertyType::Float)
        {
            BBFloatMaterialProperty *pProperty = (BBFloatMaterialProperty*)properties[i];
            BBLineEditFactory *pFloatFactory = addFactory(name, pProperty->getPropertyValue());
            pFloatFactory->setSlideStep(0.005f);
            pFloatFactory->setRange(0, 1);
            QObject::connect(pFloatFactory, SIGNAL(valueChanged(QString, float)), this, SLOT(setFloat(QString, float)));
        }
        else if (eType == BBMaterialUniformPropertyType::Sampler2D)
        {
            BBSampler2DMaterialProperty *pProperty = (BBSampler2DMaterialProperty*)properties[i];
            BBTextureFactory *pTextureFactory = new BBTextureFactory(name, pProperty->getResourcePath(), this);
            addFactory(name, pTextureFactory, 3);
            QObject::connect(pTextureFactory, SIGNAL(setSampler2D(QString, QString)), this, SLOT(setSampler2D(QString, QString)));

            // Find whether there are matching tiling and offset items
            QString key = LOCATION_TILINGANDOFFSET_PREFIX + name;
            int nIndex = names.indexOf(key.toStdString().c_str());
            if (nIndex >= 0)
            {
                // whether the matching item is a TilingAndOffset, otherwise it will be invalid
                BBMaterialProperty *pMatchingProperty = properties[nIndex];
                if (pMatchingProperty->getType() == BBMaterialUniformPropertyType::Vector4)
                {
                    BBVector4MaterialProperty *pVecProperty = (BBVector4MaterialProperty*)pMatchingProperty;
                    BBVector4MaterialPropertyFactoryType eFactoryType = pVecProperty->getFactoryType();
                    if (eFactoryType == BBVector4MaterialPropertyFactoryType::TilingAndOffset)
                    {
                        pTextureFactory->enableTilingAndOffset(true);
                        const float *pValue = pVecProperty->getPropertyValue();
                        pTextureFactory->setTiling(pValue[0], pValue[1]);
                        pTextureFactory->setOffset(pValue[2], pValue[3]);

                        QObject::connect(pTextureFactory, SIGNAL(setTilingAndOffset(QString, float, float, float, float)),
                                         this, SLOT(setTilingAndOffset(QString, float, float, float, float)));
                    }
                }
            }
        }
        else if (eType == BBMaterialUniformPropertyType::SamplerCube)
        {
            BBSamplerCubeMaterialProperty *pProperty = (BBSamplerCubeMaterialProperty*)properties[i];
            BBCubeMapFactory *pCubeMapFactory = new BBCubeMapFactory(name, pProperty->getResourcePaths(), this);
            addFactory(pCubeMapFactory);
            QObject::connect(pCubeMapFactory, SIGNAL(setSamplerCube(QString, QString*)),
                             this, SLOT(setSamplerCube(QString, QString*)));
        }
    }
}

void BBMaterialPropertyGroupManager::setSampler2D(const QString &uniformName, const QString &texturePath)
{
    BBTexture texture;
    m_pMaterial->setSampler2D(uniformName.toStdString().c_str(), texture.createTexture2D(texturePath), texturePath);
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeSampler2D(m_pMaterial, uniformName, texturePath);
}

void BBMaterialPropertyGroupManager::setTilingAndOffset(const QString &uniformName, float fTilingX, float fTilingY, float fOffsetX, float fOffsetY)
{
    m_pMaterial->setVector4(uniformName.toStdString().c_str(), fTilingX, fTilingY, fOffsetX, fOffsetY);
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    float *ptr = new float[4] {fTilingX, fTilingY, fOffsetX, fOffsetY};
    BBRendererManager::changeVector4(m_pMaterial, uniformName.toStdString().c_str(), ptr);
}

void BBMaterialPropertyGroupManager::setSamplerCube(const QString &uniformName, QString *pResourcePaths)
{
    BBTexture texture;
    m_pMaterial->setSamplerCube(uniformName.toStdString().c_str(), texture.createTextureCube(pResourcePaths), pResourcePaths);
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeSamplerCube(m_pMaterial, uniformName, pResourcePaths);
}

void BBMaterialPropertyGroupManager::setFloat(const QString &uniformName, float fValue)
{
    m_pMaterial->setFloat(uniformName.toStdString().c_str(), fValue);
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    BBRendererManager::changeFloat(m_pMaterial, uniformName, fValue);
}

void BBMaterialPropertyGroupManager::setColor(float r, float g, float b, float a, const std::string &uniformName)
{
    m_pMaterial->setVector4(uniformName, r, g, b, a);
    m_pPreviewOpenGLWidget->updateMaterialSphere(m_pMaterial);
    float *ptr = new float[4] {r, g, b, a};
    BBRendererManager::changeVector4(m_pMaterial, uniformName, ptr);
}
