#ifndef BBOFFLINERENDERER_H
#define BBOFFLINERENDERER_H


#include <QImage>

#define TestModelCount 9

class BBScene;
class BBMaterial;
class BBModel;
class BBAreaLight;
class BBPhotonMap;

class BBOfflineRenderer
{
public:
    BBOfflineRenderer(BBScene *pScene);
    virtual ~BBOfflineRenderer();

    void createTestScene();
    void startPhotonMapping();

private:
    void renderFrame();
    void showFrame();

    void generatePhotonMap();
    void showPhotonMap();

private:
    BBScene *m_pScene;
    BBMaterial *m_pMaterial;
    int m_nWidth;
    int m_nHeight;
    BBModel *m_pModels[TestModelCount];
    BBAreaLight *m_pAreaLight;
    BBPhotonMap *m_pPhotonMap;
    QImage m_CurrentImage;
    int m_nBlendFrameCount;
};

#endif // BBOFFLINERENDERER_H
