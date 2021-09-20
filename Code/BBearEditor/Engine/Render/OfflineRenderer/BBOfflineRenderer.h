#ifndef BBOFFLINERENDERER_H
#define BBOFFLINERENDERER_H


#define TestModelCount 8

class BBScene;
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
    void generatePhotonMap();
    void showPhotonMap();

private:
    BBScene *m_pScene;
    int m_nWidth;
    int m_nHeight;
    BBModel *m_pModels[TestModelCount];
    BBAreaLight *m_pAreaLight;
    BBPhotonMap *m_pPhotonMap;
};

#endif // BBOFFLINERENDERER_H
