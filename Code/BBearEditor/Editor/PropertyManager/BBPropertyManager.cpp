#include "BBPropertyManager.h"
#include "Utils/BBUtils.h"
#include <QVBoxLayout>
#include "BBGroupManager.h"
#include "BBPropertyFactory.h"
#include "Base/BBGameObject.h"
#include "Render/Light/BBDirectionalLight.h"
#include "Render/Light/BBPointLight.h"
#include "Render/Light/BBSpotLight.h"
#include "BBHeadManager.h"
#include "3D/Model/BBModel.h"


BBPropertyManager::BBPropertyManager(QWidget *pParent)
    : QWidget(pParent)
{
    m_pPreviewOpenGLWidget = NULL;
    m_pBaseInformationManager = NULL;
    m_pSetBaseInformationManager = NULL;
    m_pTransformGroupManager = NULL;
    m_pCurrentGameObject = NULL;
    setWidgetStyle();
    // init a vertical layout
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 8, 10, 0);
}

BBPropertyManager::~BBPropertyManager()
{
    BB_SAFE_DELETE(m_pBaseInformationManager);
    BB_SAFE_DELETE(m_pSetBaseInformationManager);
    BB_SAFE_DELETE(m_pTransformGroupManager);
}

void BBPropertyManager::clear()
{
    m_pCurrentGameObject = NULL;
    while (QLayoutItem *pItem = layout()->takeAt(0))
    {
        layout()->removeWidget(pItem->widget());
        delete pItem->widget();
        delete pItem;
    }
    m_pBaseInformationManager = NULL;
    m_pSetBaseInformationManager = NULL;
    m_pTransformGroupManager = NULL;
}

void BBPropertyManager::showGameObjectProperty(BBGameObject *pGameObject)
{
    // sometimes need to refresh
//    if (m_pCurrentGameObject == pGameObject)
//        return;

    // clear the last time
    clear();
    BB_PROCESS_ERROR_RETURN(pGameObject);
    m_pCurrentGameObject = pGameObject;
    addBaseInformationManager(pGameObject);
    addTransformGroupManager(pGameObject);

    if (pGameObject->getClassName() == BB_CLASSNAME_MODEL)
    {
        BBModel *pModel = (BBModel*)pGameObject;
        layout()->addWidget(new BBRenderManager(pModel->getMesh(), this));
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_DIRECTIONAL_LIGHT)
    {
        BBGroupManager *pRenderManager = addGroupManager("Render", BB_PATH_RESOURCE_ICON(render.png));
        BBLightColorFactory *pColorFactory = new BBLightColorFactory((BBDirectionalLight*)pGameObject);
        pRenderManager->addFactory("Color", pColorFactory, 1);
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_POINT_LIGHT)
    {
        BBPointLight *pLight = (BBPointLight*)pGameObject;
        layout()->addWidget(new BBPointLightManager(pLight, this));
    }

//    if (gameObject->getClassName() == ModelClassName || gameObject->getClassName() == TerrainClassName)
//    {
//        Model *model = (Model*) gameObject;
//        materialFactory = new MaterialFactory(model);
//        renderManager->addProperty("Material", materialFactory, 0);
//        //动画属性组 fbx文件才有动画
//        if (model->getMeshType() == MeshType::fbx)
//        {
//            GroupManager *animManager = addGroupManager("Animation", ":/icon/resources/icons/moive.png");
//            AnimFactory *animFactory = new AnimFactory(model, mPreview);
//            animManager->addProperty("Default", animFactory);
//        }
//        else if (model->getMeshType() == MeshType::terrain)
//        {
//            HeightMapFactory *heightMapFactory = new HeightMapFactory(model);
//            renderManager->addProperty("HeightMap", heightMapFactory, 0);
//        }
//    }
//    else if (gameObject->getClassName() == SpotLightClassName)
//    {
//        SpotLight *light = (SpotLight*) gameObject;
//        SpotLightManager *renderManager = new SpotLightManager(light, this, "Render",
//                                                               ":/icon/resources/icons/render.png");
//        layout()->addWidget(renderManager);
//    }

}

