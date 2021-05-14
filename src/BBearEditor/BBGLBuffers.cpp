#include "BBGLBuffers.h"
#include "BBUtils.h"

BBGLVertexBuffer::BBGLVertexBuffer(const int nVertexCount)
{
    setSize(nVertexCount);
}

BBGLVertexBuffer::~BBGLVertexBuffer()
{
    BB_SAFE_DELETE_ARRAY(m_fPosition);
    BB_SAFE_DELETE_ARRAY(m_fColor);
    BB_SAFE_DELETE_ARRAY(m_fTexcoord);
    BB_SAFE_DELETE_ARRAY(m_fNormal);
}

void BBGLVertexBuffer::setPosition(const int index, const float x, const float y, const float z, const float w)
{
    m_fPosition[index * 4] = x;
    m_fPosition[index * 4 + 1] = y;
    m_fPosition[index * 4 + 2] = z;
    m_fPosition[index * 4 + 3] = w;
}

void BBGLVertexBuffer::setPosition(const int index, const QVector3D position)
{
    setPosition(index, position.x(), position.y(), position.z());
}

void BBGLVertexBuffer::setPosition(const int index, const QVector4D position)
{
    setPosition(index, position.x(), position.y(), position.z(), position.w());
}

QVector3D BBGLVertexBuffer::getPosition(const int index)
{
    return QVector3D(m_fPosition[index * 4], m_fPosition[index * 4 + 1], m_fPosition[index * 4 + 2]);
}

void BBGLVertexBuffer::setColor(const int index, const float r, const float g, const float b, const float a)
{
    m_fColor[index * 4] = r;
    m_fColor[index * 4 + 1] = g;
    m_fColor[index * 4 + 2] = b;
    m_fColor[index * 4 + 3] = a;
}

void BBGLVertexBuffer::setColor(const int index, const QVector3D rgb)
{
    setColor(index, rgb.x(), rgb.y(), rgb.z());
}

void BBGLVertexBuffer::setColor(const int index, const QVector4D rgba)
{
    setColor(index, rgba.x(), rgba.y(), rgba.z(), rgba.w());
}

void BBGLVertexBuffer::setTexcoord(const int index, const float u, const float v)
{
    m_fTexcoord[index * 2] = u;
    m_fTexcoord[index * 2 + 1] = v;
}

void BBGLVertexBuffer::setTexcoord(const int index, const QVector2D uv)
{
    setTexcoord(index, uv.x(), uv.y());
}

void BBGLVertexBuffer::setNormal(const int index, const float x, const float y, const float z)
{
    m_fNormal[index * 4] = x;
    m_fNormal[index * 4 + 1] = y;
    m_fNormal[index * 4 + 2] = z;
    m_fNormal[index * 4 + 3] = 1.0f;
}

void BBGLVertexBuffer::setNormal(const int index, const QVector3D normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBGLVertexBuffer::setNormal(const int index, const QVector4D normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBGLVertexBuffer::setSize(const int nVertexCount)
{
    m_nVertexCount = nVertexCount;
    m_fPosition = new float[m_nVertexCount * 4];
    m_fColor = new float[m_nVertexCount * 4];
    m_fTexcoord = new float[m_nVertexCount * 2];
    m_fNormal = new float[m_nVertexCount * 4];
    memset(m_fPosition, 0, sizeof(float) * m_nVertexCount * 4);
    memset(m_fColor, 0, sizeof(float) * m_nVertexCount * 4);
    memset(m_fTexcoord, 0, sizeof(float) * m_nVertexCount * 2);
    memset(m_fNormal, 0, sizeof(float) * m_nVertexCount * 4);
}

//GLTexture::GLTexture() : m_texture(0), m_failed(false)
//{
//    glGenTextures(1, &m_texture);
//}

//GLTexture::~GLTexture()
//{
//    glDeleteTextures(1, &m_texture);
//}

//GLTexture2D::GLTexture2D(int width, int height)
//{
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}


//GLTexture2D::GLTexture2D(const QString &fileName, bool invertY)
//    : GLTexture()
//{
//    // TODO: Add error handling.
//    QImage image(fileName);
//    if (image.isNull()) {
//        m_failed = true;
//        return;
//    }

//    image = image.convertToFormat(QImage::Format_ARGB32);
//    if (invertY)
//    {
//        image = image.mirrored();
//    }
//    //qDebug() << "Image size:" << image.width() << "x" << image.height();
//    /*if (width <= 0)
//        width = image.width();
//    if (height <= 0)
//        height = image.height();
//    if (width != image.width() || height != image.height())
//        image = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);*/

//    glBindTexture(GL_TEXTURE_2D, m_texture);

//    // Works on x86, so probably works on all little-endian systems.
//    // Does it work on big-endian systems?
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0,
//        GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

//    //GL_CLAMP_TO_EDGE解决天空盒裂缝
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}

//void GLTexture2D::load(int width, int height, QRgb *data)
//{
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}

//void GLTexture2D::bind()
//{
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glEnable(GL_TEXTURE_2D);
//}

//void GLTexture2D::unbind()
//{
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glDisable(GL_TEXTURE_2D);
//}

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
