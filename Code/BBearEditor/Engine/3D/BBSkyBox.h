#ifndef BBSKYBOX_H
#define BBSKYBOX_H


#include "Base/BBRenderableObject.h"


class BBSkyBoxSide : public BBRenderableObject
{
public:
    BBSkyBoxSide(BBVertexBufferObject *pVBO);

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
};


class BBSkyBox : public BBRenderableObject
{
public:
    BBSkyBox();
    ~BBSkyBox();

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void change(const QString &path);
};


#endif // BBSKYBOX_H
