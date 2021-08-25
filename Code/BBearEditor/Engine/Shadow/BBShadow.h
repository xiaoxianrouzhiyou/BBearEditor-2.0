#ifndef BBSHADOW_H
#define BBSHADOW_H


class BBScene;

class BBShadow
{
public:
    static void enable(int nAlgorithmIndex, bool bEnable);
    static void open(BBScene *pScene);
};

#endif // BBSHADOW_H
