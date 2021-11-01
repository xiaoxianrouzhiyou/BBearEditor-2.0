#ifndef BBSKYBOX_H
#define BBSKYBOX_H


#include "Base/BBRenderableObject.h"

class BBFullScreenQuad;

class BBSkyBox : public BBRenderableObject
{
public:
    BBSkyBox();
    ~BBSkyBox();

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;

    void writeEnvironmentMap(BBCamera *pCamera);
    void writeIrradianceMap(BBCamera *pCamera);
    void writePrefilterMapMipmap(BBCamera *pCamera, int nMipLevel);
    void writeBRDFLUTTexture(BBCamera *pCamera, BBFullScreenQuad *pFullScreenQuad);

    void changeResource(const QString &path);
    void changeAlgorithm(int nIndex);

    QString getSkyBoxFilePath() { return m_SkyBoxFilePath; }
    GLuint getCommonSkyBoxCube() { return m_CommonSkyBoxCube; }
    GLuint getIrradianceMap() { return m_IrradianceMap; }
    GLuint getPrefilterMapMipmap() { return m_PrefilterMapMipmap; }
    GLuint getBRDFLUTTexture() { return m_BRDFLUTTexture; }

public:
    static int m_nEnvironmentMapSize;
    static int m_nIrradianceMapSize;
    static int m_nBaseMipmapSize;
    static int m_nMaxMipLevels;
    static int m_nBRDFLUTTextureSize;

private:
    void initFrom6Map();
    void initFromHDREnvironmentMap();
    void initFromEnvironmentCubeMap();

    void initIBLSettings();

    QString m_SkyBoxFilePath;

    GLuint m_CommonSkyBoxCube;
    GLuint m_EnvironmentMap;
    GLuint m_IrradianceMap;
    GLuint m_PrefilterMapMipmap;
    GLuint m_BRDFLUTTexture;

    BBMaterial *m_pEnvironmentMapMaterial;
    BBMaterial *m_pIrradianceMapMaterial;
    BBMaterial *m_pPrefilterMapMaterial;
    BBMaterial *m_pBRDFLUTTextureMaterial;

    QMatrix4x4 m_IBLCubeMapProjectionMatrix;
    QMatrix4x4 m_IBLCubeMapViewMatrix[6];
};


#endif // BBSKYBOX_H
