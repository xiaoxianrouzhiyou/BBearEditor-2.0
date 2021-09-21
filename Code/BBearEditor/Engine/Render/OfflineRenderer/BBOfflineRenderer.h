#ifndef BBOFFLINERENDERER_H
#define BBOFFLINERENDERER_H


#include <QImage>

#define TestModelCount 8

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
    QImage renderFrame();
    void showFrame(const QImage &image);

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
};

#endif // BBOFFLINERENDERER_H
