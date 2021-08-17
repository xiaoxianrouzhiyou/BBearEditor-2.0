#ifndef BBPROCEDURETEXTURE_H
#define BBPROCEDURETEXTURE_H


#include "BBTexture.h"

class BBProcedureTexture : public BBTexture
{
public:
    BBProcedureTexture();

    GLuint create(int nSize);
};

#endif // BBPROCEDURETEXTURE_H
