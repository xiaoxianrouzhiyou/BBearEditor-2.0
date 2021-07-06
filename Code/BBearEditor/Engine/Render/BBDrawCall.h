#ifndef BBDRAWCALL_H
#define BBDRAWCALL_H


#include "BBMaterial.h"
#include "BBLinkedList.h"

class BBCamera;
class BBVertexBufferObject;
class BBElementBufferObject;
class BBGameObject;
class BBDrawCall;
class BBRenderableObject;
class BBRenderQueue;

typedef void (BBDrawCall::*BBDrawFunc)(BBCamera *pCamera);

class BBDrawCall : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBDrawCall();

    void setMaterial(BBMaterial *pMaterial);
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType = GL_TRIANGLES,
                int nDrawStartIndex = 0, int nDrawCount = 3);
    void setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);

    void updateOrderInRenderQueue(const QVector3D &renderableObjectPosition);
    float getDistanceToCamera(BBCamera *pCamera);

    void draw(BBCamera *pCamera);

public:
    static void switchRenderingSettings(int nIndex);
    void onePassRendering(BBCamera *pCamera);
    void onePassRendering(BBCamera *pCamera, QList<BBGameObject*> lights);
    void forwardRendering(BBCamera *pCamera);
    void deferredRendering(BBCamera *pCamera);

private:
    QList<BBGameObject*> collectLights();

    static BBDrawFunc m_DrawFunc;
    BBMaterial *m_pMaterial;

    GLenum m_eDrawPrimitiveType;
    int m_nDrawStartIndex;

    BBVertexBufferObject *m_pVBO;
    int m_nDrawCount;

    BBElementBufferObject *m_pEBO;
    int m_nIndexCount;
};


class BBRenderQueue
{
public:
    BBRenderQueue(BBCamera *pCamera);
    ~BBRenderQueue();

    inline BBDrawCall* getOpaqueDrawCall() { return m_pOpaqueDrawCall; }
    inline BBDrawCall* getTransparentDrawCall() { return m_pTransparentDrawCall; }

    void appendOpaqueDrawCall(BBDrawCall *pDC);
    void appendTransparentDrawCall(BBDrawCall *pDC);
    void appendUIDrawCall(BBDrawCall *pDC);

    void removeOpaqueDrawCall(BBDrawCall *pDC);
    void removeTransparentDrawCall(BBDrawCall *pDC);
    void removeUIDrawCall(BBDrawCall *pDC);

    void render();
    void renderOpaque();
    void renderTransparent();
    void renderUI();

    void updateOrder(BBDrawCall *pHead, BBDrawCall *pNode);

private:
    void appendAscendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode);

    BBCamera *m_pCamera;
    BBDrawCall *m_pOpaqueDrawCall;
    BBDrawCall *m_pTransparentDrawCall;
    BBDrawCall *m_pUIDrawCall;
};

#endif // BBDRAWCALL_H
