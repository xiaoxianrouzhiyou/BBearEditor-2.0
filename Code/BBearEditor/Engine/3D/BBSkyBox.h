#ifndef BBSKYBOX_H
#define BBSKYBOX_H


#include "Base/BBRenderableObject.h"


class BBSkyBoxSide : public BBRenderableObject
{
public:
    BBSkyBoxSide(BBVertexBufferObject *pVBO);

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void change(const QString &path);
};


class BBSkyBox : public BBGameObject
{
public:
    BBSkyBox();
    virtual ~BBSkyBox();

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void change(const QString &path);

    void setVisibility(bool bVisible) override;

private:
    void initFront();
    void initBack();
    void initLeft();
    void initRight();
    void initTop();
    void initBottom();

    BBSkyBoxSide *m_pFront;
    BBSkyBoxSide *m_pBack;
    BBSkyBoxSide *m_pLeft;
    BBSkyBoxSide *m_pRight;
    BBSkyBoxSide *m_pTop;
    BBSkyBoxSide *m_pBottom;
};


#endif // BBSKYBOX_H
