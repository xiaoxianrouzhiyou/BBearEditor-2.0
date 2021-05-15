#ifndef BBSCENE_H
#define BBSCENE_H


#include <QList>


class BBCamera;
class BBSkyBox;
class BBHorizontalPlane;
class BBGameObject;
class BBScene
{
public:
    BBScene();
    virtual ~BBScene();

    void init();
    void render();
    void resize(float width, float height);

    inline BBCamera* getCamera() { return m_pCamera; }

private:
    void deleteGameObject(BBGameObject *pObject);

    float m_fUpdateRate;
    BBCamera *m_pCamera;
    BBSkyBox *m_pSkyBox;
    BBHorizontalPlane *m_pHorizontalPlane;

    QList<BBGameObject*> m_Models;

//    QList<GameObject*> directionLights;
//    QList<GameObject*> pointLights;
//    QList<GameObject*> spotLights;
//    QList<GameObject*> audios;

//    SelectionRegion *selectionRegion;
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
//    void changeSkybox(QString path);
//    Model *createModel(QString filePath, int x, int y, bool isSelect = true);
//    Model *createModel(QString filePath, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    Model *createModelForPreview(QString filePath, float distFactor = 2);
//    GameObject *pickObject(Ray ray, bool isSelect = true);
//    GameObject *createLight(QString fileName, int x, int y, bool isSelect = true);
//    GameObject *createLight(QString fileName, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    Audio *createAudio(QString filePath, int x, int y, bool isSelect = true);
//    Audio *createAudio(QString filePath, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    void lookAtGameObject(GameObject *object);
//    QList<GameObject*> setSelectionRegion(QPoint start, QPoint end);
//    void setSelectionRegionVisible(bool isVisible);
//    Model *pickModel(int x, int y);
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
//    TransformCoordinate *transformCoordinate;
};

#endif // BBSCENE_H
