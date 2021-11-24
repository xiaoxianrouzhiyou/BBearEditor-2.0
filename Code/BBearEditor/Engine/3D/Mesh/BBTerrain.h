#ifndef BBTERRAIN_H
#define BBTERRAIN_H


#include "BBMesh.h"

class BBTerrain : public BBMesh
{
public:
    BBTerrain();
    BBTerrain(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

    void init(const QString &path, BBBoundingBox3D *&pOutBoundingBox) override;

private:
    void load(const QString &path, QList<QVector4D> &outPositions) override;

    float getHeight(const QImage &image, int x, int z);
    QVector3D getNormal(const QImage &image, int x, int z);

    float m_fHeight;
    const static int m_nUnitCount;
    const static float m_fStep;
};

#endif // BBTERRAIN_H
