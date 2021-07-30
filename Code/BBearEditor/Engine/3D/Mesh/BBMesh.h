#ifndef BBMESH_H
#define BBMESH_H


#include "Base/BBRenderableObject.h"


enum BBMeshType
{
    OBJ = 0x01,
    FBX = 0x02,
    TERRAIN = 0x04
};


class BBBoundingBox3D;
class BBMesh : public BBRenderableObject
{
public:
    BBMesh();
    BBMesh(float px, float py, float pz,
           float rx, float ry, float rz,
           float sx, float sy, float sz);

    virtual void init(const QString &path, BBBoundingBox3D *&pOutBoundingBox);

    bool hit(const BBRay &ray, float &fDistance) override;

protected:
    virtual void load(const QString &path, QList<QVector4D> &outPositions) = 0;
};

#endif // BBMESH_H
