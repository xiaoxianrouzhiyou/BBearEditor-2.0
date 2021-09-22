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
    void init(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount, BBBoundingBox3D *&pOutBoundingBox);

    void setExtraMaterial(int nMaterialIndex, BBMaterial *pMaterial) override;

    bool hit(const BBRay &ray, float &fDistance) override;
    bool hit(const BBRay &ray, float fMinDistance, float fMaxDistance, BBHitInfo &hitInfo);

protected:
    virtual void load(const QString &path, QList<QVector4D> &outPositions) = 0;

    GLenum m_eDrawPrimitiveType;

    // Some meshes are composed of multiple primitivetypes and require multiple sets of data representation
    BBElementBufferObject *m_pEBO2;
    unsigned short *m_pIndexes2;
    int m_nIndexCount2;
};

#endif // BBMESH_H
