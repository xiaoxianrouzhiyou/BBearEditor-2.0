#ifndef BBOFFLINERENDERER_H
#define BBOFFLINERENDERER_H


#define TestModelCount 7

class BBScene;
class BBModel;

class BBOfflineRenderer
{
public:
    BBOfflineRenderer(BBScene *pScene);
    virtual ~BBOfflineRenderer();

    void createTestScene();
    void generatePhotonMap();

private:
    BBScene *m_pScene;
    BBModel *m_pModels[TestModelCount];
};

#endif // BBOFFLINERENDERER_H
