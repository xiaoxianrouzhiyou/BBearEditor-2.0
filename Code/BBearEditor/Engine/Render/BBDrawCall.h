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
class BBShaderStorageBufferObject;
class BBAtomicCounterBufferObject;

typedef void (BBDrawCall::*BBDrawFunc)(BBCamera *pCamera);
typedef void (BBDrawCall::*BBUpdateOrderInRenderQueueFunc)();
typedef void (BBDrawCall::*BBBindFunc)();
typedef void (BBDrawCall::*BBUnbindFunc)();

class BBDrawCall : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBDrawCall();

    void setMaterial(BBMaterial *pMaterial);
    void updateMaterialBlendState(bool bEnable);
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType = GL_TRIANGLES, int nDrawStartIndex = 0, int nDrawCount = 3);
    void setSSBO(BBShaderStorageBufferObject *pSSBO, GLenum eDrawPrimitiveType = GL_TRIANGLES, int nDrawStartIndex = 0, int nDrawCount = 3);
    void setACBO(BBAtomicCounterBufferObject *pACBO, bool bClear);
    void removeACBO() { m_pACBO = nullptr; }
    void setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);
    void setVisibility(bool bVisible) { m_bVisible = bVisible; }

    void updateOrderInRenderQueue(const QVector3D &renderableObjectPosition);
    float getDistanceToCamera(BBCamera *pCamera);

    void draw(BBCamera *pCamera);

public:
    inline BBMaterial* getMaterial() { return m_pMaterial; }

public:
    static void switchRenderingSettings(int nIndex);
    void renderOnePass(BBCamera *pCamera);
    void renderOnePass(BBCamera *pCamera, QList<BBGameObject*> lights);
    void renderOnePassSSBO(BBCamera *pCamera);
    void renderForwardPass(BBCamera *pCamera);
    void renderUIPass(BBCanvas *pCanvas);
    void renderViewSpaceFBOPass(BBCamera *pCamera);
    void renderLightSpaceFBOPass(BBCamera *pCamera);

private:
    void updateOrderInOpaqueRenderQueue();
    void updateOrderInTransparentRenderQueue();
    BBUpdateOrderInRenderQueueFunc m_UpdateOrderInRenderQueueFunc;

private:
    void bindBufferObject();
    void unbindBufferObject();

    void bindVBO();
    void unbindVBO();

    void bindSSBO();
    void unbindSSBO();

    BBBindFunc m_BindFunc;
    BBUnbindFunc m_UnbindFunc;

private:
    QList<BBGameObject*> collectLights();

    static BBDrawFunc m_DrawFunc;
    BBMaterial *m_pMaterial;

    GLenum m_eDrawPrimitiveType;
    int m_nDrawStartIndex;

    BBVertexBufferObject *m_pVBO;
    BBShaderStorageBufferObject *m_pSSBO;
    bool m_bClearACBO;
    BBAtomicCounterBufferObject *m_pACBO;
    int m_nDrawCount;
    BBElementBufferObject *m_pEBO;
    int m_nIndexCount;

    QVector3D m_RenderableObjectPosition;
    bool m_bVisible;
};


#endif // BBDRAWCALL_H
