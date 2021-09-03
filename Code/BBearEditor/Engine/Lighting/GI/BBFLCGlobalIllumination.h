#ifndef BBFLCGLOBALILLUMINATION_H
#define BBFLCGLOBALILLUMINATION_H


class BBScene;
class BBShaderStorageBufferObject;
class BBAtomicCounterBufferObject;

class BBFLCGlobalIllumination
{
public:
    static void open(BBScene *pScene);
    static void setTriangleCutPass(BBScene *pScene);
    static void setIndirectShadingPass(BBScene *pScene);
    static void setFullScreenQuadPass(BBScene *pScene);

private:
    static void clear(BBScene *pScene);

    // Divided into N stacks according to the size of the triangle
    static int m_SNum;
    static BBShaderStorageBufferObject *m_pTriangleCutSSBOSet;
    static int m_nSSBOCount;
    static BBAtomicCounterBufferObject *m_pTriangleIdACBO;
};

#endif // BBFLCGLOBALILLUMINATION_H
