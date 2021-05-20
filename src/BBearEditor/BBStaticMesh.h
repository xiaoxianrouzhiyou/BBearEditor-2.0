#ifndef BBOBJMESH_H
#define BBOBJMESH_H


#include "BBMesh.h"


class BBOBJMesh : public BBMesh
{
public:
    BBOBJMesh();
    BBOBJMesh(float px, float py, float pz,
              float rx, float ry, float rz,
              float sx, float sy, float sz);

private:
    void load(const QString &path, QList<QVector4D> &outPositions) override;
};

#endif // BBOBJMESH_H
