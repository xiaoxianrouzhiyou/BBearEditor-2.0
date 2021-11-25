#ifndef BBPROCEDUREMESH_H
#define BBPROCEDUREMESH_H


#include "BBMesh.h"

class BBProcedureMesh : public BBMesh
{
public:
    BBProcedureMesh();
    BBProcedureMesh(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

    void init(const QString &userData, BBBoundingBox3D *&pOutBoundingBox) override;

private:
    void load(const QString &userData, QList<QVector4D> &outPositions) override;

    void init0();
};

#endif // BBPROCEDUREMESH_H
