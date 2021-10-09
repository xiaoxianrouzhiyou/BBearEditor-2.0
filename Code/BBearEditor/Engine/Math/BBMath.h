#pragma once

#ifndef BBMATH_H
#define BBMATH_H


#include <QVector2D>
#include <QVector3D>

#define PI 3.14159265359

float lerp(float a, float b, float f);
QVector2D lerp(const QVector2D &a, const QVector2D &b, float f);
QVector2D lerp(const QVector2D &a, const QVector2D &b, const QVector2D &c, float u, float v);

QVector3D reflect(const QVector3D &L, const QVector3D &N);
bool refract(const QVector3D &L, const QVector3D &N, float fRefractivity, QVector3D &refracted);

template<class T>
T clamp(T x, T min, T max)
{
    if (x > max)
        return max;
    if (x < min)
        return min;
    return x;
}

float frandom();
float sfrandom();
QVector3D hemisphericalRandom(const QVector3D &normal);
QVector3D sphericalRandom();

// KD Tree
int getMedian(int start, int end);

float schlick(float cos, float fRefractivity);

template<class T>
static inline T max(const T &a, const T &b)
{
    return ((a > b) ? a : b);
}

#endif // BBMATH_H
