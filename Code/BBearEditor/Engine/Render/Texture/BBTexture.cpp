#include "BBTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


BBTexture::BBTexture()
    : BBBaseRenderComponent()
{

}

/**
 * @brief BBTexture::createTexture2D
 * @param pPixelData
 * @param nWidth
 * @param nHeight
 * @param eType                         RGB or RGBA ...
 * @return
 */
GLuint BBTexture::createTexture2D(unsigned char *pPixelData, int nWidth, int nHeight, GLenum eType)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // from CPU to GPU
    // 3: GUP
    // 6: = 0
    // 7: CPU
    glTexImage2D(GL_TEXTURE_2D, 0, eType, nWidth, nHeight, 0, eType, GL_UNSIGNED_BYTE, pPixelData);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint BBTexture::createTexture2D(float *pPixelData, int nWidth, int nHeight, GLenum eType)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // from CPU to GPU
    // 3: GUP
    // 6: = 0
    // 7: CPU
    glTexImage2D(GL_TEXTURE_2D, 0, eType, nWidth, nHeight, 0, GL_RGB, GL_FLOAT, pPixelData);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint BBTexture::createTexture2D(QImage image, GLenum eType)
{
    return createTexture2D(image.bits(), image.width(), image.height(), eType);
}

GLuint BBTexture::createTexture2D(const QString &path, GLenum eType)
{
    QImage image(path);
    if (image.isNull())
    {
        image = QImage(128, 128, QImage::Format_RGB32);
        image.fill(QColor(255, 255, 255));
    }
    image = image.mirrored(false, true);
    image = image.rgbSwapped();
    return createTexture2D(image.bits(), image.width(), image.height(), eType);
}

GLuint BBTexture::createTexture2DFromBMP(const char *path)
{
    // black
    GLuint texture = 0;
    unsigned char *pBmpFileContent = NULL;
    do
    {
        int nFileSize = 0;
        pBmpFileContent = (unsigned char*) BBUtils::loadFileContent(path, nFileSize);
        BB_PROCESS_ERROR(pBmpFileContent);

        int nBmpWidth = 0;
        int nBmpHeight = 0;
        unsigned char *pPixelData = BBUtils::decodeBMP(pBmpFileContent, nBmpWidth, nBmpHeight);
        BB_PROCESS_ERROR(pPixelData);

        texture = createTexture2D(pPixelData, nBmpWidth, nBmpHeight, GL_RGB);
    } while(0);

    BB_SAFE_DELETE(pBmpFileContent);
    return texture;
}

GLuint BBTexture::createHDRTexture2D(const char *pFilePath)
{
    stbi_set_flip_vertically_on_load(true);
    int nWidth;
    int nHeight;
    int nrComponents;
    float *pData = stbi_loadf(pFilePath, &nWidth, &nHeight, &nrComponents, 0);
    GLuint hdrTexture = 0;
    if (pData)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, nWidth, nHeight, 0, GL_RGB, GL_FLOAT, pData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(pData);
    }
    else
    {
        qDebug() << "Failed to load HDR image.";
    }
    return hdrTexture;
}

GLuint BBTexture::createTextureCube(const QString paths[], GLenum eType)
{
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++)
    {
        QImage image(paths[i]);
        image = image.mirrored(false, true);
        image = image.rgbSwapped();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, eType, image.width(), image.height(), 0, eType, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texture;
}

GLuint BBTexture::allocateTexture2D(int nWidth, int nHeight, GLint internalFormat, GLenum format, GLint minFilter)
{
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, nWidth, nHeight, 0, format, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void BBTexture::startWritingTexture2D(GLuint texture)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    // Subsequent rendering code
}

void BBTexture::generateTexture2DMipmap(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint BBTexture::allocateTexture2DMipmap(int nWidth, int nHeight, GLint internalFormat, GLenum format)
{
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, nWidth, nHeight, format, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Enable trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // To ensure that enough memory is allocated for Mipmap
//    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void BBTexture::startWritingTexture2DMipmap(GLuint texture, int nMipLevel)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, nMipLevel);
    // Subsequent rendering code
}

/**
 * @brief BBTexture::allocateTextureCube                    Allocate memory for cube map
 * @param nWidth
 * @param nHeight
 * @param eType
 * @return
 */
GLuint BBTexture::allocateTextureCube(int nWidth, int nHeight, GLint internalFormat, GLenum format, GLint minFilter)
{
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, nWidth, nHeight, 0, format, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texture;
}

/**
 * @brief BBTexture::startWritingTextureCube                start writing mode, write into the memory that is allocated by BBTexture::allocateTextureCube
 * @param texture
 */
void BBTexture::startWritingTextureCube(GLuint texture, int nSideIndex)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + nSideIndex, texture, 0);
    // Subsequent rendering code
}

void BBTexture::generateTextureCubeMipmap(GLuint texture)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

GLuint BBTexture::allocateTextureCubeMipmap(int nWidth, int nHeight, GLint internalFormat, GLenum format)
{
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, nWidth, nHeight, 0, format, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Enable trilinear filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // To ensure that enough memory is allocated for Mipmap
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texture;
}

void BBTexture::startWritingTextureCubeMipmap(GLuint texture, int nSideIndex, int nMipLevel)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + nSideIndex, texture, nMipLevel);
    // Subsequent rendering code
}
