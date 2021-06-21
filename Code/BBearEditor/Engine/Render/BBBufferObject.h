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

protected:
    GLuint createBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *pData = NULL);
    void updateData(GLenum bufferType, GLsizeiptr size, void *pData = NULL);

    GLuint m_Name;
    GLenum m_BufferType;
};

#endif // BBBUFFEROBJECT_H
