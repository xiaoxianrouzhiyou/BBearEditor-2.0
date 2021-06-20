#ifndef BBFRAMEBUFFEROBJECT_H
#define BBFRAMEBUFFEROBJECT_H


#include "BBBaseRenderComponent.h"

class BBFrameBufferObject : public BBBaseRenderComponent
{
public:
    BBFrameBufferObject();

    /* one or more */
    void attachColorBuffer(QString bufferName, GLenum attachment, int nWidth, int nHeight);
    /* only one */
    void attachDepthBuffer(QString bufferName, int nWidth, int nHeight);
    /* make the settings take effect */
    void finish();

    void bind();
    void unbind();

    GLuint getBuffer(QString bufferName);

private:
    GLuint m_FrameBufferObject;
    GLint m_PreFrameBufferObject;
    /* each buffer and its name */
    QMap<QString, GLuint> m_Buffers;
    /* buffer in which FBO need to render */
    QList<GLenum> m_DrawBuffers;
};

#endif // BBFRAMEBUFFEROBJECT_H



