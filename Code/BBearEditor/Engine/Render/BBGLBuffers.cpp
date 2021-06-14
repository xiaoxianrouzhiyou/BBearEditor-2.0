#include "BBGLBuffers.h"


//--------------------
// BBGLTexture
//--------------------

BBGLTexture::BBGLTexture()
{
    m_nTexture = 0;
    m_bFailed = false;
    glGenTextures(1, &m_nTexture);
}

BBGLTexture::~BBGLTexture()
{
    glDeleteTextures(1, &m_nTexture);
}


//--------------------
// BBGLTexture2D
//--------------------

BBGLTexture2D::BBGLTexture2D(int nWidth, int nHeight)
{
    glBindTexture(GL_TEXTURE_2D, m_nTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

BBGLTexture2D::BBGLTexture2D(const QString &fileName, bool bInvertY)
    : BBGLTexture()
{
    // Add error handling.
    QImage image(fileName);
    if (image.isNull()) {
        m_bFailed = true;
        return;
    }

    image = image.convertToFormat(QImage::Format_ARGB32);
    if (bInvertY)
    {
        image = image.mirrored();
    }
    //qDebug() << "Image size:" << image.width() << "x" << image.height();
    /*if (width <= 0)
        width = image.width();
    if (height <= 0)
        height = image.height();
    if (width != image.width() || height != image.height())
        image = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);*/

    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    // GL_CLAMP_TO_EDGE Solve the skybox crack
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void BBGLTexture2D::load(int nWidth, int nHeight, QRgb *pData)
{
    glBindTexture(GL_TEXTURE_2D, m_nTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void BBGLTexture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_nTexture);
    glEnable(GL_TEXTURE_2D);
}

void BBGLTexture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

//GLProcedureTexture::GLProcedureTexture(int size)
//    : GLTexture()
//{
//    //size是宽高 正方形
//    //RGBA *4
//    unsigned char *imageData = new unsigned char[size * size * 4];
//    float halfSize = (float)size / 2.0f;
//    float maxDistance = halfSize * 1.414214f;
//    float centerX = halfSize;
//    float centerY = halfSize;
//    for (int y = 0; y < size; y++)
//    {
//        for (int x = 0; x < size; x++)
//        {
//            int currentPixelOffset = (y * size + x) * 4;
//            imageData[currentPixelOffset] = 255;
//            imageData[currentPixelOffset + 1] = 255;
//            imageData[currentPixelOffset + 2] = 255;
//            float deltaX = (float)x - centerX;
//            float deltaY = (float)y - centerY;
//            float distance = powf(sqrtf(deltaX * deltaX + deltaY * deltaY), 6.0f);
//            float alpha = 1.0f - distance / maxDistance;
//            alpha = alpha > 1.0f ? 1.0f : alpha;
//            imageData[currentPixelOffset + 3] = (unsigned char)(255.0f * alpha);
//        }
//    }
//    m_texture = Utils::createTexture2D(imageData, size, size, GL_RGBA);
//    delete[] imageData;
//}

//void GLProcedureTexture::bind()
//{
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glEnable(GL_TEXTURE_2D);
//}

//void GLProcedureTexture::unbind()
//{
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glDisable(GL_TEXTURE_2D);
//}
