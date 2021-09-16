#ifndef BBTEXTURE_H
#define BBTEXTURE_H


#include "../BBBaseRenderComponent.h"

class BBTexture : public BBBaseRenderComponent
{
public:
    BBTexture();

    GLuint createTexture2D(unsigned char *pPixelData, int nWidth, int nHeight, GLenum eType = GL_RGB);
    GLuint createTexture2D(float *pPixelData, int nWidth, int nHeight, GLenum eType = GL_RGBA32F);
    GLuint createTexture2D(QImage image, GLenum eType = GL_RGBA);
    GLuint createTexture2D(const QString &path = "", GLenum eType = GL_RGBA);
    GLuint createTexture2DFromBMP(const char *path);

    GLuint createHDRTexture2D(const char *pFilePath);

    GLuint createTextureCube(const QString paths[], GLenum eType = GL_RGBA);

public:
    GLuint allocateTexture2D(int nWidth, int nHeight, GLint internalFormat = GL_RG16F, GLenum format = GL_RG, GLint minFilter = GL_LINEAR);
    void startWritingTexture2D(GLuint texture);
    void generateTexture2DMipmap(GLuint texture);

    GLuint allocateTexture2DMipmap(int nWidth, int nHeight, GLint internalFormat = GL_RG16F, GLenum format = GL_RG);
    void startWritingTexture2DMipmap(GLuint texture, int nMipLevel);

    GLuint allocateTextureCube(int nWidth, int nHeight, GLint internalFormat = GL_RGB16F, GLenum format = GL_RGB, GLint minFilter = GL_LINEAR);
    void startWritingTextureCube(GLuint texture, int nSideIndex);
    void generateTextureCubeMipmap(GLuint texture);

    GLuint allocateTextureCubeMipmap(int nWidth, int nHeight, GLint internalFormat = GL_RGB16F, GLenum format = GL_RGB);
    void startWritingTextureCubeMipmap(GLuint texture, int nSideIndex, int nMipLevel);
};

#endif // BBTEXTURE_H
