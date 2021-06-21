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
    void setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType = GL_TRIANGLES,
                int nDrawStartIndex = 0, int nDrawCount = 3);
    void setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);

    void draw(BBCamera *pCamera);

private:
    BBMaterial *m_pMaterial;

    GLenum m_eDrawPrimitiveType;
    int m_nDrawStartIndex;

    BBVertexBufferObject *m_pVBO;
    int m_nDrawCount;

    BBElementBufferObject *m_pEBO;
    int m_nIndexCount;
};

#endif // BBDRAWCALL_H
