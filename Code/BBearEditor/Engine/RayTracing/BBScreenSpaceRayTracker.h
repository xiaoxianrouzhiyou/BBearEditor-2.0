#ifndef BBSCREENSPACERAYTRACKER_H
#define BBSCREENSPACERAYTRACKER_H


class BBScene;

class BBScreenSpaceRayTracker
{
public:
    static void open(BBScene *pScene);

private:
    static void setGBufferPass(BBScene *pScene);
    static void setRayTracingPass(BBScene *pScene);
};

#endif // BBSCREENSPACERAYTRACKER_H
