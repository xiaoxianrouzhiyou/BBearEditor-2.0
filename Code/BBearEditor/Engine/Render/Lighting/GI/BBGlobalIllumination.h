#ifndef BBGLOBALILLUMINATION_H
#define BBGLOBALILLUMINATION_H


class BBScene;

class BBGlobalIllumination
{
public:
    static void enable(int nAlgorithmIndex, bool bEnable);

    static void setGBufferPass(BBScene *pScene);
    static void setGBufferPassByUsingExtraMaterial(BBScene *pScene);
};

#endif // BBGLOBALILLUMINATION_H
