#ifndef BBDRAWCALL_H
#define BBDRAWCALL_H


#include "BBMaterial.h"
#include "BBLinkedList.h"

class BBCamera;
class BBVertexBufferObject;
class BBElementBufferObject;

class BBDrawCall : public BBLinkedList
{
public:
    BBDrawCall();

    void setMaterial(BBMaterial *pMaterial);
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount);
    void setEBO(BBElementBufferObject *pEBO);

    void draw(BBCamera *pCamera);

private:
    BBMaterial *m_pMaterial;

    GLenum m_eDrawPrimitiveType;

    BBVertexBufferObject *m_pVBO;
    int m_nDrawStartIndex;
    int m_nDrawCount;

    BBElementBufferObject *m_pEBO;
    int m_nIndexCount;
};

#endif // BBDRAWCALL_H
