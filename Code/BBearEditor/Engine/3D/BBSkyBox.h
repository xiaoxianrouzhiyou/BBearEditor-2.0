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

    void writeCubeMap(BBCamera *pCamera);

    void changeResource(const QString &path);
    void changeAlgorithm(int nIndex);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }

public:
    static int m_nEnvironmentMapSize;

private:
    void initFrom6Map();
    void initFromHDREnvironmentMap();
    void initFromEnvironmentCubeMap();

    void initEnvironmentMapSettings();

    QString m_SkyBoxFilePath;
    GLuint m_EnvironmentMap;
    BBMaterial *m_pEnvironmentMapMaterial;
    QMatrix4x4 m_EnvironmentMapProjectionMatrix;
    QMatrix4x4 m_EnvironmentMapViewMatrix[6];
};


#endif // BBSKYBOX_H
