#ifndef BBDRAWCALL_H
#define BBDRAWCALL_H


#include "BBMaterial.h"
#include "BBLinkedList.h"

class BBCamera;
class BBVertexBufferObject;
class BBElementBufferObject;
class BBGameObject;
class BBDrawCall;

typedef void (BBDrawCall::*BBDrawFunc)(BBCamera *pCamera);

class BBDrawCall : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBDrawCall();

    void setMaterial(BBMaterial *pMaterial);
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType = GL_TRIANGLES,
                int nDrawStartIndex = 0, int nDrawCount = 3);
    void setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);

    void draw(BBCamera *pCamera);

public:
    static void setDrawFunc(int nIndex);
    void onePassRendering(BBCamera *pCamera);
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

#endif // BBDRAWCALL_H
