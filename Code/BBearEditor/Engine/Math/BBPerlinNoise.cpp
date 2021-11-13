#include "BBPerlinNoise.h"
#include "BBMath.h"
#include <QDebug>

// http://www.realtimerendering.com/raytracing/Ray%20Tracing_%20The%20Next%20Week.pdf

QVector3D *BBPerlinNoise::m_pRandFloat = BBPerlinNoise::generateRandFloat();
int *BBPerlinNoise::m_pPermuteX = BBPerlinNoise::generatePermute();
int *BBPerlinNoise::m_pPermuteY = BBPerlinNoise::generatePermute();
int *BBPerlinNoise::m_pPermuteZ = BBPerlinNoise::generatePermute();

BBPerlinNoise::BBPerlinNoise()
{

}

float BBPerlinNoise::getNoise(const QVector3D &p)
{
    // Version 1: Chessboard
//    int i = int(16 * p.x()) & 255;
//    int j = int(16 * p.y()) & 255;
//    int k = int(16 * p.z()) & 255;
//    return m_pRandFloat[m_pPermuteX[i] ^ m_pPermuteY[j] ^ m_pPermuteZ[k]];

    // Version 2: smoothing
//    float u = p.x() - floor(p.x());
//    float v = p.y() - floor(p.y());
//    float w = p.z() - floor(p.z());
//    int i = floor(p.x());
//    int j = floor(p.y());
//    int k = floor(p.z());
//    float c[2][2][2];
//    for (int di = 0; di < 2; di++)
//    {
//        for (int dj = 0; dj < 2; dj++)
//        {
//            for (int dk = 0; dk < 2; dk++)
//            {
//                c[di][dj][dk] = m_pRandFloat[m_pPermuteX[(i + di) & 255] ^ m_pPermuteY[(j + dj) & 255] ^ m_pPermuteZ[(k + dk) & 255]];
//            }
//        }
//    }
//    return trilinearInterpolate(c, u, v, w);

    // Version 3: obvious grid features, use a hermite cubic to round off the interpolation
    float u = p.x() - floor(p.x());
    float v = p.y() - floor(p.y());
    float w = p.z() - floor(p.z());
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    QVector3D c[2][2][2];
    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                c[di][dj][dk] = m_pRandFloat[m_pPermuteX[(i + di) & 255] ^ m_pPermuteY[(j + dj) & 255] ^ m_pPermuteZ[(k + dk) & 255]];
            }
        }
    }
    return trilinearInterpolate(c, u, v, w) * 0.5f + 0.5f;
}

/**
 * @brief BBPerlinNoise::generateTurbulence                 Used directly, turbulence gives a sort of camouflage netting appearance
 * @param p
 * @param nDepth
 * @return
 */
float BBPerlinNoise::getTurbulenceNoise(const QVector3D &p, int nDepth)
{
    float sum = 0.0f;
    QVector3D tmp = p;
    float w = 1.0f;
    for (int i = 0; i < nDepth; i++)
    {
        sum += w * getNoise(tmp);
        w *= 0.5f;
        tmp *= 2.0f;
    }
    return fabs(sum);
}

QVector3D* BBPerlinNoise::generateRandFloat()
{
    QVector3D *p = new QVector3D[256];
    for (int i = 0; i < 256; i++)
    {
        // irregular directions
        p[i] = QVector3D(sfrandom(), sfrandom(), sfrandom()).normalized();
    }
    return p;
}

int* BBPerlinNoise::generatePermute()
{
    int *p = new int[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

void BBPerlinNoise::permute(int *p, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = int(frandom() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}