void BBPropertyManager::showGameObjectSetProperty(BBGameObject *pCenterGameObject,
                                                    const QList<BBGameObject*> &gameObjectSet)
{
    clear();
    m_pCurrentGameObject = pCenterGameObject;
    addSetBaseInformationManager(pCenterGameObject, gameObjectSet);
    addTransformGroupManager(pCenterGameObject);
}

void BBPropertyManager::showGlobalSettingsProperty(BBScene *pScene)
{
    clear();
    addGlobalSettingsGroupManager(pScene);
}

void BBPropertyManager::showMaterialProperty(const QString &filePath)
{
    clear();
    addMaterialGroupManager(filePath);
}

void BBPropertyManager::updateCoordinateSystem()
{
    coordinateSystemUpdated();
}

void BBPropertyManager::updateTransform(BBGameObject *pGameObject, char transformModeKey)
{
    // Determine whether the property manager displays the properties of the current operation object
    // because the property manager may be locked
    if (m_pCurrentGameObject != pGameObject)
        return;

    switch (transformModeKey)
    {
    case 'W':
        m_pTransformGroupManager->updatePositionValue();
        break;
    case 'E':
        m_pTransformGroupManager->updateRotationValue();
        break;
    case 'R':
        m_pTransformGroupManager->updateScaleValue();
        break;
    }
}

void BBPropertyManager::setWidgetStyle()
{
    setStyleSheet("QToolButton {border: none; border-radius: 2px; color: #d6dfeb; background: transparent; font: 9pt \"Arial\"; font-weight: bold;}"
                  "QLineEdit {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: rgb(60, 64, 75); selection-color: #d6dfeb; selection-background-color: #8193bc; padding-left: 3px; padding-right: 3px;}"
                  "QLabel {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\";}"
                  "QCheckBox {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: none; padding-top: 1px; padding-bottom: 1px;}"
                  "QCheckBox::indicator:checked {border: none; border-radius: 2px; background: #0ebf9c;}"
                  "QCheckBox::indicator:unchecked {border: none; border-radius: 2px; background: rgb(60, 64, 75);}"
                  "QPushButton {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: none;}"
                  "QComboBox {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: rgb(60, 64, 75);}"
                  "QComboBox::drop-down {margin: 2px; border-image: url(../../resources/icons/arrow_right.png);}"
                  "QComboBox QAbstractItemView {border: none; color: #d6dfeb; font: 9pt \"Arial\"; background: rgb(60, 64, 75);}"
                  "QComboBox QAbstractItemView::item {height: 16px; border: none; padding-left: 3px; padding-right: 3px;}"
                  "QComboBox QAbstractItemView::item:selected {height: 16px; border: none; background-color: #8193bc;}"
                  "QSpinBox {border: none; border-radius: 2px; color: #191f28; font: 9pt \"Arial\"; selection-background-color: rgb(251, 236, 213);}");
}

void BBPropertyManager::addBaseInformationManager(BBGameObject *pGameObject)
{
    m_pBaseInformationManager = new BBBaseInformationManager(pGameObject, this);
    layout()->addWidget(m_pBaseInformationManager);
//    //修改对象的名字 层级视图的树节点的名字也要修改
//    QObject::connect(gameObjectBaseInfoManager, SIGNAL(nameChanged(GameObject*)),
//                     this, SLOT(renameGameObjectInHierarchy(GameObject*)));
//    QObject::connect(gameObjectBaseInfoManager, SIGNAL(activationChanged(GameObject*, bool)),
//                     this, SLOT(changeActivation(GameObject*, bool)));
}

void BBPropertyManager::addSetBaseInformationManager(BBGameObject *pCenterGameObject,
                                                     const QList<BBGameObject*> &gameObjectSet)
{
    m_pSetBaseInformationManager = new BBSetBaseInformationManager(pCenterGameObject, gameObjectSet, this);
    layout()->addWidget(m_pSetBaseInformationManager);
//    //修改集合的可见性
//    QObject::connect(gameObjectSetBaseInfoManager, SIGNAL(activationChanged(QList<GameObject*>, bool)),
//                     this, SLOT(changeActivation(QList<GameObject*>, bool)));
}

BBGroupManager* BBPropertyManager::addGroupManager(const QString &name, const QString &iconPath)
{
    BBGroupManager *pGroupManager = new BBGroupManager(name, iconPath, this);
    layout()->addWidget(pGroupManager);
    return pGroupManager;
}

