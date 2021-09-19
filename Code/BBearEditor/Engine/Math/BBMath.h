#pragma once

#ifndef BBMATH_H
#define BBMATH_H


#include <QVector2D>
#include <QVector3D>

#define PI 3.14159265359

float lerp(float a, float b, float f);
QVector2D lerp(QVector2D a, QVector2D b, float f);
QVector2D lerp(QVector2D a, QVector2D b, QVector2D c, float u, float v);
float frandom();
float sfrandom();
QVector3D hemisphericalRandom(QVector3D normal);

// KD Tree
int getMedian(int start, int end);


#endif // BBMATH_H
