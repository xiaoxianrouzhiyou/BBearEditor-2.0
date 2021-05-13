#ifndef BBSCENE_H
#define BBSCENE_H


class BBScene
{
public:
    BBScene();
    ~BBScene();

    void init();
    void render();
    void resize(float width, float height);

private:
    float m_fUpdateRate;
//    Skybox *skybox;
//    QList<GameObject*> models;
//    QList<GameObject*> directionLights;
//    QList<GameObject*> pointLights;
//    QList<GameObject*> spotLights;
//    QList<GameObject*> audios;
//    HorizontalPlane *horizontalPlane;
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
};

#endif // BBSCENE_H
