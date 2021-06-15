#ifndef BBGLBUFFERS_H
#define BBGLBUFFERS_H


#include "BBBaseRenderComponent.h"


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
