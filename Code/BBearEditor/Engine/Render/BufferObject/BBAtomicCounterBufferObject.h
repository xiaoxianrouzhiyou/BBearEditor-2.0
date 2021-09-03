#ifndef BBATOMICCOUNTERBUFFEROBJECT_H
#define BBATOMICCOUNTERBUFFEROBJECT_H


#include "BBBufferObject.h"

class BBAtomicCounterBufferObject : public BBBufferObject
{
public:
    BBAtomicCounterBufferObject();

    void clear();

    void bind() override;
    void unbind() override;
};

#endif // BBATOMICCOUNTERBUFFEROBJECT_H
