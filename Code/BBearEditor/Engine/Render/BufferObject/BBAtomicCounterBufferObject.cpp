#include "BBAtomicCounterBufferObject.h"

BBAtomicCounterBufferObject::BBAtomicCounterBufferObject()
    : BBBufferObject()
{
    createBufferObject(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), GL_DYNAMIC_DRAW);
    clear();
}

void BBAtomicCounterBufferObject::clear()
{
    GLuint *pContent = nullptr;
    glBindBuffer(m_BufferType, m_Name);
    pContent = (GLuint*)glMapBufferRange(m_BufferType, 0, sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
    memset(pContent, 0, sizeof(GLuint));
    glUnmapBuffer(m_BufferType);
}

void BBAtomicCounterBufferObject::bind()
{
    // Parameter 2 corresponds to the binding tag in the shader
    glBindBufferBase(m_BufferType, 0, m_Name);
}

void BBAtomicCounterBufferObject::unbind()
{
    glBindBufferBase(m_BufferType, 0, 0);
}
