#ifndef BBMODEL_H
#define BBMODEL_H


#include "Base/BBGameObject.h"
#include "Mesh/BBMesh.h"
#include "Geometry/BBTracer.h"


class BBMesh;
class BBOBJMesh;
class BBBoundingBox3D;
class BBModel : public BBGameObject
{
public:
    BBModel(const BBMeshType &eType);
    BBModel(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale, const BBMeshType &eType);
    BBModel(float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz, const BBMeshType &eType);
    ~BBModel();

    inline BBMesh* getMesh() { return m_pMesh; }

    void init(const QString &path) override;
    void init(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount);
    void render(BBCamera *pCamera) override;
    void renderToShadowMap(BBCamera *pCamera);
    void insertInRenderQueue(BBRenderQueue *pQueue) override;
    void removeFromRenderQueue(BBRenderQueue *pQueue) override;

    void setCurrentMaterial(BBMaterial *pMaterial) override;
    void setCurrentMaterial(int nExtraMaterialIndex) override;
    void setExtraMaterial(int nMaterialIndex, BBMaterial *pMaterial) override;
    void rollbackMaterial() override;
    void restoreMaterial() override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;
    void setBoundingBoxVisibility(bool bVisible);

    bool hit(const BBRay &ray, float &fDistance) override;
    bool hit(const BBRay &ray, float fMinDistance, float fMaxDistance, BBHitInfo &hitInfo);
    bool belongToSelectionRegion(const BBFrustum &frustum) override;
    void showCloseUp(QVector3D &outPosition, QVector3D &outViewCenter, float fDistFactor = 2.4) override;

private:
    BBMesh *m_pMesh;
    BBBoundingBox3D *m_pBoundingBox;
};

#endif // BBMODEL_H
