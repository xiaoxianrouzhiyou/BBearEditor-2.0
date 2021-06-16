#ifndef BBELEMENTBUFFEROBJECT_H
#define BBELEMENTBUFFEROBJECT_H


#include "BBBufferObject.h"

class BBElementBufferObject : public BBBufferObject
{
public:
    BBElementBufferObject();

    void setSize(int nIndexCount, GLenum hint = GL_STATIC_DRAW);

private:

};

#endif // BBELEMENTBUFFEROBJECT_H
