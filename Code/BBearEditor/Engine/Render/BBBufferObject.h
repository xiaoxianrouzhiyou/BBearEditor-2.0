#ifndef BBBUFFEROBJECT_H
#define BBBUFFEROBJECT_H


#include "BBBaseRenderComponent.h"

class BBBufferObject : public BBBaseRenderComponent
{
public:
    BBBufferObject();
    virtual ~BBBufferObject();

    void bind();
    void unbind();

    virtual void draw() = 0;

protected:
    GLuint createBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *pData = NULL);
    void updateData(GLenum bufferType, GLsizeiptr size, void *pData = NULL);

    GLuint m_Name;
    GLenum m_BufferType;

    GLenum m_eDrawPrimitiveType;
    int m_nDrawStartIndex;
    int m_nDrawCount;
};

#endif // BBBUFFEROBJECT_H
