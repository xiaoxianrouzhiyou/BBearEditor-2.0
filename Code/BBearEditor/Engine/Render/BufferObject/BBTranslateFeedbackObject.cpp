#include "BBTranslateFeedbackObject.h"
#include "BBVertexBufferObject.h"


BBTranslateFeedbackObject::BBTranslateFeedbackObject(int nVertexCount, GLenum hint, GLenum eDrawPrimitiveType)
    : BBBufferObject()
{
    m_nVertexCount = nVertexCount;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    createTFO();
}

void BBTranslateFeedbackObject::bind()
{
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TFO);
    // all data of TFO will be written into the buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_Name);
    glBeginTransformFeedback(m_eDrawPrimitiveType);
}

void BBTranslateFeedbackObject::unbind()
{
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void BBTranslateFeedbackObject::debug()
{
    BBBufferObject::bind();
    // extract from gpu
    BBVertex *pVertexes = new BBVertex[m_nVertexCount];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(BBVertex) * m_nVertexCount, pVertexes);
    if (pVertexes)
    {
        for (int i = 0; i < m_nVertexCount; i++)
        {
            qDebug() << pVertexes[i].m_fPosition[0] << pVertexes[i].m_fPosition[1] << pVertexes[i].m_fPosition[2];
        }
    }
    else
    {
        qDebug() << "error";
    }
    glUnmapBuffer(m_BufferType);
    BBBufferObject::unbind();
}

GLuint BBTranslateFeedbackObject::createTFO()
{
    glGenTransformFeedbacks(1, &m_TFO);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TFO);
    m_Name = createBufferObject(m_BufferType, sizeof(BBVertex) * m_nVertexCount, GL_STATIC_DRAW, nullptr);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    return m_TFO;
}
