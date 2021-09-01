#ifndef BBFLCGLOBALILLUMINATION_H
#define BBFLCGLOBALILLUMINATION_H


class BBScene;

class BBFLCGlobalIllumination
{
public:
    static void open(BBScene *pScene);
    static void setTriangleCutPass(BBScene *pScene);
    static void setFullScreenQuadPass(BBScene *pScene);
};

#endif // BBFLCGLOBALILLUMINATION_H
