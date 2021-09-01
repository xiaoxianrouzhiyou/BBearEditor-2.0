#ifndef BBRENDERABLEOBJECT_H
#define BBRENDERABLEOBJECT_H

#include "BBGameObject.h"
#include <QtOpenGL>


class BBDrawCall;
class BBMaterial;
class BBCamera;
class BBVertexBufferObject;
class BBShaderStorageBufferObject;
class BBElementBufferObject;

class BBRenderableObject : public BBGameObject
{
public:
    BBRenderableObject();
    BBRenderableObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    BBRenderableObject(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);
    BBRenderableObject(int x, int y, int nWidth, int nHeight);
    virtual ~BBRenderableObject();

    void init() override;
    void render(BBCamera *pCamera) override;
    void render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera) override;
    void insertInRenderQueue(BBRenderQueue *pQueue) override;
    void removeFromRenderQueue(BBRenderQueue *pQueue) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setModelMatrix(float px, float py, float pz,
                        const QQuaternion &r,
                        float sx, float sy, float sz) override;
    void setVisibility(bool bVisible) override;
    void setCurrentMaterial(BBMaterial *pMaterial) override;
    void restoreMaterial() override;

    void setMatrix4(const std::string &uniformName, const float *pMatrix4) override;
    void setVector4(const std::string &uniformName, float x, float y, float z, float w) override;
    void setArrayVector4(const std::string &uniformName, const float *pArrayVector4, int nArrayCount);
    void setTexture(const std::string &uniformName, GLuint textureName) override;

    void openLight() override;
    void closeLight() override;

    inline BBMaterial* getMaterial() { return m_pCurrentMaterial; }
    inline BBVertexBufferObject* getVBO() { return m_pVBO; }
    inline BBElementBufferObject* getEBO() { return m_pEBO; }

protected:
    void appendDrawCall(BBDrawCall *pDrawCall);

    bool m_bInRenderQueue;
    BBDrawCall *m_pDrawCalls;
    BBMaterial *m_pCurrentMaterial;
    BBMaterial *m_pDefaultMaterial;
    BBVertexBufferObject *m_pVBO;
    BBShaderStorageBufferObject *m_pSSBO;
    BBElementBufferObject *m_pEBO;
    unsigned short *m_pIndexes;
    int m_nIndexCount;
    int m_nVertexCount;
    QVector3D m_DefaultColor;

private:
    void sharedInit();
};

#endif // BBRENDERABLEOBJECT_H
