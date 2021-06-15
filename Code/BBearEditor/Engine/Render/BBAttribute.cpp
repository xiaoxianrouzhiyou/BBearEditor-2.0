#include "BBAttribute.h"


BBAttribute::BBAttribute(GLint location, int nComponentCount, unsigned int nBasicDataType,
                         bool bNormalized, int nDataStride, int nDataOffset)
    : BBBaseRenderComponent()
{
    m_Location = location;
    m_nComponentCount = nComponentCount;
    m_nBasicDataType = nBasicDataType;
    m_bNormalized = bNormalized;
    m_nDataStride = nDataStride;
    m_nDataOffset = nDataOffset;
}

void BBAttribute::active()
{
    glEnableVertexAttribArray(m_Location);
    glVertexAttribPointer(m_Location, m_nComponentCount, m_nBasicDataType,
                          m_bNormalized, m_nDataStride, (void*)m_nDataOffset);
    // activate the entire attribute chain
    if (m_pNext != nullptr)
    {
        next<BBAttribute>()->active();
    }
}
