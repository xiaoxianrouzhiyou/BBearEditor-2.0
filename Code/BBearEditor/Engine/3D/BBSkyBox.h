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

    void writeCubeMap(BBCamera *pCamera, int nSideIndex);

    void changeResource(const QString &path);
    void changeAlgorithm(int nIndex);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }

private:
    void initFrom6Map();
    void initFromHDREnvironmentMap();
    void initEnvironmentMapMaterial();

    QString m_SkyBoxFilePath;
    BBMaterial *m_pEnvironmentMapMaterial;
    QMatrix4x4 m_EnvironmentMapProjectionMatrix;
    QMatrix4x4 m_EnvironmentMapViewMatrix[6];
};


#endif // BBSKYBOX_H
