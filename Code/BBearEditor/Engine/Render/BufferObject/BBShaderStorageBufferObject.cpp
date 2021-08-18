#include "BBShaderStorageBufferObject.h"

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
