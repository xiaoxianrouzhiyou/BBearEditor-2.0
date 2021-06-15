#ifndef BBATTRIBUTE_H
#define BBATTRIBUTE_H


#include "BBBaseRenderComponent.h"
#include "BBLinkedList.h"

class BBAttribute : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBAttribute(GLint location, int nComponentCount, unsigned int nBasicDataType,
                bool bNormalized, int nDataStride, int nDataOffset);

    void active();

private:
    GLint m_Location;
    int m_nComponentCount;
    unsigned int m_nBasicDataType;
    bool m_bNormalized;
    int m_nDataStride;
    int m_nDataOffset;
};

#endif // BBATTRIBUTE_H
