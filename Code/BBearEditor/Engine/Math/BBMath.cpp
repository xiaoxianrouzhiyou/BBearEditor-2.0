#include <stdlib.h>
#include "BBMath.h"


float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


QVector2D lerp(QVector2D a, QVector2D b, float f)
{
    return QVector2D(lerp(a.x(), b.x(), f), lerp(a.y(), b.y(), f));
}


QVector2D lerp(QVector2D a, QVector2D b, QVector2D c, float u, float v)
{
    // Any point on the plane can be expressed as P = A +  u * (C – A) + v * (B - A)
    // u >= 0 v >= 0 u + v <= 1, point is within the triangle
    return lerp(a, c, u) + lerp(a, b, v);
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
QVector3D hemisphericalRandom(QVector3D normal)
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
