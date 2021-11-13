#include <stdlib.h>
#include "BBMath.h"
#include <QDebug>


float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


QVector2D lerp(const QVector2D &a, const QVector2D &b, float f)
{
    return QVector2D(lerp(a.x(), b.x(), f), lerp(a.y(), b.y(), f));
}


QVector2D lerp(const QVector2D &a, const QVector2D &b, const QVector2D &c, float u, float v)
{
    // Any point on the plane can be expressed as P = A +  u * (C – A) + v * (B - A)
    // u >= 0 v >= 0 u + v <= 1, point is within the triangle
    return lerp(a, c, u) + lerp(a, b, v);
}


float trilinearInterpolate(QVector3D c[2][2][2], float u, float v, float w)
{
    // Version 1
//    float sum = 0.0f;
//    for (int i = 0; i < 2; i++)
//    {
//        for (int j = 0; j < 2; j++)
//        {
//            for (int k = 0; k < 2; k++)
//            {
//                sum += (i * u + (1 - i) * (1 - u)) *
//                       (j * v + (1 - j) * (1 - v)) *
//                       (k * w + (1 - k) * (1 - w)) * c[i][j][k];
//            }
//        }
//    }

    // Version 2
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float sum = 0.0f;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                QVector3D weight(u - i, v - j, w - k);
                sum += (i * uu + (1 - i) * (1 - uu)) *
                       (j * vv + (1 - j) * (1 - vv)) *
                       (k * ww + (1 - k) * (1 - ww)) * QVector3D::dotProduct(c[i][j][k], weight);
            }
        }
    }
    sum = sum * 0.5f + 0.5f;
    return sum;
}


/**
 * @brief reflect
 * @param L                     incident light
 * @param N                     normal
 * @return
 */
QVector3D reflect(const QVector3D &L, const QVector3D &N)
{
    return L - 2.0f * QVector3D::dotProduct(L, N) * N;
}


bool refract(const QVector3D &L, const QVector3D &N, float fRefractivity, QVector3D &refracted)
{
    float dt = QVector3D::dotProduct(L, N);
    float discrimination = 1.0f - fRefractivity * fRefractivity * (1.0f - dt * dt);
    if (discrimination > 0.0f)
    {
        refracted = fRefractivity * (L - N * dt) - N * sqrtf(discrimination);
        return true;
    }
    return false;
}


/**
 * @brief frandom
 * @return                  0~1
 */
float frandom()
{
    return rand() / (float)RAND_MAX;
}


/**
 * @brief sfrandom
 * @return                  -1~1
 */
float sfrandom()
{
    return frandom() * 2.0f - 1.0f;
}


/**
 * @brief hemisphericalRandom       Generate a random vector on the hemisphere corresponding to a normal
 * @return
 */
QVector3D hemisphericalRandom(const QVector3D &normal)
{
    QVector3D v;
    float mode;
    do
    {
        v = QVector3D(sfrandom(), sfrandom(), sfrandom());
        mode = v.length();
    } while(mode >= 1.0f || mode == 0.0f || QVector3D::dotProduct(v, normal) < 0.0f);
    return v.normalized();
}


QVector3D sphericalRandom()
{
    QVector3D v;
    float mode;
    do
    {
        v = QVector3D(sfrandom(), sfrandom(), sfrandom());
        mode = v.length();
    } while(mode >= 1.0f || mode == 0.0f);
    return v.normalized();
}


// KD Tree
/**
 * @brief getMedian                 Find the location of the median, when you build a complete binary tree
 *                                  The all data can be divided into two parts, one is smaller than the median and the other is larger than the median
 * @param start
 * @param end
 * @return                          This position is the offset from the start
 */
int getMedian(int start, int end)
{
    int count = end - start + 1;
    int median;
    // Total number of nodes in the first k layer
    int nTotalNodeNum = 1;
    // Node number of each layer
    int nLayerNodeNum = 2;
    while (nTotalNodeNum < count)
    {
        nTotalNodeNum += nLayerNodeNum;
        nLayerNodeNum *= 2;
    }
    // full binary tree
    if (nTotalNodeNum == count)
    {
        return start + count / 2;
    }
    nLayerNodeNum /= 2;
    if (nTotalNodeNum - nLayerNodeNum / 2 < count)
    {
        // On the last layer, more than half, but not full binary tree
        return start + nTotalNodeNum / 2;
    }
    else
    {
        // On the last floor, no more than half
        return start + nTotalNodeNum / 2 - (nTotalNodeNum - nLayerNodeNum / 2 - count);
    }
}


/**
 * @brief schlick                   Schlick Fresnel approximation
 * @param cos
 * @param fRefractivity
 * @return
 */
float schlick(float cos, float fRefractivity)
{
    float f0 = (1 - fRefractivity) / (1 + fRefractivity);
    f0 *= f0;
    return f0 + (1 - f0) * pow((1 - cos), 5);
}



float radians(float angle)
{
    return angle * PI / 180.0f;
}