void BBPropertyManager::addTransformGroupManager(BBGameObject *pGameObject)
{
    m_pTransformGroupManager = new BBTransformGroupManager(pGameObject, this);
    layout()->addWidget(m_pTransformGroupManager);

    QObject::connect(m_pTransformGroupManager, SIGNAL(coordinateSystemUpdated()), this, SLOT(updateCoordinateSystem()));
}

void BBPropertyManager::addGlobalSettingsGroupManager(BBScene *pScene)
{
    layout()->addWidget(new BBGlobalSettingsGroupManager(pScene, this));
}

void BBPropertyManager::addMaterialGroupManager(const QString &filePath)
{
    BBMaterialManager *pMaterialManager = new BBMaterialManager(filePath, this);
    layout()->addWidget(pMaterialManager);
    BBMaterialPropertyGroupManager *pPropertyGroupManager = new BBMaterialPropertyGroupManager(pMaterialManager->getMaterial(),
                                                                                               m_pPreviewOpenGLWidget, this);
    layout()->addWidget(pPropertyGroupManager);
}





//void TextureFactory::removeTexture()
//{
//    if (labelIcon->text() == "None")
//    {
//        //当面没有设置纹理 无需再次移除
//        return;
//    }
//    //修改显示数据
//    setTexture("");
//    //修改材质对象的数据
//    material->setDiffuseMapPath(DefaultTexturePath);
//    //更新图标
//    updateMaterialPreview();
//}

//void TextureFactory::changeTexture(QString path)
//{
//    //修改界面显示
//    setTexture(path);
//    //修改材质
//    material->setDiffuseMapPath(path);
//    //更新图标
//    updateMaterialPreview();
//}


////------------------MaterialFactory----------------------



//void MaterialFactory::removeMaterial()
//{
//    if (labelIcon->text() == "None")
//    {
//        //当面没有 无需再次移除
//        return;
//    }

//    //给该对象设置默认材质
//    mModel->removeMaterial();
//    //修改界面显示
//    setMaterial();
//}

//void MaterialFactory::changeMaterial(QString path)
//{
//    //给该对象设置该材质
//    Material *material = Material::mtlMap.value(path);
//    mModel->setMaterial(material);
//    //修改界面显示
//    setMaterial();
//}


////------------------HeightMapFactory-------------------------


//HeightMapFactory::HeightMapFactory(Model* model, QWidget *parent)
//    : IconFactory(parent)
//{
//    mModel = model;
//    //设置图标控件可拖入的文件后缀类型
//    QList<QString> suffixs;
//    suffixs.append("bmp");
//    suffixs.append("png");
//    suffixs.append("jpg");
//    labelIcon->setAcceptableSuffixs(suffixs);

//    showHeightMap();

//    QObject::connect(labelIcon, SIGNAL(changeValue(QString)), this, SLOT(changeHeight(QString)));
//}

//void HeightMapFactory::changeHeight(QString mapPath)
//{
//    mModel->changeTerrainHeightMap(mapPath);
//    showHeightMap();
//}

//void HeightMapFactory::showHeightMap()
//{
//    labelIcon->setText("");
//    QString path = mModel->getTerrainMesh()->getHeightMapPath();
//    labelIcon->setStyleSheet("color: #d6dfeb; font: 10pt \"Arial\"; "
//                             "border-image: url(" + path + "); border-radius: 2px;");
//}


////------------------AnimFactory-------------------------


//AnimFactory::AnimFactory(Model *model, BaseOpenGLWidget *preview, QWidget *parent)
//    : QComboBox(parent), mPreviewModel(NULL)
//{
//    mModel = model;
//    mPreview = preview;
//    //动画列表
//    FBX *fbx = model->getFbxMesh();
//    QList<QString> animNames = fbx->getAllAnimationNames();
//    animNames.insert(0, "None");
//    addItems(animNames);
//    setView(new QListView());
//    //+1 第0项是none
//    setCurrentIndex(fbx->getCurrentAnimationIndex() + 1);

//    //列表展开时改变项的事件
//    view()->installEventFilter(this);

//    QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrentAnim(int)));
//}

