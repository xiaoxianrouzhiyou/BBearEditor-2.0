#ifndef BBPERLINNOISE_H
#define BBPERLINNOISE_H


#include <QVector3D>

class BBPerlinNoise
{
public:
    BBPerlinNoise();

    static float getNoise(const QVector3D &p);
    static float getTurbulenceNoise(const QVector3D &p, int nDepth = 7);

private:
    static QVector3D *generateRandFloat();
    static int* generatePermute();
    static void permute(int *p, int n);

    static QVector3D *m_pRandFloat;
    static int *m_pPermuteX;
    static int *m_pPermuteY;
    static int *m_pPermuteZ;
};

#endif // BBPERLINNOISE_H
