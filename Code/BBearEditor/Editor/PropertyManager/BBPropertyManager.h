#ifndef BBPROPERTYMANAGER_H
#define BBPROPERTYMANAGER_H

#include <QWidget>


class BBBaseInformationManager;
class BBSetBaseInformationManager;
class BBGroupManager;
class BBTransformGroupManager;
class BBGameObject;
class BBScene;
class BBOfflineRenderer;
class BBPreviewOpenGLWidget;


class BBPropertyManager : public QWidget
{
    Q_OBJECT

public:
    explicit BBPropertyManager(QWidget *pParent = nullptr);
    ~BBPropertyManager();

public slots:
    void clear();
    inline void bindPreviewOpenGLWidget(BBPreviewOpenGLWidget *pWidget) { m_pPreviewOpenGLWidget = pWidget; }
    void showGameObjectProperty(BBGameObject *pGameObject);
    void showGameObjectSetProperty(BBGameObject *pCenterGameObject,
                                   const QList<BBGameObject*> &gameObjectSet);
    void showGlobalSettingsProperty(BBScene *pScene);
    void showOfflineRendererProperty(BBOfflineRenderer *pOfflineRenderer);
    void showMaterialProperty(const QString &filePath);

    void updateCoordinateSystem();
    void updateTransform(BBGameObject *pGameObject, char transformModeKey);

    void updateFileList();

signals:
    void coordinateSystemUpdated();
    void fileListUpdated();

private:
    void setWidgetStyle();

    void addBaseInformationManager(BBGameObject *pGameObject);
    void addSetBaseInformationManager(BBGameObject *pCenterGameObject,
                                      const QList<BBGameObject*> &gameObjectSet);
    BBGroupManager* addGroupManager(const QString &name, const QString &iconPath);
    void addTransformGroupManager(BBGameObject *pGameObject);
    void addGlobalSettingsGroupManager(BBScene *pScene);
    void addOfflineRendererManager(BBOfflineRenderer *pOfflineRenderer);
    void addMaterialGroupManager(const QString &filePath);

    BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
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
