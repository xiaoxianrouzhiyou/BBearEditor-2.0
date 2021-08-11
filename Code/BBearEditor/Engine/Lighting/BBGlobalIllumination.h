#ifndef BBGLOBALILLUMINATION_H
#define BBGLOBALILLUMINATION_H


class BBScene;

class BBGlobalIllumination
{
public:
    static void open();
    static void close();

private:
    static void setRenderedObjectPass(BBScene *pScene);
    static void setScreenQuadPass(BBScene *pScene);
};

#endif // BBGLOBALILLUMINATION_H
