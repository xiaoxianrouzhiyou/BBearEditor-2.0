#include "BBUniformUpdater.h"
#include "BBCamera.h"


BBUniformUpdater::BBUniformUpdater(GLint location, const BBMaterialUniformPropertyType &eType)
    : BBBaseRenderComponent()
{
    m_Location = location;
    m_eType = eType;
    m_pData = NULL;
}

void BBUniformUpdater::setData(const float *pData)
{
    m_pData = pData;
}

void BBUniformUpdater::update(void *pData)
{
    switch (m_eType) {
    case CameraProjectionMatrix:
        glUniformMatrix4fv(m_Location, 1, GL_FALSE, ((BBCamera*)pData)->getProjectionMatrix().data());
        break;
    case CameraViewMatrix:
        glUniformMatrix4fv(m_Location, 1, GL_FALSE, ((BBCamera*)pData)->getViewMatrix().data());
        break;
    case Matrix4:
        glUniformMatrix4fv(m_Location, 1, GL_FALSE, m_pData);
        break;
    default:
        break;
    }

    if (m_pNext != nullptr)
    {
        next<BBUniformUpdater>()->update(pData);
    }
}
