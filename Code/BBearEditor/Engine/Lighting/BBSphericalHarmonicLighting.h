#ifndef BBSPHERICALHARMONICLIGHTING_H
#define BBSPHERICALHARMONICLIGHTING_H


#include <QVector3D>

class BBSphericalHarmonicLighting
{
public:
    BBSphericalHarmonicLighting();

public:
    static void bakeLightingMap();

private:
    static void writeLightingMap(const QList<QVector3D> coefficients);
    static QImage* loadSkyBox();
    static float computeSphereSurfaceArea(double x, double y);
    static QVector3D cubeUV2XYZ(int nSkyBoxSideIndex, double u, double v);
    static QList<float> getBasis(const QVector3D &pos);
    static QVector3D computeFinalColor(const QVector3D &pos, const QList<QVector3D> coefficients);
    static QList<float> getRenderBasis(const QVector3D &pos);

private:
    static int m_nDegree;
    static int m_nWidth;
    static int m_nHeight;
};

#endif // BBSPHERICALHARMONICLIGHTING_H
