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
    // obvious grid features, use a hermite cubic to round off the interpolation
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    float c[2][2][2];
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

    return trilinearInterpolate(c, u, v, w);
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
