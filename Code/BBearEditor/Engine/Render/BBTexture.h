#ifndef BBTEXTURE_H
#define BBTEXTURE_H


#include "BBBaseRenderComponent.h"

class BBTexture : public BBBaseRenderComponent
{
public:
    BBTexture();

    GLuint createTexture2D(unsigned char *pPixelData, int nWidth, int nHeight, GLenum eType = GL_RGB);
    GLuint createTexture2D(const QString &path);
    GLuint createTexture2DFromBMP(const char *path);
};

#endif // BBTEXTURE_H
