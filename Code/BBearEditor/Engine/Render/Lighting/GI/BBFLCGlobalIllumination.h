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

private:
    static void clear(BBScene *pScene);

    static float* generateS();
    static float* generateSp(float *pS);
    static float* generateNoise();


    static float* m_S;
    static float* m_Sp;
    static float* m_Noise;
    static int m_nNoiseCount;
    // Divided into N stacks according to the size of the triangle
    static int m_SNum;
    static BBShaderStorageBufferObject *m_pTriangleCutSSBOSet;
    static int m_nSSBOCount;
    static BBAtomicCounterBufferObject *m_pTriangleIdACBO;
};

#endif // BBFLCGLOBALILLUMINATION_H
