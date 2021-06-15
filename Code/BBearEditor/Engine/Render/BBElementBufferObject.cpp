#include "BBElementBufferObject.h"
#include "BBUtils.h"
#include <QOpenGLBuffer>


BBElementBufferObject::BBElementBufferObject(const unsigned short *pIndexes, int nIndexCount)
    : BBBufferObject()
{
    m_pBuffer = NULL;
    set(pIndexes, nIndexCount);
}

BBElementBufferObject::~BBElementBufferObject()
{
    BB_SAFE_DELETE(m_pBuffer);
}

void BBElementBufferObject::set(const unsigned short *pIndexes, int nIndexCount)
{
    if (m_pBuffer)
    {
        BB_SAFE_DELETE(m_pBuffer);
    }
    m_pBuffer= new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pBuffer->create();
    m_pBuffer->bind();
    m_pBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_pBuffer->allocate(pIndexes, sizeof(unsigned short) * nIndexCount);
    m_pBuffer->release();
}

void BBElementBufferObject::bind()
{
    BB_PROCESS_ERROR_RETURN(m_pBuffer);
    m_pBuffer->bind();
}

void BBElementBufferObject::release()
{
    BB_PROCESS_ERROR_RETURN(m_pBuffer);
    m_pBuffer->release();
}
