#ifndef BBGLOBALILLUMINATION_H
#define BBGLOBALILLUMINATION_H


class BBScene;

class BBGlobalIllumination
{
public:
    static void enable(bool bEnable);

private:
    static void setRenderedObjectPass(BBScene *pScene);
    static void setScreenQuadPass(BBScene *pScene);
};

#endif // BBGLOBALILLUMINATION_H