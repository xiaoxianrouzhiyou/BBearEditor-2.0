#ifndef BBSSAOGLOBALILLUMINATION_H
#define BBSSAOGLOBALILLUMINATION_H


#include <vector>
#include <QVector3D>

class BBScene;

class BBSSAOGlobalIllumination
{
public:
    static void open(BBScene *pScene);

    static void setRenderedObjectPass(BBScene *pScene);
    static void setSSAOPass(BBScene *pScene);
    static void setSSAOBlurPass(BBScene *pScene);

    static float* generateKernel();
    static float* generateNoise();
};

#endif // BBSSAOGLOBALILLUMINATION_H
