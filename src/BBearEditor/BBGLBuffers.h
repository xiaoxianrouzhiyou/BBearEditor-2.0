#ifndef BBGLBUFFERS_H
#define BBGLBUFFERS_H


#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QtOpenGL>
#include <QRgb>


class BBGLVertexBuffer
{
public:
    BBGLVertexBuffer(const int nVertexCount);
    ~BBGLVertexBuffer();

    void setPosition(const int index, const float x, const float y, const float z, const float w = 1.0f);
    void setPosition(const int index, const QVector3D position);
    void setPosition(const int index, const QVector4D position);
    QVector3D getPosition(const int index);
    inline float *getPosition() { return m_fPosition; }

    void setColor(const int index, const float r, const float g, const float b, const float a = 1.0f);
    void setColor(const int index, const QVector3D rgb);
    void setColor(const int index, const QVector4D rgba);
    inline float *getColor() { return m_fColor; }

    void setTexcoord(const int index, const float u, const float v);
    void setTexcoord(const int index, const QVector2D uv);
    inline float *getTexcoord() { return m_fTexcoord; }

    void setNormal(const int index, const float x, const float y, const float z);
    void setNormal(const int index, const QVector3D normal);
    void setNormal(const int index, const QVector4D normal);
    inline float *getNormal() { return m_fNormal; }

private:
    void setSize(const int nVertexCount);

    int m_nVertexCount;
    float *m_fPosition;
    float *m_fColor;
    float *m_fTexcoord;
    float *m_fNormal;
};


class BBGLTexture
{
public:
    BBGLTexture();
    virtual ~BBGLTexture();
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual bool failed() const { return m_bFailed; }
    GLuint m_nTexture;
    bool m_bFailed;
};

class BBGLTexture2D : public BBGLTexture
{
public:
    BBGLTexture2D(const int nWidth, const int nHeight);
    BBGLTexture2D(const QString &fileName, const bool bInvertY = true);
    void load(const int nWidth, const int nHeight, QRgb *pData);
    void bind() override;
    void unbind() override;
};


////程序纹理是程序生成的 不是读文件
//class GLProcedureTexture : public GLTexture
//{
//public:
//    explicit GLProcedureTexture(int size);
//    void bind() override;
//    void unbind() override;
//};



#endif // BBGLBUFFERS_H
