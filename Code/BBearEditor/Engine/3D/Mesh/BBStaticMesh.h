#ifndef BBSTATICMESH_H
#define BBSTATICMESH_H


#include "BBMesh.h"


class BBStaticMesh : public BBMesh
{
public:
    BBStaticMesh();
    BBStaticMesh(float px, float py, float pz,
                 float rx, float ry, float rz,
                 float sx, float sy, float sz);

    void init(const QString &path, BBBoundingBox3D *&pOutBoundingBox) override;

private:
    void load(const QString &path, QList<QVector4D> &outPositions) override;
};

#endif // BBSTATICMESH_H
