#ifndef BBSHADERSTORAGEBUFFEROBJECT_H
#define BBSHADERSTORAGEBUFFEROBJECT_H


#include "BBVertexBufferObject.h"

class BBShaderStorageBufferObject : public BBVertexBufferObject
{
public:
    BBShaderStorageBufferObject(int nVertexCount);
    ~BBShaderStorageBufferObject();

    void bind() override;
    void unbind() override;

private:
};

#endif // BBSHADERSTORAGEBUFFEROBJECT_H
