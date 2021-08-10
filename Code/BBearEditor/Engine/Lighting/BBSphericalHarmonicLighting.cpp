#include "BBSphericalHarmonicLighting.h"
#include "Utils/BBUtils.h"
#include <QImage>
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "3D/BBSkyBox.h"


int BBSphericalHarmonicLighting::m_nDegree = 9;
int BBSphericalHarmonicLighting::m_nWidth = 1024;
int BBSphericalHarmonicLighting::m_nHeight = 1024;
QList<QVector3D> BBSphericalHarmonicLighting::m_CoefficientL;


void BBSphericalHarmonicLighting::computeLightingData(int nAlgorithmIndex)
{
    if (nAlgorithmIndex == 0)
    {
        bakeLightingMap();
    }
    else
    {
        computeCoefficientL();
    }
}

const float* BBSphericalHarmonicLighting::getCoefficientL()
{
    float *L = new float[m_nDegree * 4];
    for (int i = 0; i < m_nDegree; i++)
    {
        L[i * 4] = m_CoefficientL[i].x();
        L[i * 4 + 1] = m_CoefficientL[i].y();
        L[i * 4 + 2] = m_CoefficientL[i].z();
        L[i * 4 + 3] = 0.0f;
    }
    return L;
}

int BBSphericalHarmonicLighting::getCoefficientLCount()
{
    return m_CoefficientL.count();
}

void BBSphericalHarmonicLighting::bakeLightingMap()
{
    QImage *pSkyBoxSides = loadSkyBox();
    m_nWidth = pSkyBoxSides[0].width();
    m_nHeight = pSkyBoxSides[0].height();
    // test
    m_nWidth = 20;
    m_nHeight = 20;

    // Generally, 9 coefficients are taken
    // A coefficient exists in a channel of a image -> 3 images
    // 6 sides of skybox needs 18 images
    m_CoefficientL.clear();
    for (int d = 0; d < m_nDegree; d++)
    {
        m_CoefficientL.append(QVector3D());
    }
    // RGB 3 channels, save 1 coefficient respectively
    // compute 1 x m_nDegree coefficient corresponding to 1 normal
    // 1 coefficient is 1 channel of 1 pixel in 1 image
    // m_nDegree channels

    for (int k = 0; k < 6; k++)
    {
        int nMapCount = ceil((float)m_nDegree / 3);
        QImage bakedMaps[nMapCount];
        for (int nMapIndex = 0; nMapIndex < nMapCount; nMapIndex++)
        {
            bakedMaps[nMapIndex] = QImage(m_nWidth, m_nHeight, QImage::Format_RGB32);
        }

        for (int j = 0; j < m_nWidth; j++)
        {
            for (int i = 0; i < m_nHeight; i++)
            {
                float px = (float)i + 0.5f;
                float py = (float)j + 0.5f;
                float u = 2.0f * (px / (float)m_nWidth) - 1.0f;
                float v = 2.0f * (py / (float)m_nHeight) - 1.0f;
                float dx = 1.0f / (float)m_nWidth;
                float x0 = u - dx;
                float y0 = v - dx;
                float x1 = u + dx;
                float y1 = v + dx;
                float da = computeSphereSurfaceArea(x0, y0) - computeSphereSurfaceArea(x0, y1) - computeSphereSurfaceArea(x1, y0) + computeSphereSurfaceArea(x1, y1);
                u = (float)j / (m_nWidth - 1);
                v = 1.0f - (float)i / (m_nHeight - 1);
                QColor skyBoxPixelColor = pSkyBoxSides[k].pixelColor(i, j);
                QVector3D skyBoxColor = QVector3D(skyBoxPixelColor.redF(), skyBoxPixelColor.greenF(), skyBoxPixelColor.blueF());
                // The pixel position of the sky box is normalized and projected onto the sphere
                QVector3D normalizedSkyBoxPixel = cubeUV2XYZ(k, u, v).normalized();
                QList<float> Y = getYBasis(normalizedSkyBoxPixel);
                for (int d = 0; d < m_nDegree; d++)
                {
                    m_CoefficientL[d] += skyBoxColor * Y[d] * da;
                }
                // normals of sphere
                for (int nk = 0; nk < 6; nk++)
                {
                    for (int nj = 0; nj < m_nWidth; nj++)
                    {
                        for (int ni = 0; ni < m_nHeight; ni++)
                        {
                            float nu = (float)nj / (m_nWidth - 1);
                            float nv = 1.0f - (float)ni / (m_nHeight - 1);
                            QVector3D sphereNormal = cubeUV2XYZ(nk, nu, nv).normalized();

                            for (int nMapIndex = 0, d = 0; nMapIndex < nMapCount; nMapIndex++, d += 3)
                            {
                                QColor color = bakedMaps[nMapIndex].pixelColor(ni, nj);
                                float r = color.redF();
                                float g = color.greenF();
                                float b = color.blueF();

                                float tmp = QVector3D::dotProduct(sphereNormal, normalizedSkyBoxPixel) * da;

                                // Channel exceeding m_nDegree is 0
                                if (d < m_nDegree)
                                    r += Y[d] * tmp;
                                if (d + 1 < m_nDegree)
                                    g += Y[d + 1] * tmp;
                                if (d + 2 < m_nDegree)
                                    b += Y[d + 2] * tmp;

                                if (r > 1.0f)
                                    r = 1.0f;
                                if (g > 1.0f)
                                    g = 1.0f;
                                if (b > 1.0f)
                                    b = 1.0f;
                                if (r < 0.0f)
                                    r = 0.0f;
                                if (g < 0.0f)
                                    g = 0.0f;
                                if (b < 0.0f)
                                    b = 0.0f;

                                color.setRgbF(r, g, b);
                                bakedMaps[nMapIndex].setPixelColor(ni, nj, color);
                            }
                        }
                    }
                }
            }
        }
        // save map
        for (int nMapIndex = 0; nMapIndex < nMapCount; nMapIndex++)
        {
            bakedMaps[nMapIndex].save(BBConstant::BB_PATH_PROJECT_ENGINE + "SH_LightMap_t" + QString::number(k) + "_" + QString::number(nMapIndex) + ".png");
        }
    }
}

