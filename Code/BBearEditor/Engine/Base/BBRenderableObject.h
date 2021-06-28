#ifndef BBRENDERABLEOBJECT_H
#define BBRENDERABLEOBJECT_H

#include "BBGameObject.h"
#include <QtOpenGL>


class BBDrawCall;
class BBMaterial;
class BBCamera;
class BBVertexBufferObject;
class BBElementBufferObject;
class BBRenderableObject : public BBGameObject
{
public:
    BBRenderableObject();
    BBRenderableObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    BBRenderableObject(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);
    virtual ~BBRenderableObject();

    void init() override;
    void render(BBCamera *pCamera) override;
    void render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera) override;
    void setCurrentMaterial(int nIndex) override;
    void setCurrentMaterial(BBMaterial *pMaterial) override;

    inline BBMaterial* getMaterial() { return m_pCurrentMaterial; }
    inline BBVertexBufferObject* getVBO() { return m_pVBO; }
    inline BBElementBufferObject* getEBO() { return m_pEBO; }

protected:
    void appendDrawCall(BBDrawCall *pDrawCall);

    BBDrawCall *m_pDrawCalls;
    QList<BBMaterial*> m_Materials;
    BBMaterial *m_pCurrentMaterial;
    BBVertexBufferObject *m_pVBO;
    BBElementBufferObject *m_pEBO;
    unsigned short *m_pIndexes;
    int m_nIndexCount;
    int m_nVertexCount;
    QVector3D m_DefaultColor;
};

#endif // BBRENDERABLEOBJECT_H
