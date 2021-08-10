#ifndef BBSKYBOX_H
#define BBSKYBOX_H


#include "Base/BBRenderableObject.h"

class BBSkyBox : public BBRenderableObject
{
public:
    BBSkyBox();
    ~BBSkyBox();

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void change(const QString &path);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }

private:
    QString m_SkyBoxFilePath;
};


#endif // BBSKYBOX_H
