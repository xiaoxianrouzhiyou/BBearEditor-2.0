#ifndef BBOBJMESH_H
#define BBOBJMESH_H


#include "BBMesh.h"


class BBOBJMesh : public BBMesh
{
public:
    BBOBJMesh();
    BBOBJMesh(const float px, const float py, const float pz,
              const float rx, const float ry, const float rz,
              const float sx, const float sy, const float sz);

private:
    void load(const QString path, QList<QVector4D> &outPositions) override;
};

#endif // BBOBJMESH_H
