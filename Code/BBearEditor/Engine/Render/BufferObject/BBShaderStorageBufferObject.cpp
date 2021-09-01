#include "BBShaderStorageBufferObject.h"


BBShaderStorageBufferObject::BBShaderStorageBufferObject(BBVertexBufferObject *pVBO)
    : BBShaderStorageBufferObject(pVBO->getVertexCount())
{
    // copy the content of VBO
    for (int i = 0; i < m_nVertexCount; i++)
    {
        setPosition(i, pVBO->getPosition(i));
        setColor(i, pVBO->getColor(i));
        setTexcoord(i, pVBO->getTexcoord(i));
        setNormal(i, pVBO->getNormal(i));
        setTangent(i, pVBO->getTangent(i));
        setBiTangent(i, pVBO->getBiTangent(i));
    }
}

BBShaderStorageBufferObject::BBShaderStorageBufferObject(int nVertexCount)
    : BBVertexBufferObject()
{
    m_BufferType = GL_SHADER_STORAGE_BUFFER;
    setSize(nVertexCount);
}

BBShaderStorageBufferObject::~BBShaderStorageBufferObject()
{

}

void BBShaderStorageBufferObject::bind()
{
    // Parameter 2 corresponds to the binding tag in the shader
    glBindBufferBase(m_BufferType, 0, m_Name);
}

void BBShaderStorageBufferObject::unbind()
{
    glBindBufferBase(m_BufferType, 0, 0);
}