//void AnimFactory::showPopup()
//{
//    mPreview->show();
//    mPreviewModel = mPreview->scene.createModelForPreview(mModel->getFilePath());
//    QComboBox::showPopup();
//}

//void AnimFactory::hidePopup()
//{
//    QComboBox::hidePopup();
//    if (mPreviewModel)
//    {
//        mPreview->scene.deleteGameObject(mPreviewModel);
//        mPreviewModel = NULL;
//    }
//    mPreview->hide();
//}

//bool AnimFactory::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == view())
//    {
//        if (event->type() == QEvent::ToolTip)
//        {
//            //假定每项高度固定为16
//            //-1 减去开头的none
//            int animIndex = ((QHelpEvent*)event)->pos().y() / 16 - 1;
//            if (animIndex < 0)
//            {
//                mPreviewModel->getFbxMesh()->stopAnimation();
//            }
//            else
//            {
//                mPreviewModel->getFbxMesh()->setCurrentAnimation(animIndex);
//                mPreviewModel->getFbxMesh()->startAnimation();
//            }
//        }
//        else if (event->type() == QEvent::FocusOut)
//        {
//            hidePopup();
//        }
//    }
//    return QComboBox::eventFilter(watched, event);
//}

//void AnimFactory::changeCurrentAnim(int index)
//{
//    //-1 减去none
//    mModel->getFbxMesh()->setCurrentAnimation(index - 1);
//}


////------------------GroupManager-------------------------

//void GroupManager::removeProperty(int index)
//{
//    if (index < container->layout()->count())
//    {
//        //少了item的高度 造成不好的视觉效果
//        container->setVisible(false);
//        QLayoutItem *item = container->layout()->takeAt(index);
//        container->layout()->removeWidget(item->widget());
//        delete item->widget();
//        container->setVisible(true);
//    }
//}


////------------------GameObjectBaseInfoManager----------------------


//void GameObjectBaseInfoManager::rename(QString newName)
//{
//    editName->setText(newName);
//}

//void GameObjectBaseInfoManager::finishRename()
//{
//    if (editName->text().isEmpty())
//    {
//        //非法新名字 编辑框重置
//        editName->setText(mGameObject->getName());
//    }
//    else
//    {
//        if (mGameObject->getName() != editName->text())
//        {
//            //修改对象的名字
//            mGameObject->setName(editName->text());
//            //给层级视图发送信号 修改树节点的名字
//            nameChanged(mGameObject);
//        }
//    }
//}


////------------------MaterialManager----------------------


//MaterialManager::MaterialManager(QWidget *parent, QString filePath, BaseOpenGLWidget *previewWidget)
//    : QWidget(parent)
//{
//    mPreviewWidget = previewWidget;
//    mFilePath = filePath;
//    //找到材质对象
//    material = Material::mtlMap.value(filePath);

//    //main maps 属性组
//    GroupManager *mainGroupManager = new GroupManager(this, "Main Maps", "");
//    l->addWidget(mainGroupManager);
//    //给main maps 属性组添加具体属性
//    MaterialColorFactory *colorFactory = new MaterialColorFactory(material->getDiffuseColor(), filePath);
//    mainGroupManager->addProperty("Base Color", colorFactory, 1);
//    TextureFactory *textureFactory = new TextureFactory(material->getDiffuseMapPath(), filePath);
//    mainGroupManager->addProperty("Base Texture", textureFactory, 0);
//    SliderFactory *smoothnessFactory = new SliderFactory(material->getSmoothness(), 0, 100);
//    mainGroupManager->addProperty("Smoothness", smoothnessFactory, 1);
//    //进阶选项 属性组
//    //GroupManager *advancedGroupManager = new GroupManager(this, "Advanced Options", "");
//    //l->addWidget(advancedGroupManager);

//    QObject::connect(textureFactory, SIGNAL(updateMaterialPreview()), this, SLOT(updatePreview()));
//    QObject::connect(colorFactory, SIGNAL(updateMaterialPreview()), this, SLOT(updatePreview()));
//    QObject::connect(enumFactory, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentIndexChangedSlot(QString)));
//    QObject::connect(smoothnessFactory, SIGNAL(valueChangedSignal(int)), this, SLOT(smoothnessChanged(int)));
//}

