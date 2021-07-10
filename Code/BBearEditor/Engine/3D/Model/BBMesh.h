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


//class Mesh : public RenderableObject
//{
//public:
//    void setColor(QVector3D color);
//    void setIsDepthTest(bool isDepthTest);
//    void setMaterial(Material *material);
//    void setDefaultMaterial();
//    void updateDirectionLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updateDirectionLightColor(QVector4D *lightColor, int count);
//    void updatePointLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updatePointLightColor(QVector4D *lightColor, int count);
//    void updatePointLightOption(QVector4D *option, int count);
//    void updateSpotLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updateSpotLightDirection(QVector4D *lightDirection, int count);
//    void updateSpotLightColor(QVector4D *lightColor, int count);
//    void updateSpotLightOption(QVector4D *option, int count);
//    void updateSpotLightOption2(QVector4D *option, int count);
//    void switchFog(bool b);
//    void setFogColor(float r, float g, float b);
//    void setFogOption(float start, float end, int density, float power);
//    void setFogMode(int mode);
//    void renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos);
//    void setShadowMap(GLuint map);
//    void changeTerrainHeightMap(QString path);
//    QList<QVector4D> mVertexPositions;

//private:
//    bool mIsDepthTest;
//};
