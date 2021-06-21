#include "BBBufferObject.h"


BBBufferObject::BBBufferObject()
    : BBBaseRenderComponent()
{
    m_Name = 0;
    m_BufferType = GL_ARRAY_BUFFER;
}

BBBufferObject::~BBBufferObject()
{
    if (m_Name != 0)
    {
        glDeleteBuffers(1, &m_Name);
    }
}

void BBBufferObject::updateData(GLenum bufferType, GLsizeiptr size, const void *pData)
{
    glBindBuffer(bufferType, m_Name);
    glBufferSubData(bufferType, 0, size, pData);
    glBindBuffer(bufferType, 0);
}

void BBBufferObject::bind()
{
    glBindBuffer(m_BufferType, m_Name);
}

void BBBufferObject::unbind()
{
    glBindBuffer(m_BufferType, 0);
}

GLuint BBBufferObject::createBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *pData)
{
    m_BufferType = bufferType;
    glGenBuffers(1, &m_Name);
    glBindBuffer(bufferType, m_Name);
    glBufferData(bufferType, size, pData, usage);
    glBindBuffer(bufferType, 0);
    return m_Name;
}
