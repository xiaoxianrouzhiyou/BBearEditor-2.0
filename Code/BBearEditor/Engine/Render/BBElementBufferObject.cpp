#include "BBElementBufferObject.h"
#include "BBUtils.h"
#include <QOpenGLBuffer>


BBElementBufferObject::BBElementBufferObject()
    : BBBufferObject()
{

}

void BBElementBufferObject::setSize(int nIndexCount, GLenum hint)
{
    createBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * nIndexCount, hint, nullptr);
}

