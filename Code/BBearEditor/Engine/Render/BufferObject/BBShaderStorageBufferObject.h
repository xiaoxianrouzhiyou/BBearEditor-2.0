#ifndef BBSHADERSTORAGEBUFFEROBJECT_H
#define BBSHADERSTORAGEBUFFEROBJECT_H


#include "BBVertexBufferObject.h"
#include "../BBLinkedList.h"

class BBShaderStorageBufferObject : public BBVertexBufferObject, public BBLinkedList
{
public:
    BBShaderStorageBufferObject();
    BBShaderStorageBufferObject(BBVertexBufferObject *pVBO);
    BBShaderStorageBufferObject(int nVertexCount);

    ~BBShaderStorageBufferObject();

    void bind() override;
    void bind(int location);
    void unbind() override;

    template<typename T>
    /**
     * @brief createBufferObject            allocate nCount T
     * @param location                      Consistent with layout binding in shader
     * @param nCount
     * @param hint
     * @param pData
     */
    void createBufferObject(GLint location, int nCount, GLenum hint = GL_STATIC_DRAW, void *pData = nullptr)
    {
        m_Location = location;
        BBVertexBufferObject::createBufferObject(m_BufferType, sizeof(T) * nCount, hint, pData);
    }


private:
    GLint m_Location;
};

#endif // BBSHADERSTORAGEBUFFEROBJECT_H