void BBSphericalHarmonicLighting::computeCoefficientL()
{
    QImage *pSkyBoxSides = loadSkyBox();
    m_nWidth = pSkyBoxSides[0].width();
    m_nHeight = pSkyBoxSides[0].height();

    // Generally, 9 coefficients are taken
    // A coefficient exists in a channel of a image -> 3 images
    // 6 sides of skybox needs 18 images
    m_CoefficientL.clear();
    for (int d = 0; d < m_nDegree; d++)
    {
        m_CoefficientL.append(QVector3D());
    }
    // RGB 3 channels, save 1 coefficient respectively
    // compute 1 x m_nDegree coefficient corresponding to 1 normal
    // 1 coefficient is 1 channel of 1 pixel in 1 image
    // m_nDegree channels

    for (int k = 0; k < 6; k++)
    {
        for (int j = 0; j < m_nWidth; j++)
        {
            for (int i = 0; i < m_nHeight; i++)
            {
                float px = (float)i + 0.5f;
                float py = (float)j + 0.5f;
                float u = 2.0f * (px / (float)m_nWidth) - 1.0f;
                float v = 2.0f * (py / (float)m_nHeight) - 1.0f;
                float dx = 1.0f / (float)m_nWidth;
                float x0 = u - dx;
                float y0 = v - dx;
                float x1 = u + dx;
                float y1 = v + dx;
                float da = computeSphereSurfaceArea(x0, y0) - computeSphereSurfaceArea(x0, y1) - computeSphereSurfaceArea(x1, y0) + computeSphereSurfaceArea(x1, y1);
                u = (float)j / (m_nWidth - 1);
                v = 1.0f - (float)i / (m_nHeight - 1);
                QColor skyBoxPixelColor = pSkyBoxSides[k].pixelColor(i, j);
                QVector3D skyBoxColor = QVector3D(skyBoxPixelColor.redF(), skyBoxPixelColor.greenF(), skyBoxPixelColor.blueF());
                // The pixel position of the sky box is normalized and projected onto the sphere
                QVector3D normalizedSkyBoxPixel = cubeUV2XYZ(k, u, v).normalized();
                QList<float> Y = getYBasis(normalizedSkyBoxPixel);
                for (int d = 0; d < m_nDegree; d++)
                {
                    m_CoefficientL[d] += skyBoxColor * Y[d] * da;
                }
            }
        }
    }
}

