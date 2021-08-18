#ifndef BBELEMENTBUFFEROBJECT_H
#define BBELEMENTBUFFEROBJECT_H


#include "BBBufferObject.h"

class BBElementBufferObject : public BBBufferObject
{
public:
    BBElementBufferObject(int nIndexCount);

    void setSize(int nIndexCount, GLenum hint = GL_STATIC_DRAW);
    void submitData(const unsigned short *pIndexes, int nIndexCount);

    void draw(GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex);

private:

};

#endif // BBELEMENTBUFFEROBJECT_H
