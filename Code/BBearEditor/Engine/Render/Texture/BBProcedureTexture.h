#ifndef BBPROCEDURETEXTURE_H
#define BBPROCEDURETEXTURE_H


#include "BBTexture.h"

class BBProcedureTexture : public BBTexture
{
public:
    BBProcedureTexture();

    GLuint create0(int nSize);
    GLuint create3D0(int nWidth, int nHeight, int nDepth);
    GLuint createPerlinNoiseTexture2D(int nSize, float fScale = 1.0f);
    GLuint createCamouflagePerlinNoiseTexture2D(int nSize, float fScale = 1.0f);
    GLuint createPerlinNoiseTexture3D(int nWidth, int nHeight, int nDepth, float fScale = 1.0f);
};

#endif // BBPROCEDURETEXTURE_H
