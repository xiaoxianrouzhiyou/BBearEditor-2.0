#include "BBPropertyManager.h"
#include "Utils/BBUtils.h"
#include <QVBoxLayout>
#include "GroupManager/BBGroupManager.h"
#include "GroupManager/BBMaterialPropertyGroupManager.h"
#include "GroupManager/BBLightManager.h"
#include "GroupManager/BBGlobalSettingsGroupManager.h"
#include "GroupManager/BBRenderManager.h"
#include "GroupManager/BBOfflineRendererManager.h"
#include "BBPropertyFactory.h"
#include "Base/BBGameObject.h"
#include "Lighting/GameObject/BBDirectionalLight.h"
#include "Lighting/GameObject/BBPointLight.h"
#include "Lighting/GameObject/BBSpotLight.h"
#include "BBHeadManager.h"
#include "3D/BBModel.h"
#include "Physics/FluidSystem/BBSPHFluidSystem.h"
#include "GroupManager/BBFluidRendererManager.h"
#include "Physics/FluidSystem/BBSPHFluidRenderer.h"
#include "GroupManager/BBGeometricProcessingManager.h"
#include "3D/Mesh/BBProcedureMesh.h"


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
    pLayout->setContentsMargins(0, 8, 10, 10);
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

    if (pGameObject->getClassName() == BB_CLASSNAME_MODEL
            || pGameObject->getClassName() == BB_CLASSNAME_TERRAIN
            || pGameObject->getClassName() == BB_CLASSNAME_PROCEDURE_MESH)
    {
        BBModel *pModel = (BBModel*)pGameObject;
        layout()->addWidget(new BBRenderManager(pModel->getMesh(), this));

        // Sub
        if (pGameObject->getClassName() == BB_CLASSNAME_PROCEDURE_MESH)
        {
            layout()->addWidget(new BBGeometricProcessingManager((BBProcedureMesh*)pModel->getMesh(), this));
        }
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_SPHFLUID)
    {
        BBSPHFluidSystem *pFluidSystem = (BBSPHFluidSystem*)pGameObject;
        layout()->addWidget(new BBFluidRendererManager(pFluidSystem->getFluidRenderer(), this));
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
    else if (pGameObject->getClassName() == BB_CLASSNAME_SPOT_LIGHT)
    {
        BBSpotLight *pLight = (BBSpotLight*)pGameObject;
        layout()->addWidget(new BBSpotLightManager(pLight, this));
    }

//    if (gameObject->getClassName() == ModelClassName || gameObject->getClassName() == TerrainClassName)
//    {
//        Model *model = (Model*) gameObject;
//        materialFactory = new MaterialFactory(model);
//        renderManager->addProperty("Material", materialFactory, 0);
//        //??????????????? fbx??????????????????
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

void BBPropertyManager::showOfflineRendererProperty(BBOfflineRenderer *pOfflineRenderer)
{
    clear();
    addOfflineRendererManager(pOfflineRenderer);
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

void BBPropertyManager::updateFileList()
{
    emit fileListUpdated();
}

void BBPropertyManager::setWidgetStyle()
{
    setStyleSheet("QToolButton {border: none; border-radius: 2px; color: #d6dfeb; background: transparent; font: 9pt \"Arial\"; font-weight: bold;}"
                  "QLineEdit {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: rgb(60, 64, 75); selection-color: #d6dfeb; selection-background-color: #8193bc; padding-left: 3px; padding-right: 3px;}"
                  "QLabel {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\";}"
                  "QCheckBox {border: none; border-radius: 2px; color: #d6dfeb; font: 9pt \"Arial\"; background: none; padding: 0px;}"
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
//    //????????????????????? ?????????????????????????????????????????????
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
//    //????????????????????????
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
    BBGlobalSettingsGroupManager *pGlobalSettingsGroupManager = new BBGlobalSettingsGroupManager(pScene, this);
    layout()->addWidget(pGlobalSettingsGroupManager);

    QObject::connect(pGlobalSettingsGroupManager, SIGNAL(updateFileList()), this, SLOT(updateFileList()));
}

void BBPropertyManager::addOfflineRendererManager(BBOfflineRenderer *pOfflineRenderer)
{
    BBOfflineRendererManager *pOfflineRendererManager = new BBOfflineRendererManager(pOfflineRenderer, this);
    layout()->addWidget(pOfflineRendererManager);
}

void BBPropertyManager::addMaterialGroupManager(const QString &filePath)
{
    BBMaterialManager *pMaterialManager = new BBMaterialManager(filePath, this);
    layout()->addWidget(pMaterialManager);
    BBMaterialPropertyGroupManager *pPropertyGroupManager = new BBMaterialPropertyGroupManager(pMaterialManager->getMaterial(), m_pPreviewOpenGLWidget, this);
    layout()->addWidget(pPropertyGroupManager);
}





////------------------AnimFactory-------------------------


//AnimFactory::AnimFactory(Model *model, BaseOpenGLWidget *preview, QWidget *parent)
//    : QComboBox(parent), mPreviewModel(NULL)
//{
//    mModel = model;
//    mPreview = preview;
//    //????????????
//    FBX *fbx = model->getFbxMesh();
//    QList<QString> animNames = fbx->getAllAnimationNames();
//    animNames.insert(0, "None");
//    addItems(animNames);
//    setView(new QListView());
//    //+1 ???0??????none
//    setCurrentIndex(fbx->getCurrentAnimationIndex() + 1);

//    //?????????????????????????????????
//    view()->installEventFilter(this);

//    QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrentAnim(int)));
//}

//bool AnimFactory::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == view())
//    {
//        if (event->type() == QEvent::ToolTip)
//        {
//            //???????????????????????????16
//            //-1 ???????????????none
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
//    //-1 ??????none
//    mModel->getFbxMesh()->setCurrentAnimation(index - 1);
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
//        //??????????????? ???????????????
//        editName->setText(mGameObject->getName());
//    }
//    else
//    {
//        if (mGameObject->getName() != editName->text())
//        {
//            //?????????????????????
//            mGameObject->setName(editName->text());
//            //??????????????????????????? ????????????????????????
//            nameChanged(mGameObject);
//        }
//    }
//}


////------------------SceneManager---------------------


//SceneManager::SceneManager(Scene *scene, QWidget *parent)
//    : QWidget(parent)
//{
//    mScene = scene;
//    //??????
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    //??????????????????????????????
//    QWidget *baseInfoWidget = new QWidget(this);
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(baseInfoWidget);
//    baseInfoLayout->setContentsMargins(10, 0, 0, 0);
//    QLabel *icon = new QLabel(baseInfoWidget);
//    icon->setFocusPolicy(Qt::NoFocus);
//    QPixmap pix(":/icon/resources/icons/earth.png");
//    //?????????????????????
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon->setPixmap(pix);
//    baseInfoLayout->addWidget(icon, 1, Qt::AlignLeft);
//    //????????????
//    QString name = "Scene1";
//    QLabel *labelName = new QLabel(name, baseInfoWidget);
//    labelName->setStyleSheet("font: 75 11pt \"Arial\";");
//    baseInfoLayout->addWidget(labelName, 3);
//    l->addWidget(baseInfoWidget);

//    //fog?????????
//    fogManager = new GroupManager(this, "Fog", ":/icon/resources/icons/fog.png");
//    l->addWidget(fogManager);
//    //??????????????????????????????
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
//    //?????????????????? ???????????????????????????
//    switchMode(mScene->getFogMode());

//    QObject::connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(switchFog(bool)));
//    QObject::connect(colorFactory, SIGNAL(colorChanged(float, float, float)),
//                     this, SLOT(setFogColor(float, float, float)));
//    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchMode(int)));
//}


//void SceneManager::switchMode(int id)
//{
//    mScene->setFogMode(id);
//    fogManager->removeProperty(3);
//    //?????? ??????????????????3 ?????????????????? ?????????3????????? ??????????????????
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
//        //?????????????????? ??????
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
//        //?????????????????? ??????
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
//    //0-179???
//    QObject::connect(angleFactory, SIGNAL(valueChanged(float)), this, SLOT(changeSpotAngle(float)));
//    angleFactory->setRegExp("^([0-9]|([1-9][0-9])|([1][0-7][0-9]))$");
//    angleFactory->setSliderRange(0, 179);
//    angleFactory->setSliderStep(1);

//    //????????????????????????
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
//    //??????
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    //??????????????????????????????
//    QWidget *baseInfoWidget = new QWidget(this);
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(baseInfoWidget);
//    baseInfoLayout->setContentsMargins(10, 0, 0, 0);
//    QLabel *icon = new QLabel(baseInfoWidget);
//    icon->setFocusPolicy(Qt::NoFocus);
//    QPixmap pix(FileList::getMetaJpgFilePath(filePath));
//    //?????????????????????
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon->setPixmap(pix);
//    baseInfoLayout->addWidget(icon, 1, Qt::AlignLeft);
//    //????????????
//    QString name = filePath.mid(filePath.lastIndexOf('/') + 1);
//    name = name.mid(0, name.lastIndexOf('.'));
//    QLabel *labelName = new QLabel(name, baseInfoWidget);
//    labelName->setStyleSheet("font: 75 11pt \"Arial\";");
//    baseInfoLayout->addWidget(labelName, 3);
//    l->addWidget(baseInfoWidget);

//    //fbx
//    FBX fbx(filePath);
//    QList<QString> animNames = fbx.getAllAnimationNames();

//    //???????????????
//    GroupManager *animManager = new GroupManager(this, "Animation", ":/icon/resources/icons/moive.png");
//    l->addWidget(animManager);
//    //??????????????????????????????
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
//    //???????????????????????????????????????????????????????????? ??????????????????????????????
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
//    //???????????????????????? ???????????????????????????
//    setSelectedObjects(gameObjects);
//}

//void PropertyManager::changeButtonActiveCheckState(GameObject *gameObject, bool isActive)
//{
//    //???????????????????????????????????????????????????????????? ??????????????????????????????
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
//    //?????????????????????????????????model?????????
//    if (currentObject == model)
//    {
//        materialFactory->setMaterial();
//    }
//}