//void MaterialManager::updatePreview()
//{
//    //利用浏览小窗 计算材质的浏览图
//    Model *model = mPreviewWidget->scene.createModelForPreview(engineResourcesModelsPath + "sphere.obj", 1.4);
//    model->previewMaterial(material);
//    //截下浏览图
//    QPixmap pix = QPixmap::grabWidget(mPreviewWidget);
//    //删去浏览模型
//    mPreviewWidget->scene.deleteGameObject(model);
//    //裁剪成方形
//    pix.setDevicePixelRatio(devicePixelRatio());
//    int h = pix.height();
//    int w = pix.width();
//    int size = h < w ? h : w;
//    pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);
//    //设置材质的浏览图
//    material->setPreview(pix);

//    //材质图标更新
//    setIcon();
//    //其他显示材质浏览图的地方更新
//    updatePreviewInOtherWidget(mFilePath);
//}

//void MaterialManager::currentIndexChangedSlot(const QString &text)
//{
//    if (text == Material::StandardShader)
//    {
//        material->setShader(Material::StandardShader, engineResourcesShadersPath + "standard.vert",
//                            engineResourcesShadersPath + "standard.frag");
//    }
//    else if (text == Material::BlinByVertexShader)
//    {
//        material->setShader(Material::BlinByVertexShader, engineResourcesShadersPath + "blin_by_vertex.vert",
//                            engineResourcesShadersPath + "blin_by_vertex.frag");
//    }

//    updatePreview();
//}

//void MaterialManager::smoothnessChanged(int value)
//{
//    material->setSmoothness(value);
//    updatePreview();
//}


////------------------SceneManager---------------------


//SceneManager::SceneManager(Scene *scene, QWidget *parent)
//    : QWidget(parent)
//{
//    mScene = scene;
//    //布局
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    //图标和信息的水平布局
//    QWidget *baseInfoWidget = new QWidget(this);
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(baseInfoWidget);
//    baseInfoLayout->setContentsMargins(10, 0, 0, 0);
//    QLabel *icon = new QLabel(baseInfoWidget);
//    icon->setFocusPolicy(Qt::NoFocus);
//    QPixmap pix(":/icon/resources/icons/earth.png");
//    //和两行差不多高
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon->setPixmap(pix);
//    baseInfoLayout->addWidget(icon, 1, Qt::AlignLeft);
//    //右场景名
//    QString name = "Scene1";
//    QLabel *labelName = new QLabel(name, baseInfoWidget);
//    labelName->setStyleSheet("font: 75 11pt \"Arial\";");
//    baseInfoLayout->addWidget(labelName, 3);
//    l->addWidget(baseInfoWidget);

//    //fog属性组
//    fogManager = new GroupManager(this, "Fog", ":/icon/resources/icons/fog.png");
//    l->addWidget(fogManager);
//    //给属性组添加具体属性
//    QCheckBox *checkBox = new QCheckBox();
//    checkBox->setChecked(mScene->getFogSwitch());
//    fogManager->addProperty("On-Off", checkBox);
//    ColorFactory *colorFactory = new ColorFactory(mScene->getFogColor());
//    fogManager->addProperty("Color", colorFactory);
//    QStringList mode;
//    mode.append("Linear");
//    mode.append("exponential");
//    mode.append("exponential X");
//    QComboBox *comboBox = new QComboBox();
//    comboBox->addItems(mode);
//    comboBox->setView(new QListView());
//    comboBox->setCurrentIndex(mScene->getFogMode());
//    fogManager->addProperty("Mode", comboBox);
//    //根据雾的模式 设置之后的属性控件
//    switchMode(mScene->getFogMode());

//    QObject::connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(switchFog(bool)));
//    QObject::connect(colorFactory, SIGNAL(colorChanged(float, float, float)),
//                     this, SLOT(setFogColor(float, float, float)));
//    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchMode(int)));
//}

//void SceneManager::switchFog(bool b)
//{
//    mScene->switchFog(b);
//}

//void SceneManager::setFogColor(float r, float g, float b)
//{
//    mScene->setFogColor(r, g, b);
//}

