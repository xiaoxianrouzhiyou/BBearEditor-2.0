#ifndef BBOFFLINERENDERER_H
#define BBOFFLINERENDERER_H


class BBScene;
class BBModel;

class BBOfflineRenderer
{
public:
    BBOfflineRenderer(BBScene *pScene);
    virtual ~BBOfflineRenderer();

    void createTestModels();

private:
    BBScene *m_pScene;
    BBModel *m_pModels[3];
};

#endif // BBOFFLINERENDERER_H
