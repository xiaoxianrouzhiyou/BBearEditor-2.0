#ifndef BBSPHERICALHARMONICLIGHTING_H
#define BBSPHERICALHARMONICLIGHTING_H


#include <QVector3D>
#include <QList>

class BBSphericalHarmonicLighting
{
public:
    static void computeLightingData(int nAlgorithmIndex);

    static const float* getCoefficientL();
    static int getCoefficientLCount();

    static void bakeLightingMap();
    static void computeCoefficientL();

private:
    static QImage* loadSkyBox();
    static float computeSphereSurfaceArea(double x, double y);
    static QVector3D cubeUV2XYZ(int nSkyBoxSideIndex, double u, double v);
    static QList<float> getYBasis(const QVector3D &normal);

private:
    static int m_nDegree;
    static int m_nWidth;
    static int m_nHeight;

    static QList<QVector3D> m_CoefficientL;
};

#endif // BBSPHERICALHARMONICLIGHTING_H