//void SceneManager::switchMode(int id)
//{
//    mScene->setFogMode(id);
//    fogManager->removeProperty(3);
//    //删完 后面的变为了3 如果只有一项 后面的3不存在 不会进行删除
//    fogManager->removeProperty(3);
//    switch(id)
//    {
//    case 0:
//        startEditFactory = fogManager->addProperty("Start", mScene->getFogStart());
//        startEditFactory->setRegExp("^([0]|[1-9][0-9]{0,4})(?:\\.\\d{1,4})?$|(^\\t?$)");
//        startEditFactory->setSliderRange(0, 100000);
//        startEditFactory->setSliderStep(0.1f);
//        endEditFactory = fogManager->addProperty("End", mScene->getFogEnd());
//        endEditFactory->setRegExp("^([0]|[1-9][0-9]{0,4})(?:\\.\\d{1,4})?$|(^\\t?$)");
//        endEditFactory->setSliderRange(0, 100000);
//        endEditFactory->setSliderStep(0.1f);
//        QObject::connect(startEditFactory, SIGNAL(valueChanged(float)), this, SLOT(setFogStart(float)));
//        QObject::connect(endEditFactory, SIGNAL(valueChanged(float)), this, SLOT(setFogEnd(float)));
//        break;
//    case 1:
//        densitySliderFactory = new SliderFactory(mScene->getFogDensity());
//        fogManager->addProperty("Density", densitySliderFactory);
//        QObject::connect(densitySliderFactory, SIGNAL(valueChangedSignal(int)), this, SLOT(setFogDensity(int)));
//        break;
//    case 2:
//        densitySliderFactory = new SliderFactory(mScene->getFogDensity());
//        fogManager->addProperty("Density", densitySliderFactory);
//        QObject::connect(densitySliderFactory, SIGNAL(valueChangedSignal(int)), this, SLOT(setFogDensity(int)));
//        powerEditFactory = fogManager->addProperty("Power", mScene->getFogPower());
//        powerEditFactory->setRegExp("^([0]|[1-9][0-9]{0,0})(?:\\.\\d{1,2})?$|(^\\t?$)");
//        powerEditFactory->setSliderRange(0, 10);
//        powerEditFactory->setSliderStep(0.2f);
//        QObject::connect(powerEditFactory, SIGNAL(valueChanged(float)), this, SLOT(setFogPower(float)));
//        break;
//    }
//}

//void SceneManager::setFogStart(float start)
//{
//    float end = mScene->getFogEnd();
//    if (end - start < 0.01f)
//    {
//        //上下限不合法 调整
//        start = end - 0.01f;
//        startEditFactory->setValue(start);
//    }
//    mScene->setFogOption(start, end, mScene->getFogDensity(), mScene->getFogPower());
//}

//void SceneManager::setFogEnd(float end)
//{
//    float start = mScene->getFogStart();
//    if (end - start < 0.01f)
//    {
//        //上下限不合法 调整
//        end = start + 0.01f;
//        endEditFactory->setValue(end);
//    }
//    mScene->setFogOption(start, end, mScene->getFogDensity(), mScene->getFogPower());
//}

//void SceneManager::setFogDensity(int density)
//{
//    mScene->setFogOption(mScene->getFogStart(), mScene->getFogEnd(), density, mScene->getFogPower());
//}

//void SceneManager::setFogPower(float power)
//{
//    mScene->setFogOption(mScene->getFogStart(), mScene->getFogEnd(), mScene->getFogDensity(), power);
//}


////------------------SpotLightManager----------------------


//SpotLightManager::SpotLightManager(SpotLight *light, QWidget *parent, QString name, QString iconPath)
//    : PointLightManager(light, parent, name, iconPath)
//{
//    mLight = light;
//    LineEditFactory *angleFactory = addProperty("Spot Angle", light->getSpotAngle());
//    //0-179度
//    QObject::connect(angleFactory, SIGNAL(valueChanged(float)), this, SLOT(changeSpotAngle(float)));
//    angleFactory->setRegExp("^([0-9]|([1-9][0-9])|([1][0-7][0-9]))$");
//    angleFactory->setSliderRange(0, 179);
//    angleFactory->setSliderStep(1);

