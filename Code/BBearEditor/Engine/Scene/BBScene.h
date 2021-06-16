#ifndef BBSCENE_H
#define BBSCENE_H


#include <QList>
#include <QVector3D>
#include <Serializer/BBGameObject.pb.h>


class BBDrawCall;
class BBCamera;
class BBSkyBox;
class BBHorizontalPlane;
class BBGameObject;
class BBModel;
class BBSelectionRegion;
class BBRay;
class BBTransformCoordinateSystem;
class BBScene
{
public:
    BBScene();
    virtual ~BBScene();

    void init();
    void render();
    void resize(float width, float height);

    inline BBCamera* getCamera() { return m_pCamera; }
    inline BBTransformCoordinateSystem* getTransformCoordinateSystem() { return m_pTransformCoordinateSystem; }

    void setSkyBox(const QString &path);

    BBModel* createModel(const QString &filePath, int x, int y);
    BBModel* createModel(const QString &filePath,
                         const QVector3D &position = QVector3D(0, 0, 0),
                         const QVector3D &rotation = QVector3D(0, 0, 0),
                         const QVector3D &scale = QVector3D(1, 1, 1));
    BBModel* createModel(const BBSerializer::BBGameObject &gameObject);
    BBModel* createModelForPreview(const QString &filePath, float fDistFactor = 2.0f);
//    BBModel* pickModel(int x, int y);
    BBGameObject* pickObject(const BBRay &ray, bool bSelect = true);
    void lookAtGameObject(BBGameObject *pGameObject);
    void deleteGameObject(BBGameObject *pGameObject);

    void setSelectionRegionVisibility(bool bVisible);
    QList<BBGameObject*> getSelectedObjects(QPoint start, QPoint end);

    void clear();

private:
    BBDrawCall *m_pDrawCall;

    float m_fUpdateRate;
    BBCamera *m_pCamera;
    BBSkyBox *m_pSkyBox;
    BBHorizontalPlane *m_pHorizontalPlane;
    BBTransformCoordinateSystem *m_pTransformCoordinateSystem;

    QList<BBGameObject*> m_Models;

    BBSelectionRegion *m_pSelectionRegion;

//    QList<GameObject*> directionLights;
//    QList<GameObject*> pointLights;
//    QList<GameObject*> spotLights;
//    QList<GameObject*> audios;


//    Particle *particle;
//    QVector4D *directionLightPosition;
//    QVector4D *directionLightColor;
//    QVector4D *pointLightPosition;
//    QVector4D *pointLightColor;
//    QVector4D *pointLightOption;
//    QVector4D *spotLightPosition;
//    QVector4D *spotLightDirection;
//    QVector4D *spotLightColor;
//    QVector4D *spotLightOption;
//    QVector4D *spotLightOption2;
//    bool fogSwitch;
//    QColor fogColor;
//    float fogStart;
//    float fogEnd;
//    int fogMode;
//    int fogDensity;
//    float fogPower;
//    //必须是指针 在opengl环境没有搭建起来 调用opengl的函数（构造函数中）会出错
//    FrameBufferObject *mFBO;


//    void renderShadowMap();
//    GameObject *createLight(QString fileName, int x, int y, bool isSelect = true);
//    GameObject *createLight(QString fileName, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    Audio *createAudio(QString filePath, int x, int y, bool isSelect = true);
//    Audio *createAudio(QString filePath, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    void lookAtGameObject(GameObject *object);
//    bool setModelMaterial(Model *model, QString mtlPath);
//    void updateDirectionLightPosition();
//    void updateDirectionLightColor();
//    void updatePointLightPosition();
//    void updatePointLightColor();
//    void updatePointLightOption();
//    void updateSpotLightPosition();
//    void updateSpotLightDirection();
//    void updateSpotLightColor();
//    void updateSpotLightOption();
//    void updateSpotLightOption2();
//    void switchFog(bool b);
//    float getFogSwitch();
//    void setFogColor(float r, float g, float b);
//    QColor getFogColor();
//    void setFogOption(float start, float end, int density, float power);
//    float getFogStart();
//    float getFogEnd();
//    int getFogDensity();
//    float getFogPower();
//    void setFogMode(int mode);
//    int getFogMode();
//    void play();
//    void stop();
//    void onKeyPress(QKeyEvent *e);
//    Sprite2D sprite;
//    GLuint particleTexture;
};

#endif // BBSCENE_H
