#include "BBPerlinNoise.h"
#include "BBMath.h"


float *BBPerlinNoise::m_pRandFloat = BBPerlinNoise::generateRandFloat();
int *BBPerlinNoise::m_pPermuteX = BBPerlinNoise::generatePermute();
int *BBPerlinNoise::m_pPermuteY = BBPerlinNoise::generatePermute();
int *BBPerlinNoise::m_pPermuteZ = BBPerlinNoise::generatePermute();

BBPerlinNoise::BBPerlinNoise()
{

}

float BBPerlinNoise::getNoise(const QVector3D &p)
{
    float u = p.x() - floor(p.x());
    float v = p.y() - floor(p.y());
    float w = p.z() - floor(p.z());
    int i = int(4 * p.x()) & 255;
    int j = int(4 * p.y()) & 255;
    int k = int(4 * p.z()) & 255;
    return m_pRandFloat[m_pPermuteX[i] ^ m_pPermuteY[j] ^ m_pPermuteZ[k]];
}

float* BBPerlinNoise::generateRandFloat()
{
    float *p = new float[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = frandom();
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
