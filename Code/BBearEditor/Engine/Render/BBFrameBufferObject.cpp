#include "BBFrameBufferObject.h"

BBFrameBufferObject::BBFrameBufferObject()
{
    glGenFramebuffers(1, &m_FrameBufferObject);
}

void BBFrameBufferObject::attachColorBuffer(QString bufferName, GLenum attachment, int nWidth, int nHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
    GLuint colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // open up space for texture objects on the GPU
    // The last parameter is NULL, only open up space in the GPU, and will not transfer data from the CPU.
    // The specific data is written by the GPU during rendering.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    // give the color buffer to the attachment node
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
    // record nodes for subsequent use
    m_DrawBuffers.append(attachment);
    m_Buffers.insert(bufferName, colorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BBFrameBufferObject::attachDepthBuffer(QString bufferName, int nWidth, int nHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
    GLuint depthBuffer;
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, nWidth, nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
    m_Buffers.insert(bufferName, depthBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BBFrameBufferObject::finish()
{
    // number of draw targets
    int count = m_DrawBuffers.count();
    if (count > 0)
    {
        GLenum *pBuffers = new GLenum[count];
        for (int i = 0; i < count; i++)
        {
            pBuffers[i] = m_DrawBuffers.at(i);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
        glDrawBuffers(count, pBuffers);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void BBFrameBufferObject::bind()
{
    // save the previous render target
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PreFrameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
    // all the things rendered afterwards will be drawn to this FBO
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BBFrameBufferObject::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_PreFrameBufferObject);
}

GLuint BBFrameBufferObject::getBuffer(QString bufferName)
{
    QMap<QString, GLuint>::Iterator it = m_Buffers.find(bufferName);
    if (it != m_Buffers.end())
    {
        return it.value();
    }
    else
    {
        return 0;
    }
}