#include "BBTexture.h"

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

GLuint BBTexture::createTextureCube(const QString paths[], GLenum eType)
{
    return 0;
}
