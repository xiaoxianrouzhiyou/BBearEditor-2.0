#ifndef BBDRAWCALL_H
#define BBDRAWCALL_H


#include "BBMaterial.h"
#include "BBLinkedList.h"

class BBCamera;
class BBCanvas;
class BBVertexBufferObject;
class BBElementBufferObject;
class BBGameObject;
class BBDrawCall;
class BBRenderableObject;
class BBRenderQueue;

typedef void (BBDrawCall::*BBDrawFunc)(BBCamera *pCamera);
typedef void (BBDrawCall::*BBUpdateOrderInRenderQueueFunc)();

class BBDrawCall : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBDrawCall();

    void setMaterial(BBMaterial *pMaterial);
    void updateMaterialBlendState(bool bEnable);
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType = GL_TRIANGLES,
                int nDrawStartIndex = 0, int nDrawCount = 3);
    void setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);
    void setVisibility(bool bVisible) { m_bVisible = bVisible; }

    void updateOrderInRenderQueue(const QVector3D &renderableObjectPosition);
    float getDistanceToCamera(BBCamera *pCamera);

    void draw(BBCamera *pCamera);

public:
    inline BBMaterial* getMaterial() { return m_pMaterial; }

public:
    static void switchRenderingSettings(int nIndex);
    void onePassRendering(BBCamera *pCamera);
    void onePassRendering(BBCamera *pCamera, QList<BBGameObject*> lights);
    void forwardRendering(BBCamera *pCamera);
    void deferredRendering(BBCamera *pCamera);
    void uiRendering(BBCanvas *pCanvas);
    void fboRendering(BBCamera *pCamera);
    void shadowMapRendering(BBCamera *pCamera);

private:
    void updateOrderInOpaqueRenderQueue();
    void updateOrderInTransparentRenderQueue();
    BBUpdateOrderInRenderQueueFunc m_UpdateOrderInRenderQueueFunc;

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

    QVector3D m_RenderableObjectPosition;
    bool m_bVisible;
};


#endif // BBDRAWCALL_H
