#include "BBMatrix.h"
#include <math.h>
#include <QDebug>
#include "Math/BBMath.h"


/**
 * @brief BBMatrix::SVDdecomposition
 * @param w                         Calculated singular value (eigenvalue)
 * @param u                         Left singular matrix (matrix with column eigenvector)
 * @param v                         Right singular matrix
 * @param eps                       Maximum allowable error
 */
void BBMatrix::SVDdecomposition(float w[3], float u[9], float v[9], float eps)
{
    bool flag;
    int i, its, j, jj, k, l, nm;
    float anorm, c, f, g, h, s, scale, x, y, z;
    float rv1[3];
    g = scale = anorm = 0.0;
    for (i = 0; i < 3; i++)
    {
        l = i + 2;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        for (k = i; k < 3; k++)
            scale += fabsf(u[k * 3 + i]);
        if (scale != 0.0)
        {
            for (k = i; k < 3; k++)
            {
                u[k * 3 + i] /= scale;
                s += u[k * 3 + i] * u[k * 3 + i];
            }
            f = u[i * 3 + i];
            g = -sign(sqrtf(s), f);
            h = f * g - s;
            u[i * 3 + i] = f - g;
            for (j = l - 1; j < 3; j++)
            {
                for (s = 0.0, k = i; k < 3; k++)
                    s += u[k * 3 + i] * u[k * 3 + j];
                f = s / h;
                for (k = i; k < 3; k++)
                    u[k * 3 + j] += f * u[k * 3 + i];
            }
            for (k = i; k < 3; k++)
                u[k * 3 + i] *= scale;
        }

        w[i] = scale * g;
        g = s = scale = 0.0;
        if (i + 1 <= 3 && i + 1 != 3)
        {
            for (k = l - 1; k < 3; k++)
                scale += fabsf(u[i * 3 + k]);
            if (scale != 0.0)
            {
                for (k = l - 1; k < 3; k++)
                {
                    u[i * 3 + k] /= scale;
                    s += u[i * 3 + k] * u[i * 3 + k];
                }
                f = u[i * 3 + l - 1];
                g = -sign(sqrtf(s), f);
                h = f * g - s;
                u[i * 3 + l - 1] = f - g;
                for (k = l - 1; k < 3; k++)
                    rv1[k] = u[i * 3 + k] / h;
                for (j = l - 1; j < 3; j++)
                {
                    for (s = 0.0, k = l - 1; k < 3; k++)
                        s += u[j * 3 + k] * u[i * 3 + k];
                    for (k = l - 1; k < 3; k++)
                        u[j * 3 + k] += s * rv1[k];
                }
                for (k = l - 1; k < 3; k++)
                    u[i * 3 + k] *= scale;
            }
        }
        anorm = max(anorm, (fabsf(w[i]) + fabsf(rv1[i])));
    }
    for (i = 2; i >= 0; i--)
    {
        if (i < 2)
        {
            if (g != 0.0)
            {
                for (j = l; j < 3; j++)
                {
                    v[j * 3 + i] = (u[i * 3 + j] / u[i * 3 + l]) / g;
                }
                for (j = l; j < 3; j++)
                {
                    for (s = 0.0, k = l; k < 3; k++)
                        s += u[i * 3 + k] * v[k * 3 + j];
                    for (k = l; k < 3; k++)
                        v[k * 3 + j] += s * v[k * 3 + i];
                }
            }
            for (j = l; j < 3; j++)
                v[i * 3 + j] = v[j * 3 + i] = 0.0;
        }
        v[i * 3 + i] = 1.0;
        g = rv1[i];
        l = i;
    }
    for (i = 2; i >= 0; i--)
    {
        l = i + 1;
        g = w[i];
        for (j = l; j < 3; j++)
            u[i * 3 + j] = 0.0;
        if (g != 0.0)
        {
            g = 1.0 / g;
            for (j = l; j < 3; j++)
            {
                for (s = 0.0, k = l; k < 3; k++)
                    s += u[k * 3 + i] * u[k * 3 + j];
                f = (s / u[i * 3 + i]) * g;
                for (k = i; k < 3; k++)
                    u[k * 3 + j] += f * u[k * 3 + i];
            }
            for (j = i; j < 3; j++)
                u[j * 3 + i] *= g;
        }
        else
        {
            for (j = i; j < 3; j++)
                u[j * 3 + i] = 0.0;
        }
        u[i * 3 + i]++;
    }
    for (k = 2; k >= 0; k--)
    {
        for (its = 0; its < 30; its++)
        {
            flag = true;
            for (l = k; l >= 0; l--)
            {
                nm = l - 1;
                if (l == 0 || fabsf(rv1[l]) <= eps * anorm)
                {
                    flag = false;
                    break;
                }
                if (fabsf(w[nm]) <= eps * anorm)
                    break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for (i = l; i < k + 1; i++)
                {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if (fabsf(f) <= eps * anorm)
                        break;
                    g = w[i];
                    h = pythag(f, g);
                    w[i] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for (j = 0; j < 3; j++)
                    {
                        y = u[j * 3 + nm];
                        z = u[j * 3 + i];
                        u[j * 3 + nm] = y * c + z * s;
                        u[j * 3 + i] = z * c - y * s;
                    }
                }
            }
            z = w[k];
            if (l == k)
            {
                if (z < 0.0)
                {
                    w[k] = -z;
                    for (j = 0; j < 3; j++)
                        v[j * 3 + k] = -v[j * 3 + k];
                }
                break;
            }
            if (its == 29)
            {
                qDebug() << "no convergence in 30 svdcmp iterations";
                return;
            }
            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = pythag(f, 1.0f);
            f = ((x-z) * (x + z) + h * ((y / (f + sign(g, f))) - h)) / x;
            c = s = 1.0;
            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = pythag(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (jj = 0; jj < 3; jj++)
                {
                    x = v[jj * 3 + j];
                    z = v[jj * 3 + i];
                    v[jj * 3 + j] = x * c + z * s;
                    v[jj * 3 + i] = z * c - x * s;
                }
                z = pythag(f, h);
                w[j] = z;
                if (z)
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for (jj = 0; jj < 3; jj++)
                {
                    y = u[jj * 3 + j];
                    z = u[jj * 3 + i];
                    u[jj * 3 + j] = y * c + z * s;
                    u[jj * 3 + i] = z * c - y * s;
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }
    }

    // reorder
    int inc = 1;
    float sw;
    float su[3], sv[3];

    do
    {
        inc *= 3;
        inc++;
    } while(inc <= 3);

    do
    {
        inc /= 3;
        for (i = inc; i < 3; i++)
        {
            sw = w[i];
            for (k = 0; k < 3; k++)
                su[k] = u[k * 3 + i];
            for (k = 0; k < 3; k++)
                sv[k] = v[k * 3 + i];
            j = i;
            while (w[j - inc] < sw)
            {
                w[j] = w[j - inc];
                for (k = 0; k < 3; k++)
                    u[k * 3 + j] = u[k * 3 + j - inc];
                for (k = 0; k < 3; k++)
                    v[k * 3 + j] = v[k * 3 + j - inc];
                j -= inc;
                if (j < inc)
                    break;
            }
            w[j] = sw;
            for (k = 0; k < 3; k++)
                u[k * 3 + j] = su[k];
            for (k = 0; k < 3; k++)
                v[k * 3 + j] = sv[k];

        }
    } while(inc > 1);

    for (k = 0; k < 3; k++)
    {
        s = 0;
        for (i = 0; i < 3; i++)
            if (u[i * 3 + k] < 0.0)
                s++;
        for (j = 0; j < 3; j++)
            if (v[j * 3 + k] < 0.0)
                s++;
        if (s > 3)
        {
            for (i = 0; i < 3; i++)
                u[i * 3 + k] = -u[i * 3 + k];
            for (j = 0; j < 3; j++)
                v[j * 3 + k] = -v[j * 3 + k];
        }
    }
}

float BBMatrix::pythag(const float a, const float b)
{
    float absa = fabsf(a), absb = fabsf(b);
    return (absa > absb ? absa * (float)sqrt((double)(1.0 + (absb / absa) * (absb / absa))) :
                          (absb == 0.0 ? 0.0 : absb * (float)sqrt((double)(1.0 + (absa / absb) * (absa / absb)))));
}
