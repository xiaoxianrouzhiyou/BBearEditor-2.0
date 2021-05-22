#ifndef BBPROPERTYMANAGER_H
#define BBPROPERTYMANAGER_H

#include <QWidget>

class BBPropertyManager : public QWidget
{
    Q_OBJECT
public:
    explicit BBPropertyManager(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // BBPROPERTYMANAGER_H


////悬浮变为调节大小的光标 按下后左右移动可调节大小
//class Label : public QPushButton
//{
//    Q_OBJECT

//public:
//    Label(QWidget *parent = nullptr);

//private:
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//    QPoint lastPos;
//    bool isPress;

//signals:
//    void slide(int deltaX);
//};


////左侧带有name的单行编辑框
//class LineEditFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    LineEditFactory(QWidget *parent, QString name, float value, int nameStretch = 0, int valueStretch = 0);
//    QString setValue(float value);
//    void setRegExp(const QString &pattern);
//    void setSliderRange(float min, float max);
//    void setSliderStep(float step);

//private slots:
//    void editTextChanged(QString text);
//    void slideChangeValue(int deltaX);
//    void showLeft();

//signals:
//    void valueChanged(float value);

//private:
//    Label *label;
//    QLineEdit *edit;
//    float sliderMaxValue;
//    float sliderMinValue;
//    float sliderStep;
//};

////由3个LineEditFactory组成的三维向量编辑框
//class Vector3DFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    Vector3DFactory(QWidget *parent, QVector3D value);
//    void setValue(QVector3D value);

//public slots:
//    void xChanged(float x);
//    void yChanged(float y);
//    void zChanged(float z);

//signals:
//    void valueChanged(QVector3D value);

//private:
//    LineEditFactory *xEdit;
//    LineEditFactory *yEdit;
//    LineEditFactory *zEdit;
//    QVector3D mValue;
//};


////左侧名字 右侧枚举框
//class EnumFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    EnumFactory(QString name, QStringList comboBoxItems, QWidget *parent = 0, QString currentText = "");

//signals:
//    void currentIndexChanged(const QString &text);

//private slots:
//    void currentIndexChangedSlot(const QString &text);

//private:
//    QLabel *label;
//    QComboBox *comboBox;
//};


////可以设置背景颜色的按钮
//class ColorButton : public QPushButton
//{
//    Q_OBJECT

//public:
//    ColorButton(QWidget *parent = 0);
//    void setColor(int r, int g, int b, int a = 255);
//    void setColor(QColor color);

//private slots:
//    void clickedSlot();

//private:
//    QWidget *contentBlack;
//    QWidget *contentWhite;
//};


////截取全屏幕的对话框 用于滴管取色
//class ScreenDialog : public QDialog
//{
//    Q_OBJECT

//public:
//    ScreenDialog(QWidget *parent = 0);

//signals:
//    void setColor(int r, int g, int b);

//private:
//    void setBackground();
//    void mousePressEvent(QMouseEvent *event) override;
//    QLabel *background;
//    QPixmap pixBackground;
//    static int cursorSize;
//};


////左侧滴管按钮 右侧颜色框
//class ColorFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    ColorFactory(QColor color = QColor(255, 255, 255), QWidget *parent = 0);

//protected:
//    ColorButton *buttonColor;

//signals:
//    void colorChanged(float r, float g, float b);

//protected slots:
//    void catchColor();
//    virtual void finishCatchColor(int r, int g, int b);
//};


//class MaterialColorFactory : public ColorFactory
//{
//    Q_OBJECT

//public:
//    MaterialColorFactory(QColor color, QString filePath, QWidget *parent = 0);

//signals:
//    void updateMaterialPreview();

//private slots:
//    void finishCatchColor(int r, int g, int b) override;

//private:
//    Material *material;
//};


//class LightColorFactory : public ColorFactory
//{
//    Q_OBJECT

//public:
//    LightColorFactory(Light *light, QWidget *parent = 0);

//private slots:
//    void finishCatchColor(int r, int g, int b) override;

//private:
//    Light *mLight;
//};


////可以拖入文件的 可以显示图标的控件 如显示纹理贴图 可以拖入图片替换
//class IconLabel : public QLabel
//{
//    Q_OBJECT

//public:
//    IconLabel(QWidget *parent = 0);
//    void setAcceptableSuffixs(QList<QString> acceptableSuffixs);
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dropEvent(QDropEvent *event) override;

//signals:
//    void changeValue(QString path);

//private:
//    QList<QString> mAcceptableSuffixs;
//};


////带有图标 归零按钮 选择列表按钮 可以拖入图标 的控件
//class IconFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    IconFactory(QWidget *parent = 0);

//protected:
//    QPushButton *buttonRemove;
//    QPushButton *buttonMore;
//    IconLabel *labelIcon;
//};


////设置纹理
//class TextureFactory : public IconFactory
//{
//    Q_OBJECT

//public:
//    TextureFactory(QString texturePath, QString materialPath, QWidget *parent = 0);

//signals:
//    void updateMaterialPreview();

//private slots:
//    void removeTexture();
//    void changeTexture(QString path);

//private:
//    void setTexture(QString texturePath);
//    Material *material;
//};


////对象的材质 可以拖放材质进行修改
//class MaterialFactory : public IconFactory
//{
//    Q_OBJECT

//public:
//    MaterialFactory(Model* model, QWidget *parent = 0);
//    void setMaterial();

//private slots:
//    void removeMaterial();
//    void changeMaterial(QString path);

//private:
//    Model* mModel;
//};


////高度图
//class HeightMapFactory : public IconFactory
//{
//    Q_OBJECT

//public:
//    HeightMapFactory(Model* model, QWidget *parent = 0);

//private slots:
//    void changeHeight(QString mapPath);

//private:
//    void showHeightMap();
//    Model* mModel;
//};


////左侧滑块 右侧滑块值编辑框
//class SliderFactory : public QWidget
//{
//    Q_OBJECT

//public:
//    SliderFactory(int value = 50, int min = 0, int max = 100, QWidget *parent = 0);
//    void setRange(int min, int max);
//    void setValue(int value);

//signals:
//    void valueChangedSignal(int value);

//private slots:
//    void valueChanged(int value);
//    void valueChanged(QString value);

//private:
//    int mMin;
//    int mMax;
//    QSlider *slider;
//    QLineEdit *valueEdit;
//};


//class AnimFactory : public QComboBox
//{
//    Q_OBJECT

//public:
//    AnimFactory(Model *model, BaseOpenGLWidget *preview, QWidget *parent = 0);

//private:
//    void showPopup() override;
//    void hidePopup() override;
//    bool eventFilter(QObject *watched, QEvent *event) override;
//    Model *mModel;
//    BaseOpenGLWidget *mPreview;
//    Model *mPreviewModel;

//private slots:
//    void changeCurrentAnim(int index);
//};


////管理一组属性
//class GroupManager : public QWidget
//{
//    Q_OBJECT

//public:
//    GroupManager(QWidget *parent, QString name, QString iconPath);
//    void addProperty(QString name, QWidget *factory, int stretch = 1);
//    void addProperty(QWidget *factory);
//    LineEditFactory *addProperty(QString name, float value);
//    void removeProperty(int index);

//protected:
//    QToolButton *mainButton;
//    QPushButton *settingButton;
//    QMenu *menu;
//    QWidget *container;

//public slots:
//    void containerTrigger(bool isTrigger);
//};


////管理名字 类名 可见性等基本信息
//class GameObjectBaseInfoManager : public QWidget
//{
//    Q_OBJECT

//public:
//    GameObjectBaseInfoManager(QWidget *parent, GameObject *gameObject);
//    void rename(QString newName);
//    void changeButtonActiveCheckState(bool isActive);

//private slots:
//    void finishRename();
//    virtual void changeActivation();

//signals:
//    void nameChanged(GameObject *gameObject);
//    void activationChanged(GameObject *gameObject, bool isActive);

//protected:
//    QPushButton *buttonActive;
//    QLineEdit *editName;
//    GameObject *mGameObject;
//};


////管理多个对象的集合的基本信息
//class GameObjectSetBaseInfoManager : public GameObjectBaseInfoManager
//{
//    Q_OBJECT

//public:
//    GameObjectSetBaseInfoManager(QWidget *parent, GameObject *gameObject, QList<GameObject*> gameObjects);

//private slots:
//    void changeActivation() override;

//signals:
//    void activationChanged(QList<GameObject*> gameObjects, bool isActive);

//private:
//    QList<GameObject*> mGameObjects;
//};


//enum ReferenceSystem
//{
//    Global = 0,
//    Local = 1
//};

////管理transform属性的一个属性组
//class TransformGroupManager : public GroupManager
//{
//    Q_OBJECT

//public:
//    TransformGroupManager(QWidget *parent, GameObject *gameObject);
//    void setPositionValue();
//    void setRotationValue();
//    void setScaleValue();

//public slots:
//    void positionChanged(QVector3D value);
//    void rotationChanged(QVector3D value);
//    void scaleChanged(QVector3D value);

//private slots:
//    void clickButtonSetting();
//    void switchToGlobal();
//    void switchToLocal();

//signals:
//    void updateCoordinate();

//private:
//    Vector3DFactory *positionFactory;
//    Vector3DFactory *rotationFactory;
//    Vector3DFactory *scaleFactory;
//    GameObject *mGameObject;
//    ReferenceSystem referenceSystem;
//};


////材质各项属性
//class MaterialManager : public QWidget
//{
//    Q_OBJECT

//public:
//    MaterialManager(QWidget *parent, QString filePath, BaseOpenGLWidget *previewWidget);

//signals:
//    void updatePreviewInOtherWidget(QString filePath);

//private slots:
//    void updatePreview();
//    void currentIndexChangedSlot(const QString &text);
//    void smoothnessChanged(int value);

//private:
//    void setIcon();
//    BaseOpenGLWidget *mPreviewWidget;
//    QString mFilePath;
//    Material *material;
//    QLabel *icon;
//};


////场景各项属性
//class SceneManager : public QWidget
//{
//    Q_OBJECT

//public:
//    SceneManager(Scene *scene, QWidget *parent = 0);

//private slots:
//    void switchFog(bool b);
//    void setFogColor(float r, float g, float b);
//    void switchMode(int id);
//    void setFogStart(float start);
//    void setFogEnd(float end);
//    void setFogDensity(int density);
//    void setFogPower(float power);

//private:
//    Scene *mScene;
//    GroupManager *fogManager;
//    LineEditFactory *startEditFactory;
//    LineEditFactory *endEditFactory;
//    SliderFactory *densitySliderFactory;
//    LineEditFactory *powerEditFactory;
//};


//class PointLightManager : public GroupManager
//{
//    Q_OBJECT

//public:
//    PointLightManager(PointLight *light, QWidget *parent, QString name, QString iconPath);

//protected slots:
//    void changeConstantFactory(float value);
//    void changeLinearFactory(float value);
//    void changeQuadricFactory(float value);
//    void changeIntensity(int value);

//private:
//    PointLight *mLight;
//};

//class SpotLightManager : public PointLightManager
//{
//    Q_OBJECT

//public:
//    SpotLightManager(SpotLight *light, QWidget *parent, QString name, QString iconPath);

//private slots:
//    void changeSpotAngle(float value);
//    void changeSpotLevel(int value);

//private:
//    SpotLight *mLight;

//};


////fbx文件属性
//class FbxFileManager : public QWidget
//{
//    Q_OBJECT

//public:
//    FbxFileManager(QString filePath, QWidget *parent = 0);

//private slots:

//private:
//};


////属性栏控件
//class PropertyManager : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit PropertyManager(QWidget *parent = nullptr);
//    void bindPreview(BaseOpenGLWidget *preview);

//private:
//    GroupManager *addGroupManager(QString name, QString iconPath);
//    void addGameObjectBaseInfoManager(GameObject *gameObject);
//    void addGameObjectSetBaseInfoManager(QList<GameObject*> gameObjects, CenterPoint *center);
//    void addTransformManager(GameObject *gameObject);
//    void setWidgetStyle();
//    GameObject *currentObject;
//    GameObjectBaseInfoManager *gameObjectBaseInfoManager;
//    GameObjectSetBaseInfoManager *gameObjectSetBaseInfoManager;
//    TransformGroupManager *transformManager;
//    MaterialFactory *materialFactory;
//    BaseOpenGLWidget *mPreview;

//signals:
//    void updateCoordinateSignal();
//    void renameGameObjectInHierarchySignal(GameObject *gameObject);
//    void changeActivationSignal(GameObject *gameObject, bool isActive);
//    void setSelectedObjects(QList<GameObject*> gameObjects);
//    void updateMaterialPreviewInOtherWidgetSignal(QString filePath);

//public slots:
//    void showSceneProperty(Scene *scene);

//private slots:
//    void clear();
//    void showHierarchyTreeItemProperty(GameObject *gameObject);
//    void showHierarchyTreeItemsProperty(QList<GameObject*> gameObjects, CenterPoint *center);
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void updateTransform(GameObject *gameObject, char transformModeKey);
//    void updateCoordinate();
//    void renameGameObject(GameObject *gameObject, QString newName);
//    void renameGameObjectInHierarchy(GameObject *gameObject);
//    void changeActivation(GameObject *gameObject, bool isActive);
//    void changeActivation(QList<GameObject*> gameObjects, bool isActive);
//    void changeButtonActiveCheckState(GameObject *gameObject, bool isActive);
//    void updateMaterialPreviewInOtherWidget(QString filePath);
//    void updateMaterialFactory(Model* model);

//};
