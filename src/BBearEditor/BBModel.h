#ifndef BBMODEL_H
#define BBMODEL_H


#include "BBGameObject.h"
#include "BBMesh.h"


class BBMesh;
class BBOBJMesh;
class BBModel : public BBGameObject
{
public:
    BBModel(BBMeshType eType);
    BBModel(const float px, const float py, const float pz,
            const float rx, const float ry, const float rz,
            const float sx, const float sy, const float sz, BBMeshType eType);

    void init(const QString path) override;

private:
    BBMesh *m_pMesh;
};

#endif // BBMODEL_H


//    Model();
//    Model(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    void init(QString path);
//    void render(Camera camera) override;
//    void renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos);
//    void resize(float width, float height) override;
//    void setDiffuseMaterial(float r, float g, float b, float a);
//    void setPosition(QVector3D position, bool isUpdateLocalTransform = true) override;
//    void setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform = true) override;
//    void setRotation(QVector3D rotation, bool isUpdateLocalTransform = true) override;
//    void setScale(QVector3D scale, bool isUpdateLocalTransform = true) override;
//    bool hit(Ray ray, float &distance) override;
//    void setActive(bool isActive) override;
//    void setVisible(bool isVisible) override;
//    void lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor = 2.4) override;
//    bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                 QVector3D top1, QVector3D top2, QVector3D top3,
//                                 QVector3D right1, QVector3D right2, QVector3D right3,
//                                 QVector3D bottom1, QVector3D bottom2, QVector3D bottom3) override;
//    void setMaterial(Material *material);
//    Material *getMaterial();
//    void previewMaterial(Material *material);
//    void materialRemoveUser();
//    void removeMaterial();
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
//    MeshType getMeshType();
//    FBX *getFbxMesh();
//    void play();
//    void stop();
//    void setFBO(GLuint texture);
//    void setShadowMap(GLuint map);
//    void changeTerrainHeightMap(QString path);
//    Terrain *getTerrainMesh();

//    void onLua();
//    void offLua();
//    void onUpdateLua();
//    void onKeyDownLua(QKeyEvent *e);

//    Mesh *mMesh;
//    AABBBoundingBox3D *boundingBox;
//    QList<ScriptManager*> mScriptManagers;
