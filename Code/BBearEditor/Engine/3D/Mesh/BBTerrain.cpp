#include "BBTerrain.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"


const int BBTerrain::m_nUnitCount = 200;
const float BBTerrain::m_fStep = 1.0f;

BBTerrain::BBTerrain()
    : BBTerrain(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBTerrain::BBTerrain(float px, float py, float pz,
                     float rx, float ry, float rz,
                     float sx, float sy, float sz)
    : BBMesh(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_fHeight = 30;
    m_eDrawPrimitiveType = GL_TRIANGLES;
}

void BBTerrain::init(const QString &path, BBBoundingBox3D *&pOutBoundingBox)
{
    BBMesh::init(path, pOutBoundingBox);
    m_pCurrentMaterial->getBaseRenderPass()->setPolygonMode(GL_FRONT, GL_LINE);
}

void BBTerrain::load(const QString &path, QList<QVector4D> &outPositions)
{
    QImage image(path);
    m_pVBO = new BBVertexBufferObject(m_nUnitCount * m_nUnitCount);
    float fRadius = (m_nUnitCount - 1) * m_fStep / 2;
    for (int z = 0; z < m_nUnitCount; z++)
    {
        for (int x = 0; x < m_nUnitCount; x++)
        {
            float fPosX = x * m_fStep;
            float fPosZ = z * m_fStep;
            int nIndex = z * m_nUnitCount + x;
            QVector4D position(fPosX - fRadius, getHeight(image, fPosX, fPosZ), fPosZ - fRadius, 1.0f);
            outPositions.append(position);
            m_pVBO->setPosition(nIndex, position);
            m_pVBO->setTexcoord(nIndex, fPosX / 128, fPosZ / 128);
            m_pVBO->setNormal(nIndex, getNormal(image, fPosX, fPosZ));
        }
    }

    m_nIndexCount = (m_nUnitCount - 1) * (m_nUnitCount - 1) * 6;
    m_pIndexes = new unsigned short[m_nIndexCount];
    int nIndex = 0;
    for (int z = 0; z < m_nUnitCount - 1; z++)
    {
        for (int x = 0; x < m_nUnitCount - 1; x++)
        {
            int nOffset = x + z * m_nUnitCount;

            m_pIndexes[nIndex * 6] = nOffset;
            m_pIndexes[nIndex * 6 + 1] = nOffset + 1;
            m_pIndexes[nIndex * 6 + 2] = nOffset + m_nUnitCount;

            m_pIndexes[nIndex * 6 + 3] = nOffset + 1;
            m_pIndexes[nIndex * 6 + 4] = nOffset + m_nUnitCount;
            m_pIndexes[nIndex * 6 + 5] = nOffset + m_nUnitCount + 1;

            nIndex++;
        }
    }
}

float BBTerrain::getHeight(const QImage &image, int x, int z)
{
    if (x < 0 || x > image.width() || z < 0 || z > image.height())
    {
        return 0;
    }
    else
    {
        return (image.pixelColor(x, z).greenF() - 0.5f) * m_fHeight;
    }
}

QVector3D BBTerrain::getNormal(const QImage &image, int x, int z)
{
    float fHeightL = getHeight(image, x - m_fStep, z);
    float fHeightR = getHeight(image, x + m_fStep, z);
    float fHeightU = getHeight(image, x, z + m_fStep);
    float fHeightD = getHeight(image, x, z - m_fStep);

    return QVector3D(fHeightL - fHeightR, 2.0f, fHeightD - fHeightU).normalized();
}





//void Terrain::changeHeightMap(QString mapPath)
//{
//    //清理
//    closeLoad();
//    //生成网格
//    mHeightMapPath = mapPath;
//    QImage image(mapPath);
//    float radius = (mUnitCount - 1) * mStep / 2;
//    for (int z = 0; z < mUnitCount; z++)
//    {
//        for (int x = 0; x < mUnitCount; x++)
//        {
//            float xPos = x * mStep;
//            float zPos = z * mStep;
//            mVertexPositions.append(QVector4D(xPos - radius, getHeightByMap(image, xPos, zPos), zPos - radius, 1.0f));
//            mVertexUVs.append(QVector2D(xPos / 128, zPos / 128));
//            mVertexNormals.append(getNormalByMap(image, xPos, zPos));
//        }
//    }
//}
