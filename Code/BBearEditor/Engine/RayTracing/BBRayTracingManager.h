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
    void renderOnePixel(int nPixelIndex);

private:
    BBScene *m_pScene;
    int m_nTotalPixelCount;
    int m_nCurrentRenderedPixel;
};

#endif // BBRAYTRACINGMANAGER_H
