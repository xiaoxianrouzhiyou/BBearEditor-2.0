#include "BBPropertyManager.h"
#include "BBUtils.h"
#include <QVBoxLayout>
#include "BBGroupManager.h"


BBPropertyManager::BBPropertyManager(QWidget *pParent)
    : QWidget(pParent)
{
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

//    if (gameObject)
//    {
//        if (gameObject->getClassName() == ModelClassName || gameObject->getClassName() == TerrainClassName)
//        {
//            //渲染属性组
//            GroupManager *renderManager = addGroupManager("Render", ":/icon/resources/icons/render.png");
//            Model *model = (Model*) gameObject;
//            materialFactory = new MaterialFactory(model);
//            renderManager->addProperty("Material", materialFactory, 0);
//            //动画属性组 fbx文件才有动画
//            if (model->getMeshType() == MeshType::fbx)
//            {
//                GroupManager *animManager = addGroupManager("Animation", ":/icon/resources/icons/moive.png");
//                AnimFactory *animFactory = new AnimFactory(model, mPreview);
//                animManager->addProperty("Default", animFactory);
//            }
//            else if (model->getMeshType() == MeshType::terrain)
//            {
//                HeightMapFactory *heightMapFactory = new HeightMapFactory(model);
//                renderManager->addProperty("HeightMap", heightMapFactory, 0);
//            }
//        }
//        else if (gameObject->getClassName() == DirectionLightClassName)
//        {
//            GroupManager *renderManager = addGroupManager("Render", ":/icon/resources/icons/render.png");
//            //转为DirectionLight*才会多态
//            DirectionLight *light = (DirectionLight*) gameObject;
//            LightColorFactory *colorFactory = new LightColorFactory(light);
//            renderManager->addProperty("Color", colorFactory, 1);
//        }
//        else if (gameObject->getClassName() == PointLightClassName)
//        {
//            PointLight *light = (PointLight*) gameObject;
//            PointLightManager *renderManager = new PointLightManager(light, this, "Render",
//                                                                     ":/icon/resources/icons/render.png");
//            layout()->addWidget(renderManager);
//        }
//        else if (gameObject->getClassName() == SpotLightClassName)
//        {
//            SpotLight *light = (SpotLight*) gameObject;
//            SpotLightManager *renderManager = new SpotLightManager(light, this, "Render",
//                                                                   ":/icon/resources/icons/render.png");
//            layout()->addWidget(renderManager);
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




////------------------ColorButton--------------------------


//ColorButton::ColorButton(QWidget *parent)
//    : QPushButton(parent)
//{
//    //白色大背景
//    setStyleSheet("border: none; border-radius: 2px; padding: 2px 4px; background: white;");
//    //在白色背景中加入设置具体颜色的内容控件 这样做 透明颜色可以与白色叠加
//    contentWhite = new QWidget(this);
//    QHBoxLayout *l = new QHBoxLayout(this);
//    l->setMargin(1);
//    l->addWidget(contentWhite);
//    //在白色背景的内容控件中加入一块黑色背景
//    QHBoxLayout *whiteLayout = new QHBoxLayout(contentWhite);
//    whiteLayout->setMargin(2);
//    whiteLayout->setSpacing(0);
//    //黑色背景
//    QWidget *black = new QWidget(this);
//    black->setStyleSheet("background: black;");
//    whiteLayout->addWidget(black);
//    //在黑色背景中加入设置具体颜色的内容控件 这样做 透明颜色可以与黑色叠加
//    contentBlack = new QWidget(black);
//    QHBoxLayout *blackLayout = new QHBoxLayout(black);
//    blackLayout->setMargin(1);
//    blackLayout->addWidget(contentBlack);
//    //占位
//    QWidget *transparent = new QWidget(contentWhite);
//    transparent->setStyleSheet("background: transparent;");
//    whiteLayout->addWidget(transparent);
//    //默认选中白色
//    setColor(255, 255, 255, 255);
//    QObject::connect(this, SIGNAL(clicked()), this, SLOT(clickedSlot()));
//}

//void ColorButton::setColor(int r, int g, int b, int a)
//{
//    QString styleSheet = "background: rgba(" + QString::number(r) + ", " + QString::number(g) + ", "
//            + QString::number(b) + ", " + QString::number(a) + ");";
//    //刷新成新颜色
//    contentBlack->setStyleSheet(styleSheet);
//    contentWhite->setStyleSheet(styleSheet);
//}

//void ColorButton::setColor(QColor color)
//{
//    setColor(color.red(), color.green(), color.blue(), color.alpha());
//}

//void ColorButton::clickedSlot()
//{
//    qDebug() << "123";
//}


////------------------ScreenDialog-------------------------


//int ScreenDialog::cursorSize = 19;

//ScreenDialog::ScreenDialog(QWidget *parent)
//    : QDialog(parent)
//{
//    //边框
//    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
//    setWindowState(Qt::WindowMaximized);
//    //截取屏幕 将对话框背景设置与屏幕一致
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    background = new QLabel(this);
//    l->addWidget(background);
//    //截屏
//    setBackground();
//    //除去弹出动画
//    hide();
//    show();
//    //设置光标为滴管
//    QPixmap pix(":/icon/resources/icons/eyedropper.png");
//    //针对R屏
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(cursorSize * devicePixelRatio(), cursorSize * devicePixelRatio(),
//                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    setCursor(QCursor(pix));
//}

//void ScreenDialog::setBackground()
//{
//    //截屏
//    pixBackground = QPixmap::grabWindow(QApplication::desktop()->winId());
//    //R屏
//    pixBackground.setDevicePixelRatio(devicePixelRatio());
//    //mac需要去掉截屏上方的菜单栏
//#if defined(Q_OS_WIN32)

//#elif defined(Q_OS_MAC)
//    //去掉截屏的菜单栏的高度
//    pixBackground = pixBackground.copy(0, QApplication::desktop()->availableGeometry().y() * devicePixelRatio(),
//                                       pixBackground.width(), pixBackground.height());
//#endif
//    background->setPixmap(pixBackground);
//}

//void ScreenDialog::mousePressEvent(QMouseEvent *event)
//{
//    QColor color = pixBackground.toImage().pixelColor((event->pos() + cursorSize / 2 * QPoint(-1, 1)) * devicePixelRatio());
//    //设置颜色按钮的颜色为当前选中值
//    setColor(color.red(), color.green(), color.blue());
//    accept();
//}


////------------------ColorFactory------------------------


//ColorFactory::ColorFactory(QColor color, QWidget *parent)
//    : QWidget(parent)
//{
//    QHBoxLayout *l = new QHBoxLayout(this);
//    l->setMargin(0);
//    QPushButton *buttonDropper = new QPushButton(this);
//    buttonDropper->setStyleSheet("image: url(:/icon/resources/icons/eyedropper.png);");
//    l->addWidget(buttonDropper, 0);
//    buttonColor = new ColorButton(this);
//    l->addWidget(buttonColor, 1);

//    QObject::connect(buttonDropper, SIGNAL(clicked()), this, SLOT(catchColor()));

//    //给定初始颜色
//    buttonColor->setColor(color);
//}

//void ColorFactory::catchColor()
//{
//    //创建一个无边框最大化的模态对话框
//    //和屏幕长相一致
//    //在对话框上选取颜色
//    ScreenDialog dialog;
//    //选取完毕 颜色按钮设置选中颜色
//    QObject::connect(&dialog, SIGNAL(setColor(int, int, int)), this, SLOT(finishCatchColor(int, int, int)));
//    //会阻塞 绑定信号槽要在之前
//    dialog.exec();
//}

//void ColorFactory::finishCatchColor(int r, int g, int b)
//{
//    buttonColor->setColor(r, g, b);
//    colorChanged(r / 255.0f, g / 255.0f, b / 255.0f);
//}


////------------------MaterialColorFactory-------------------


//MaterialColorFactory::MaterialColorFactory(QColor color, QString filePath, QWidget *parent)
//    : ColorFactory(color, parent)
//{
//    material = Material::mtlMap.value(filePath);
//}

//void MaterialColorFactory::finishCatchColor(int r, int g, int b)
//{
//    ColorFactory::finishCatchColor(r, g, b);
//    //更新材质
//    material->setDiffuseColor(r, g, b);
//    //更新材质浏览图的显示 如图标
//    updateMaterialPreview();
//}


////------------------LightColorFactory--------------------


//LightColorFactory::LightColorFactory(Light *light, QWidget *parent)
//    : ColorFactory(light->getColor(), parent)
//{
//    mLight = light;
//}

//void LightColorFactory::finishCatchColor(int r, int g, int b)
//{
//    ColorFactory::finishCatchColor(r, g, b);
//    //修改光的颜色属性
//    mLight->setColor(r, g, b);
//}


////------------------IconLabel----------------------------


//IconLabel::IconLabel(QWidget *parent)
//    : QLabel(parent)
//{
//    //支持拖放
//    setAcceptDrops(true);
//}

//void IconLabel::setAcceptableSuffixs(QList<QString> acceptableSuffixs)
//{
//    mAcceptableSuffixs = acceptableSuffixs;
//}

//void IconLabel::dragEnterEvent(QDragEnterEvent *event)
//{
//    QByteArray data;
//    if ((data = event->mimeData()->data(FileList::getMimeType())) != nullptr)
//    {
//        //只能拖文件
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString filePath;
//        dataStream >> filePath;
//        QString suffix = filePath.mid(filePath.lastIndexOf('.') + 1);
//        if (mAcceptableSuffixs.contains(suffix))
//        {
//            //接受的文件类型
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
//    }
//    else
//    {
//        event->ignore();
//    }
//}

//void IconLabel::dropEvent(QDropEvent *event)
//{
//    QByteArray data = event->mimeData()->data(FileList::getMimeType());
//    QDataStream dataStream(&data, QIODevice::ReadOnly);
//    QString filePath;
//    dataStream >> filePath;
//    changeValue(filePath);
//    event->accept();
//}


////------------------IconFactory--------------------------


//IconFactory::IconFactory(QWidget *parent)
//    : QWidget(parent)
//{
//    QHBoxLayout *l = new QHBoxLayout(this);
//    l->setMargin(0);
//    //左侧按钮
//    buttonRemove = new QPushButton(this);
//    buttonRemove->setStyleSheet("image: url(:/icon/resources/icons/return.png);");
//    l->addWidget(buttonRemove, 0, Qt::AlignBottom);
//    buttonMore = new QPushButton(this);
//    buttonMore->setStyleSheet("image: url(:/icon/resources/icons/more2.png);");
//    l->addWidget(buttonMore, 0, Qt::AlignBottom);
//    //右侧显示纹理图片
//    //边框
//    QWidget *frame = new QWidget(this);
//    frame->setStyleSheet("border: none; border-radius: 2px; background: rgb(60, 64, 75);");
//    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
//    frameLayout->setMargin(1);
//    labelIcon = new IconLabel(frame);
//    labelIcon->setAlignment(Qt::AlignCenter);
//    labelIcon->setFocusPolicy(Qt::NoFocus);
//    labelIcon->setText("None");
//    //尺寸
//    labelIcon->setMinimumSize(24 * devicePixelRatio(), 24 * devicePixelRatio());
//    frameLayout->addWidget(labelIcon);
//    l->addWidget(frame, 1, Qt::AlignRight);
//}


////------------------TextureFactory-----------------------


//TextureFactory::TextureFactory(QString texturePath, QString materialPath, QWidget *parent)
//    : IconFactory(parent)
//{
//    material = Material::mtlMap.value(materialPath);
//    //从文件读取的纹理
//    setTexture(texturePath);
//    //设置图标控件可拖入的文件后缀类型
//    QList<QString> suffixs;
//    suffixs.append("bmp");
//    suffixs.append("png");
//    suffixs.append("jpg");
//    labelIcon->setAcceptableSuffixs(suffixs);

//    QObject::connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeTexture()));
//    QObject::connect(labelIcon, SIGNAL(changeValue(QString)), this, SLOT(changeTexture(QString)));
//}

//void TextureFactory::setTexture(QString texturePath)
//{
//    //默认纹理不处理
//    if (texturePath == DefaultTexturePath)
//    {
//        texturePath = "";
//    }

//    //设置纹理显示区的内容
//    if (texturePath.isEmpty())
//    {
//        //没有设置纹理 文字提示
//        labelIcon->setText("None");
//    }
//    else
//    {
//        //纹理是否存在
//        if (QFile(texturePath).exists())
//        {
//            labelIcon->setText("");
//        }
//        else
//        {
//            labelIcon->setText("Missing");
//        }
//    }
//    labelIcon->setStyleSheet("color: #d6dfeb; font: 10pt \"Arial\"; "
//                             "border-image: url(" + texturePath + "); border-radius: 2px;");
//}

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


//MaterialFactory::MaterialFactory(Model* model, QWidget *parent)
//    : IconFactory(parent)
//{
//    mModel = model;
//    //设置图标控件可拖入的文件后缀类型
//    QList<QString> suffixs;
//    suffixs.append("mtl");
//    labelIcon->setAcceptableSuffixs(suffixs);

//    setMaterial();

//    QObject::connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeMaterial()));
//    QObject::connect(labelIcon, SIGNAL(changeValue(QString)), this, SLOT(changeMaterial(QString)));
//}

//void MaterialFactory::setMaterial()
//{

//    if (mModel->getMaterial())
//    {
//        //可能无材质
//        labelIcon->setText("");
//        QString path = engineResourcesPicturesPath + materialPreviewName;
//        //将材质的浏览图暂存在engine目录下
//        mModel->getMaterial()->getPreview().save(path);
//        //设置图片
//        labelIcon->setStyleSheet("color: #d6dfeb; font: 10pt \"Arial\"; "
//                                 "border-image: url(" + path + "); border-radius: 2px;");
//    }
//    else
//    {
//        labelIcon->setText("None");
//        labelIcon->setStyleSheet("color: #d6dfeb; font: 10pt \"Arial\"; border-radius: 2px;");
//    }

//}

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


////------------------SliderFactory-------------------------


//SliderFactory::SliderFactory(int value, int min, int max, QWidget *parent)
//    : QWidget(parent)
//{
//    QHBoxLayout *l = new QHBoxLayout(this);
//    l->setMargin(0);
//    slider = new QSlider(Qt::Horizontal, this);
//    l->addWidget(slider);
//    valueEdit = new QLineEdit(this);
//    valueEdit->setMaximumWidth(50);
//    QRegExp rx("[0-9]+$");
//    QValidator *validator = new QRegExpValidator(rx, valueEdit);
//    valueEdit->setValidator(validator);
//    l->addWidget(valueEdit);

//    setRange(min, max);
//    setValue(value);

//    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
//    QObject::connect(valueEdit, SIGNAL(textEdited(QString)), this, SLOT(valueChanged(QString)));
//}

//void SliderFactory::setRange(int min, int max)
//{
//    mMin = min;
//    mMax = max;
//    slider->setRange(min, max);
//}

//void SliderFactory::setValue(int value)
//{
//    slider->setValue(value);
//    valueEdit->setText(QString::number(value));
//}

//void SliderFactory::valueChanged(int value)
//{
//    valueEdit->setText(QString::number(value));
//    valueChangedSignal(value);
//}

//void SliderFactory::valueChanged(QString value)
//{
//    int valueInt = value.toInt();
//    slider->setValue(valueInt);
//    if (!value.isEmpty())
//    {
//        //否则不能清空编辑框
//        if (valueInt > mMax)
//            valueInt = mMax;
//        if (valueInt < mMin)
//            valueInt = mMin;
//        valueEdit->setText(QString::number(valueInt));
//    }
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



//void GroupManager::addProperty(QWidget *factory)
//{
//    //向一组属性的容器中添加一行属性 左侧没有属性名
//    QWidget *w = new QWidget(container);
//    QHBoxLayout *l = new QHBoxLayout(w);
//    l->setMargin(0);
//    //整行factory
//    factory->setParent(w);
//    l->addWidget(factory);
//    container->layout()->addWidget(w);
//}

//LineEditFactory *GroupManager::addProperty(QString name, float value)
//{
//    //向一组属性的容器中添加一行LineEditFactory
//    LineEditFactory *lineEditFactory = new LineEditFactory(container, name, value, 1, 1);
//    container->layout()->addWidget(lineEditFactory);
//    return lineEditFactory;
//}

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
//    //布局
//    QVBoxLayout *l = new QVBoxLayout(this);
//    l->setMargin(0);
//    //图标和信息的水平布局
//    QWidget *baseInfoWidget = new QWidget(this);
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(baseInfoWidget);
//    baseInfoLayout->setContentsMargins(10, 0, 0, 0);
//    icon = new QLabel(baseInfoWidget);
//    icon->setFocusPolicy(Qt::NoFocus);
//    setIcon();
//    baseInfoLayout->addWidget(icon, 1, Qt::AlignLeft);
//    //右侧具体数据
//    QWidget *infoWidget = new QWidget(baseInfoWidget);
//    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
//    infoLayout->setMargin(0);
//    //右上文件名
//    QLabel *labelName = new QLabel(infoWidget);
//    //labelName->setAlignment(Qt::AlignRight);
//    QString name = filePath.mid(filePath.lastIndexOf('/') + 1);
//    name = name.mid(0, name.lastIndexOf('.'));
//    labelName->setText(name);
//    labelName->setStyleSheet("font: 75 11pt \"Arial\";");
//    infoLayout->addWidget(labelName, 1);
//    //右下的 选择shader类型
//    QStringList shaderType;
//    shaderType.append(Material::StandardShader);
//    shaderType.append(Material::BlinByVertexShader);
//    EnumFactory *enumFactory = new EnumFactory("Type", shaderType, infoWidget, material->getShaderType());
//    infoLayout->addWidget(enumFactory);
//    baseInfoLayout->addWidget(infoWidget, 3);
//    l->addWidget(baseInfoWidget);
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

//void MaterialManager::setIcon()
//{
//    QPixmap pix = material->getPreview();
//    //和两行差不多高
//    pix.setDevicePixelRatio(devicePixelRatio());
//    pix = pix.scaled(30 * devicePixelRatio(), 30 * devicePixelRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon->setPixmap(pix);
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


////------------------PointLightManager---------------------


//PointLightManager::PointLightManager(PointLight *light, QWidget *parent, QString name, QString iconPath)
//    : GroupManager(parent, name, iconPath)
//{
//    mLight = light;
//    LightColorFactory *colorFactory = new LightColorFactory(light);
//    addProperty("Color", colorFactory, 1);
//    LineEditFactory *constantFactory = addProperty("Constant Factor", light->getConstantFactor());
//    LineEditFactory *linearFactory = addProperty("Linear Factor", light->getLinearFactor());
//    LineEditFactory *quadricFactory = addProperty("Quadric Factor", light->getQuadricFactor());
//    QObject::connect(constantFactory, SIGNAL(valueChanged(float)), this, SLOT(changeConstantFactory(float)));
//    QObject::connect(linearFactory, SIGNAL(valueChanged(float)), this, SLOT(changeLinearFactory(float)));
//    QObject::connect(quadricFactory, SIGNAL(valueChanged(float)), this, SLOT(changeQuadricFactory(float)));
//    //限制因子编辑框的取值范围
//    constantFactory->setRegExp("^([0]|[1-9][0-9]{0,0})(?:\\.\\d{1,3})?$|(^\\t?$)");
//    linearFactory->setRegExp("^([0]|[1-9][0-9]{0,0})(?:\\.\\d{1,3})?$|(^\\t?$)");
//    quadricFactory->setRegExp("^([0]|[1-9][0-9]{0,0})(?:\\.\\d{1,3})?$|(^\\t?$)");
//    //限制因子滑动的取值范围
//    constantFactory->setSliderRange(0, 10);
//    linearFactory->setSliderRange(0, 10);
//    quadricFactory->setSliderRange(0, 10);
//    //设置因子滑动的步长
//    constantFactory->setSliderStep(0.005f);
//    linearFactory->setSliderStep(0.005f);
//    quadricFactory->setSliderStep(0.005f);

//    SliderFactory *intensitySliderFactory = new SliderFactory(light->getIntensity(), 1, 10);
//    addProperty("Intensity", intensitySliderFactory);
//    QObject::connect(intensitySliderFactory, SIGNAL(valueChangedSignal(int)), this, SLOT(changeIntensity(int)));
//}

//void PointLightManager::changeConstantFactory(float value)
//{
//    mLight->setConstantFactor(value);
//}

//void PointLightManager::changeLinearFactory(float value)
//{
//    mLight->setLinearFactor(value);
//}

//void PointLightManager::changeQuadricFactory(float value)
//{
//    mLight->setQuadricFactor(value);
//}

//void PointLightManager::changeIntensity(int value)
//{
//    mLight->setIntensity(value);
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


//void PropertyManager::bindPreview(BaseOpenGLWidget *preview)
//{
//    mPreview = preview;
//}

//void PropertyManager::showMaterialProperty(QString filePath)
//{
//    clear();
//    if (!filePath.isNull())
//    {
//        //currentObject = new Object();
//        //显示材质的属性
//        MaterialManager *materialManager = new MaterialManager(this, filePath, mPreview);
//        layout()->addWidget(materialManager);
//        QObject::connect(materialManager, SIGNAL(updatePreviewInOtherWidget(QString)),
//                         this, SLOT(updateMaterialPreviewInOtherWidget(QString)));
//    }
//}

//void PropertyManager::showFbxProperty(QString filePath)
//{
//    clear();
//    if (!filePath.isNull())
//    {
//        FbxFileManager *fbxFileManager = new FbxFileManager(filePath, this);
//        layout()->addWidget(fbxFileManager);
//    }
//}

//void PropertyManager::showSceneProperty(Scene *scene)
//{
//    clear();
//    SceneManager *sceneManager = new SceneManager(scene, this);
//    layout()->addWidget(sceneManager);
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
