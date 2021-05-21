#ifndef BBMODEL_H
#define BBMODEL_H


#include "BBGameObject.h"
#include "BBMesh.h"


class BBMesh;
class BBOBJMesh;
class BBBoundingBox3D;
class BBModel : public BBGameObject
{
public:
    BBModel(BBMeshType eType);
    BBModel(float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz, const BBMeshType &eType);
    virtual ~BBModel();

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, const bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                 const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                 const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                 const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3) override;

private:
    BBMesh *m_pMesh;
    BBBoundingBox3D *m_pBoundingBox;
};

#endif // BBMODEL_H




//    void renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos);
//    void setDiffuseMaterial(float r, float g, float b, float a);
//    void lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor = 2.4) override;
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
//    QList<ScriptManager*> mScriptManagers;