QImage* BBSphericalHarmonicLighting::loadSkyBox()
{
    QImage *pSkyBoxSides = new QImage[6];
    QString filePath = BBSceneManager::getScene()->getSkyBox()->getSkyBoxFilePath();
    pSkyBoxSides[0] = QImage(filePath + "right");
    pSkyBoxSides[1] = QImage(filePath + "left");
    pSkyBoxSides[2] = QImage(filePath + "bottom");
    pSkyBoxSides[3] = QImage(filePath + "top");
    pSkyBoxSides[4] = QImage(filePath + "back");
    pSkyBoxSides[5] = QImage(filePath + "front");
    return pSkyBoxSides;
}

float BBSphericalHarmonicLighting::computeSphereSurfaceArea(double x, double y)
{
    return atan2(x * y, sqrt(x * x + y * y + 1.0));
}

QVector3D BBSphericalHarmonicLighting::cubeUV2XYZ(int nSkyBoxSideIndex, double u, double v)
{
    u = u * 2.0f - 1.0f;
    v = v * 2.0f - 1.0f;
    switch (nSkyBoxSideIndex)
    {
    case 0:
        // +x
        return QVector3D(1, v, -u);
    case 1:
        // -x
        return QVector3D(-1, v, u);
    case 2:
        // +y
        return QVector3D(u, 1, -v);
    case 3:
        // -y
        return QVector3D(u, -1, v);
    case 4:
        // +z
        return QVector3D(u, v, 1);
    case 5:
        // -z
        return QVector3D(-u, v, -1);
    }
    return QVector3D();
}

QList<float> BBSphericalHarmonicLighting::getYBasis(const QVector3D &normal)
{
    QList<float> Y;
    float x = normal.x();
    float y = normal.y();
    float z = normal.z();

    Y.append(1.0f / 2.0f * sqrt(1.0f / PI));
    Y.append(sqrt(3.0f / (4.0f * PI)) * z);
    Y.append(sqrt(3.0f / (4.0f * PI)) * y);
    Y.append(sqrt(3.0f / (4.0f * PI)) * x);
    Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * x * z);
    Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * z * y);
    Y.append(1.0f / 4.0f * sqrt(5.0f / PI) * (-x * x - z * z + 2 * y * y));
    Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * y * x);
    Y.append(1.0f / 4.0f * sqrt(15.0f / PI) * (x * x - z * z));
    Y.append(1.0f / 4.0f * sqrt(35.0f / (2.f * PI)) * (3 * x * x - z * z) * z);
    Y.append(1.0f / 2.0f * sqrt(105.0f / PI) * x * z * y);
    Y.append(1.0f / 4.0f * sqrt(21.0f / (2.f * PI)) * z * (4 * y * y - x * x - z * z));
    Y.append(1.0f / 4.0f * sqrt(7.0f / PI) * y * (2 * y * y - 3 * x * x - 3 * z * z));
    Y.append(1.0f / 4.0f * sqrt(21.0f / (2.f * PI)) * x * (4 * y * y - x * x - z * z));
    Y.append(1.0f / 4.0f * sqrt(105.0f / PI) * (x * x - z * z) * y);
    Y.append(1.0f / 4.0f * sqrt(35.0f / (2 * PI)) * (x * x - 3 * z * z) * x);

    return Y;
}
