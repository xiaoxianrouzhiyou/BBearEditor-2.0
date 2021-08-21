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

    void changeResource(const QString &path);
    void changeAlgorithm(int nIndex);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }

private:
    QString m_SkyBoxFilePath;
};


#endif // BBSKYBOX_H
