#ifndef BBSKYBOX_H
#define BBSKYBOX_H


#include "BBRenderableObject.h"

class BBSkyBoxSide : public BBRenderableObject
{
public:
    BBSkyBoxSide(BBGLVertexBuffer *pVertexBuffer);
    void init(const QString path) override;
    void render(BBCamera *pCamera) override;
//    void change(QString path);

private:
    void draw() override;
};


class BBSkyBox
{
public:
    BBSkyBox();
};



//class Skybox : public GameObject
//{
//public:
//    Skybox();
//    void init(QString path);
//    void render(Camera camera) override;
//    void resize(float width, float height) override;
//    void change(QString path);

//private:
//    SkyElement *front;
//    SkyElement *back;
//    SkyElement *left;
//    SkyElement *right;
//    SkyElement *top;
//    SkyElement *bottom;
//    void initFront();
//    void initBack();
//    void initLeft();
//    void initRight();
//    void initTop();
//    void initBottom();

//};


#endif // BBSKYBOX_H
