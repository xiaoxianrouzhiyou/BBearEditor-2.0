#ifndef BBELEMENTBUFFEROBJECT_H
#define BBELEMENTBUFFEROBJECT_H


#include "BBBufferObject.h"

class QOpenGLBuffer;

class BBElementBufferObject : public BBBufferObject
{
public:
    BBElementBufferObject(const unsigned short *pIndexes, int nIndexCount);
    ~BBElementBufferObject();

    void set(const unsigned short *pIndexes, int nIndexCount);
    void bind();
    void release();

private:
    QOpenGLBuffer *m_pBuffer;
};

#endif // BBELEMENTBUFFEROBJECT_H
