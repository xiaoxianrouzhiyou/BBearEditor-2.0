#ifndef BBSSDOGLOBALILLUMINATION_H
#define BBSSDOGLOBALILLUMINATION_H


class BBScene;

class BBSSDOGlobalIllumination
{
public:
    static void open(BBScene *pScene);
    static void setSSDOPass(BBScene *pScene);
    static void setSSDOBlurPass(BBScene *pScene);
};

#endif // BBSSDOGLOBALILLUMINATION_H
