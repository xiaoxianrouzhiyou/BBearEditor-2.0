#include "BBElementBufferObject.h"
#include "Utils/BBUtils.h"
#include <QOpenGLBuffer>


BBElementBufferObject::BBElementBufferObject(int nIndexCount)
    : BBBufferObject()
{
    setSize(nIndexCount);
}

void BBElementBufferObject::setSize(int nIndexCount, GLenum hint)
{
    m_Name = createBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * nIndexCount, hint, nullptr);
}

void BBElementBufferObject::submitData(const unsigned short *pIndexes, int nIndexCount)
{
    updateData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * nIndexCount, pIndexes);
}

void BBElementBufferObject::draw(GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex)
{
    glDrawElements(eDrawPrimitiveType, nIndexCount, GL_UNSIGNED_SHORT, (void*) (sizeof(unsigned short) * nDrawStartIndex));
}
