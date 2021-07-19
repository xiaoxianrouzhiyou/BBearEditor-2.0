#ifndef BBRAYTRACINGMANAGER_H
#define BBRAYTRACINGMANAGER_H


class BBScene;

class BBRayTracingManager
{
public:
    BBRayTracingManager(BBScene *pScene);

    void open();
    void close();
    void onWindowResize(int nWidth, int nHeight);

private:
    void updateWindow();

    void render();

private:
    BBScene *m_pScene;
    int m_nViewportWidth;
    int m_nViewportHeight;
};

#endif // BBRAYTRACINGMANAGER_H
