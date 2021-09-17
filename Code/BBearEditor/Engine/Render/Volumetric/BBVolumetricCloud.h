#ifndef BBVOLUMETRICCLOUD_H
#define BBVOLUMETRICCLOUD_H


class BBScene;

class BBVolumetricCloud
{
public:
    static void enable(int nAlgorithmIndex, bool bEnable);

    static void open(BBScene *pScene);

    static void setGBufferPass(BBScene *pScene);
    static void setScreenQuadPass(BBScene *pScene);
};

#endif // BBVOLUMETRICCLOUD_H
