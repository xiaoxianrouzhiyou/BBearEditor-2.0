#ifndef BBGLOBALILLUMINATION_H
#define BBGLOBALILLUMINATION_H


class BBScene;

class BBGlobalIllumination
{
public:
    static void enable(bool bEnable);

private:
    static void useSSAOAlgorithm(BBScene *pScene);
};

#endif // BBGLOBALILLUMINATION_H
