#ifndef BBPERLINNOISE_H
#define BBPERLINNOISE_H


#include <QVector3D>

class BBPerlinNoise
{
public:
    BBPerlinNoise();

    static float getNoise(const QVector3D &p, float fScale = 1.0f);
    static float generateTurbulence(const QVector3D &p, int nDepth = 7);

private:
    static float* generateRandFloat();
    static int* generatePermute();
    static void permute(int *p, int n);

    static float *m_pRandFloat;
    static int *m_pPermuteX;
    static int *m_pPermuteY;
    static int *m_pPermuteZ;
};

#endif // BBPERLINNOISE_H
