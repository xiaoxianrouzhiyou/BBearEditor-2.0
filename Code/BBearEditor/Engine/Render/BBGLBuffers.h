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
    BBGLVertexBuffer(int nVertexCount);
    virtual ~BBGLVertexBuffer();

    void setPosition(int index, float x, float y, float z, float w = 1.0f);
    void setPosition(int index, const QVector3D &position);
    void setPosition(int index, const QVector4D &position);
    QVector3D getPosition(int index);
    inline float *getPosition() { return m_fPosition; }

    void setColor(int index, float r, float g, float b, float a = 1.0f);
    void setColor(int index, const QVector3D &rgb);
    void setColor(int index, const QVector4D &rgba);
    inline float *getColor() { return m_fColor; }

    void setTexcoord(int index, float u, float v);
    void setTexcoord(int index, const QVector2D &uv);
    inline float *getTexcoord() { return m_fTexcoord; }

    void setNormal(int index, float x, float y, float z);
    void setNormal(int index, const QVector3D &normal);
    void setNormal(int index, const QVector4D &normal);
    inline float *getNormal() { return m_fNormal; }

    inline int getVertexCount() { return m_nVertexCount; }

private:
    void setSize(int nVertexCount);

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
    BBGLTexture2D(int nWidth, int nHeight);
    BBGLTexture2D(const QString &fileName, bool bInvertY = true);
    void load(int nWidth, int nHeight, QRgb *pData);
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
