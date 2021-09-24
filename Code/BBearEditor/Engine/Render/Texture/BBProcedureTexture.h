#ifndef BBPROCEDURETEXTURE_H
#define BBPROCEDURETEXTURE_H


#include "BBTexture.h"

class BBProcedureTexture : public BBTexture
{
public:
    BBProcedureTexture();

    GLuint create0(int nSize);
    GLuint create3D0(int nWidth, int nHeight, int nDepth);
    GLuint createPerlinNoise(const QVector3D &p);
};

#endif // BBPROCEDURETEXTURE_H
