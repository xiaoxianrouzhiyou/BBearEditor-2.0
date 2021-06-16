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
    void setVBO(BBVertexBufferObject *pVBO);
    void setEBO(BBElementBufferObject *pEBO);

    void draw(BBCamera *pCamera);

private:
    BBMaterial *m_pMaterial;
    BBVertexBufferObject *m_pVBO;
    BBElementBufferObject *m_pEBO;
};

#endif // BBDRAWCALL_H
