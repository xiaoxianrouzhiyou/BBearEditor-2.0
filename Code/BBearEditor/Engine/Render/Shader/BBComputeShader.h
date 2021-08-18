#ifndef BBCOMPUTESHADER_H
#define BBCOMPUTESHADER_H


#include "BBShader.h"

class BBComputeShader : public BBShader
{
public:
    BBComputeShader();

    void init(const QString &shaderPath);

    void bind(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ);
    void unbind();
};

#endif // BBCOMPUTESHADER_H
