#include "BBSphericalHarmonicLighting.h"
#include "Utils/BBUtils.h"
#include <QImage>
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "3D/BBSkyBox.h"


int BBSphericalHarmonicLighting::m_nDegree = 9;
int BBSphericalHarmonicLighting::m_nWidth = 1024;
int BBSphericalHarmonicLighting::m_nHeight = 1024;


BBSphericalHarmonicLighting::BBSphericalHarmonicLighting()
{

}

void BBSphericalHarmonicLighting::bakeLightingMap()
{
    QImage *pSkyBoxSides = loadSkyBox();
    m_nWidth = pSkyBoxSides[0].width();
    m_nHeight = pSkyBoxSides[0].height();
    // Generally, 9 coefficients are taken
    // A coefficient exists in a channel of a image -> 3 images
    // 6 sides of skybox needs 18 images
    QList<QVector3D> L;
    for (int d = 0; d < m_nDegree; d++)
    {
        L.append(QVector3D());
    }
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
                QVector3D pixelPos = cubeUV2XYZ(k, u, v);
                QColor skyBoxPixelColor = pSkyBoxSides[k].pixelColor(i, j);
                QVector3D color = QVector3D(skyBoxPixelColor.redF(), skyBoxPixelColor.greenF(), skyBoxPixelColor.blueF()) * da;
                QList<float> Y = getBasis(pixelPos);
                for (int d = 0; d < m_nDegree; d++)
                {
                    L[d] += Y[d] * color;
                }
            }
        }
    }

    writeLightingMap(L);
}

void BBSphericalHarmonicLighting::writeLightingMap(const QList<QVector3D> coefficients)
{
    for (int k = 0; k < 6; k++)
    {
        QImage bakedMap(m_nWidth, m_nHeight, QImage::Format_RGB32);
        for (int i = 0; i < m_nWidth; i++)
        {
            for (int j = 0; j < m_nHeight; j++)
            {
                float u = (float)j / (m_nWidth - 1);
                float v = 1.0f - (float)i / (m_nHeight - 1);
                QVector3D pos = cubeUV2XYZ(k, u, v);
                QVector3D color = computeFinalColor(pos, coefficients);
                if (color.x() > 1.0f)
                    color.setX(1.0f);
                if (color.y() > 1.0f)
                    color.setY(1.0f);
                if (color.z() > 1.0f)
                    color.setZ(1.0f);
                if (color.x() < 0.0f)
                    color.setX(0.0f);
                if (color.y() < 0.0f)
                    color.setY(0.0f);
                if (color.z() < 0.0f)
                    color.setZ(0.0f);

                bakedMap.setPixelColor(i, j, QColor(color.x() * 255, color.y() * 255, color.z() * 255));
            }
        }
        bakedMap.save(BBConstant::BB_PATH_PROJECT_USER + "/SHLightMap" + QString::number(k) + ".png");
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

QList<float> BBSphericalHarmonicLighting::getBasis(const QVector3D &pos)
{
    QList<float> Y;
    QVector3D normal = pos.normalized();
    float x = normal.x();
    float y = normal.y();
    float z = normal.z();

    if (m_nDegree >= 0)
    {
        Y.append(1.0f / 2.0f * sqrt(1.0f / PI));
    }
    if (m_nDegree >= 1)
    {
        Y.append(sqrt(3.0f / (4.0f * PI)) * z);
        Y.append(sqrt(3.0f / (4.0f * PI)) * y);
        Y.append(sqrt(3.0f / (4.0f * PI)) * x);
    }
    if (m_nDegree >= 2)
    {
        Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * x * z);
        Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * z * y);
        Y.append(1.0f / 4.0f * sqrt(5.0f / PI) * (-x * x - z * z + 2 * y * y));
        Y.append(1.0f / 2.0f * sqrt(15.0f / PI) * y * x);
        Y.append(1.0f / 4.0f * sqrt(15.0f / PI) * (x * x - z * z));
    }
    if (m_nDegree >= 3)
    {
        Y.append(1.0f / 4.0f * sqrt(35.0f / (2.f * PI)) * (3 * x * x - z * z) * z);
        Y.append(1.0f / 2.0f * sqrt(105.0f / PI) * x * z * y);
        Y.append(1.0f / 4.0f * sqrt(21.0f / (2.f * PI)) * z * (4 * y * y - x * x - z * z));
        Y.append(1.0f / 4.0f * sqrt(7.0f / PI) * y * (2 * y * y - 3 * x * x - 3 * z * z));
        Y.append(1.0f / 4.0f * sqrt(21.0f / (2.f * PI)) * x * (4 * y * y - x * x - z * z));
        Y.append(1.0f / 4.0f * sqrt(105.0f / PI) * (x * x - z * z) * y);
        Y.append(1.0f / 4.0f * sqrt(35.0f / (2 * PI)) * (x * x - 3 * z * z) * x);
    }
    return Y;
}

QVector3D BBSphericalHarmonicLighting::computeFinalColor(const QVector3D &pos, const QList<QVector3D> coefficients)
{
    QList<float> Y = getRenderBasis(pos);
    QVector3D color;
    for (int i = 0; i < m_nDegree; i++)
    {
        color += Y[i] * coefficients[i];
    }
    return color;
}

QList<float> BBSphericalHarmonicLighting::getRenderBasis(const QVector3D &pos)
{
    QList<float> Y;
    QVector3D normal = pos.normalized();
    float x = normal.x();
    float y = normal.y();
    float z = normal.z();

    if (m_nDegree >= 0)
    {
        Y.append(1.0f / 2.0f * sqrt(1.0f / PI));
    }
    if (m_nDegree >= 1)
    {
        Y.append(2.0f / 3.0f * sqrt(3.0f / (4.0f * PI)) * z);
        Y.append(2.0f / 3.0f * sqrt(3.0f / (4.0f * PI)) * y);
        Y.append(2.0f / 3.0f * sqrt(3.0f / (4.0f * PI)) * x);
    }
    if (m_nDegree >= 2)
    {
        Y.append(1.0f / 4.0f * 1.0f / 2.0f * sqrt(15.0f / PI) * x * z);
        Y.append(1.0f / 4.0f * 1.0f / 2.0f * sqrt(15.0f / PI) * z * y);
        Y.append(1.0f / 4.0f * 1.0f / 4.0f * sqrt(5.0f / PI) * (-x * x - z * z + 2 * y * y));
        Y.append(1.0f / 4.0f * 1.0f / 2.0f * sqrt(15.0f / PI) * y * x);
        Y.append(1.0f / 4.0f * 1.0f / 4.0f * sqrt(15.0f / PI) * (x * x - z * z));
    }
    return Y;
}

