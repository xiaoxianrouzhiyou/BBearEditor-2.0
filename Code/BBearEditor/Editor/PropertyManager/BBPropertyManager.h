#ifndef BBPROPERTYMANAGER_H
#define BBPROPERTYMANAGER_H

#include <QWidget>


class BBBaseInformationManager;
class BBSetBaseInformationManager;
class BBGroupManager;
class BBTransformGroupManager;
class BBGameObject;


class BBPropertyManager : public QWidget
{
    Q_OBJECT

public:
    explicit BBPropertyManager(QWidget *pParent = nullptr);
    ~BBPropertyManager();

private slots:
    void clear();
    void showGameObjectProperty(BBGameObject *pGameObject);
    void showGameObjectSetProperty(BBGameObject *pCenterGameObject,
                                   const QList<BBGameObject*> &gameObjectSet);
    void updateCoordinateSystem();
    void updateTransform(BBGameObject *pGameObject, char transformModeKey);

signals:
    void coordinateSystemUpdated();

private:
    void setWidgetStyle();

    void addBaseInformationManager(BBGameObject *pGameObject);
    void addSetBaseInformationManager(BBGameObject *pCenterGameObject,
                                      const QList<BBGameObject*> &gameObjectSet);
    BBGroupManager* addGroupManager(const QString &name, const QString &iconPath);
    void addTransformGroupManager(BBGameObject *pGameObject);

    BBBaseInformationManager *m_pBaseInformationManager;
    BBSetBaseInformationManager *m_pSetBaseInformationManager;
    BBTransformGroupManager *m_pTransformGroupManager;
    BBGameObject *m_pCurrentGameObject;
};

#endif // BBPROPERTYMANAGER_H










//class PropertyManager : public QWidget
//{
//public:
//    void bindPreview(BaseOpenGLWidget *preview);

//private:
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
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void renameGameObject(GameObject *gameObject, QString newName);
//    void renameGameObjectInHierarchy(GameObject *gameObject);
//    void changeActivation(GameObject *gameObject, bool isActive);
//    void changeActivation(QList<GameObject*> gameObjects, bool isActive);
//    void changeButtonActiveCheckState(GameObject *gameObject, bool isActive);
//    void updateMaterialPreviewInOtherWidget(QString filePath);
//    void updateMaterialFactory(Model* model);

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
