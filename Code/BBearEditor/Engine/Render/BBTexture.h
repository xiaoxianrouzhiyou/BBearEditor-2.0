#ifndef BBTEXTURE_H
#define BBTEXTURE_H


#include "BBBaseRenderComponent.h"

class BBTexture : public BBBaseRenderComponent
{
public:
    BBTexture();

    GLuint createTexture2D(unsigned char *pPixelData, int nWidth, int nHeight, GLenum eType = GL_RGB);
    GLuint createTexture2D(QImage image, GLenum eType = GL_RGBA);
    GLuint createTexture2D(const QString &path = "", GLenum eType = GL_RGBA);
    GLuint createTexture2DFromBMP(const char *path);
};

#endif // BBTEXTURE_H