//    //调节聚光度的滑块
//    SliderFactory *spotLevelSliderFactory = new SliderFactory(light->getSpotLevel(), 0, 128);
//    addProperty("Spot Level", spotLevelSliderFactory);
//    QObject::connect(spotLevelSliderFactory, SIGNAL(valueChangedSignal(int)), this, SLOT(changeSpotLevel(int)));
//}

//void SpotLightManager::changeSpotAngle(float value)
//{
//    mLight->setSpotAngle(value);
//}

//void SpotLightManager::changeSpotLevel(int value)
//{
//    mLight->setSpotLevel(value);
//}


////------------------FbxFileManager----------------------


//FbxFileManager::FbxFileManager(QString filePath, QWidget *parent)
//    : QWidget(parent)
//{
//    //布局
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    //图标和信息的水平布局
//    QWidget *baseInfoWidget = new QWidget(this);
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(baseInfoWidget);
//    baseInfoLayout->setContentsMargins(10, 0, 0, 0);
//    QLabel *icon = new QLabel(baseInfoWidget);
//    icon->setFocusPolicy(Qt::NoFocus);
//    QPixmap pix(FileList::getMetaJpgFilePath(filePath));
//    //和两行差不多高
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon->setPixmap(pix);
//    baseInfoLayout->addWidget(icon, 1, Qt::AlignLeft);
//    //右场景名
//    QString name = filePath.mid(filePath.lastIndexOf('/') + 1);
//    name = name.mid(0, name.lastIndexOf('.'));
//    QLabel *labelName = new QLabel(name, baseInfoWidget);
//    labelName->setStyleSheet("font: 75 11pt \"Arial\";");
//    baseInfoLayout->addWidget(labelName, 3);
//    l->addWidget(baseInfoWidget);

//    //fbx
//    FBX fbx(filePath);
//    QList<QString> animNames = fbx.getAllAnimationNames();

//    //动画属性组
//    GroupManager *animManager = new GroupManager(this, "Animation", ":/icon/resources/icons/moive.png");
//    l->addWidget(animManager);
//    //给属性组添加具体属性
//    for (int i = 0; i < animNames.count(); i++)
//    {
//        QLineEdit *edit = new QLineEdit();
//        animManager->addProperty(animNames.at(i), edit);
//    }

//}


////------------------PropertyManager----------------------


//void PropertyManager::showFbxProperty(QString filePath)
//{
//    clear();
//    if (!filePath.isNull())
//    {
//        FbxFileManager *fbxFileManager = new FbxFileManager(filePath, this);
//        layout()->addWidget(fbxFileManager);
//    }
//}

//void PropertyManager::renameGameObject(GameObject *gameObject, QString newName)
//{
//    //判断属性栏是否显示的是当前操作对象的属性 因为属性栏可能被锁定
//    if (currentObject != gameObject)
//        return;
//    gameObjectBaseInfoManager->rename(newName);
//}

//void PropertyManager::renameGameObjectInHierarchy(GameObject *gameObject)
//{
//    renameGameObjectInHierarchySignal(gameObject);
//}

//void PropertyManager::changeActivation(GameObject *gameObject, bool isActive)
//{
//    changeActivationSignal(gameObject, isActive);
//}

//void PropertyManager::changeActivation(QList<GameObject*> gameObjects, bool isActive)
//{
//    int count = gameObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        changeActivationSignal(gameObjects.at(i), isActive);
//    }
//    //重新选中所有对象 否则会选中最后一项
//    setSelectedObjects(gameObjects);
//}

//void PropertyManager::changeButtonActiveCheckState(GameObject *gameObject, bool isActive)
//{
//    //判断属性栏是否显示的是当前操作对象的属性 因为属性栏可能被锁定
//    if (currentObject != gameObject)
//        return;
//    gameObjectBaseInfoManager->changeButtonActiveCheckState(isActive);
//}

//void PropertyManager::updateMaterialPreviewInOtherWidget(QString filePath)
//{
//    updateMaterialPreviewInOtherWidgetSignal(filePath);
//}

//void PropertyManager::updateMaterialFactory(Model* model)
//{
//    //如果当前属性栏显示的是model的属性
//    if (currentObject == model)
//    {
//        materialFactory->setMaterial();
//    }
//}
