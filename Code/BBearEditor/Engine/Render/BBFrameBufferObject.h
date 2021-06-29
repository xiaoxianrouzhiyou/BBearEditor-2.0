#ifndef BBFRAMEBUFFEROBJECT_H
#define BBFRAMEBUFFEROBJECT_H


#include "BBBaseRenderComponent.h"

class BBFrameBufferObject : public BBBaseRenderComponent
{
public:
    BBFrameBufferObject();

    /* one or more */
    void attachColorBuffer(const QString &bufferName, GLenum attachment, int nWidth, int nHeight);
    /* only one */
    void attachDepthBuffer(const QString &bufferName, int nWidth, int nHeight);
    /* make the settings take effect */
    void finish();

    void bind();
    void unbind();

    GLuint getBuffer(const QString &bufferName);
    QPixmap getPixmap();

private:
    GLuint m_FrameBufferObject;
    GLint m_PreFrameBufferObject;
    int m_nWidth;
    int m_nHeight;
    /* each buffer and its name */
    QMap<QString, GLuint> m_Buffers;
    /* buffer in which FBO need to render */
    QList<GLenum> m_DrawBuffers;
};

#endif // BBFRAMEBUFFEROBJECT_H



