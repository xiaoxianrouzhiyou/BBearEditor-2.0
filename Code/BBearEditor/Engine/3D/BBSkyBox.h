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

    void writeEnvironmentMap(BBCamera *pCamera);
    void writeIrradianceMap(BBCamera *pCamera);

    void changeResource(const QString &path);
    void changeAlgorithm(int nIndex);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }
    GLuint getIrradianceMap() { return m_IrradianceMap; }

public:
    static int m_nEnvironmentMapSize;
    static int m_nIrradianceMapSize;

private:
    void initFrom6Map();
    void initFromHDREnvironmentMap();
    void initFromEnvironmentCubeMap();

    void initIBLSettings();

    QString m_SkyBoxFilePath;
    GLuint m_EnvironmentMap;
    GLuint m_IrradianceMap;
    BBMaterial *m_pEnvironmentMapMaterial;
    BBMaterial *m_pIrradianceMapMaterial;
    QMatrix4x4 m_IBLCubeMapProjectionMatrix;
    QMatrix4x4 m_IBLCubeMapViewMatrix[6];
};


#endif // BBSKYBOX_H
