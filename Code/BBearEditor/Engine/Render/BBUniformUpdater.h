#ifndef BBUNIFORMUPDATER_H
#define BBUNIFORMUPDATER_H


#include "BBBaseRenderComponent.h"
#include "BBLinkedList.h"


enum BBMaterialUniformPropertyType
{
    CameraProjectionMatrix,
    CameraViewMatrix,
    Matrix4,
    Count
};

class BBUniformUpdater : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBUniformUpdater(GLint location, const BBMaterialUniformPropertyType &eType);

    inline BBMaterialUniformPropertyType getPropertyType() const { return m_eType; }
    void setData(const float *pData);
    void update(void *pData);

private:
    GLint m_Location;
    BBMaterialUniformPropertyType m_eType;
    const float *m_pData;
};

#endif // BBUNIFORMUPDATER_H
