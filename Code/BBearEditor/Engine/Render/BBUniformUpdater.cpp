#include "BBUniformUpdater.h"
#include "BBCamera.h"


BBUniformUpdater::BBUniformUpdater(GLint location, const BBUpdateUniformFunc &updateFunc, BBMaterialProperty *pTargetProperty)
    : BBBaseRenderComponent()
{
    m_Location = location;
    m_UpdateUniformFunc = updateFunc;
    m_pTargetProperty = pTargetProperty;
}

BBUniformUpdater::~BBUniformUpdater()
{
    BB_SAFE_DELETE(m_pTargetProperty);
}

BBUniformUpdater* BBUniformUpdater::clone()
{
    return new BBUniformUpdater(m_Location, m_UpdateUniformFunc,
                                m_pTargetProperty == nullptr ? nullptr : m_pTargetProperty->clone());
}

void BBUniformUpdater::updateUniform(GLint location, void *pCamera, void *pPropertyValue)
{
    (this->*m_UpdateUniformFunc)(location, pCamera, pPropertyValue);
}

void BBUniformUpdater::updateCameraProjectionMatrix(GLint location, void *pCamera, void *pPropertyValue)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, ((BBCamera*)pCamera)->getProjectionMatrix().data());
}

void BBUniformUpdater::updateCameraViewMatrix(GLint location, void *pCamera, void *pPropertyValue)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, ((BBCamera*)pCamera)->getViewMatrix().data());
}

void BBUniformUpdater::updateMatrix4(GLint location, void *pCamera, void *pPropertyValue)
{
    BBMatrix4MaterialProperty *pProperty = (BBMatrix4MaterialProperty*)pPropertyValue;
    glUniformMatrix4fv(location, 1, GL_FALSE, pProperty->getPropertyValue());
}

void BBUniformUpdater::updateVector4(GLint location, void *pCamera, void *pPropertyValue)
{
    BBVector4MaterialProperty *pProperty = (BBVector4MaterialProperty*)pPropertyValue;
    glUniform4fv(location, 1, pProperty->getPropertyValue());
}

void BBUniformUpdater::updateSampler2D(GLint location, void *pCamera, void *pPropertyValue)
{
    BBSampler2DMaterialProperty *pProperty = (BBSampler2DMaterialProperty*)pPropertyValue;
    glUniform1i(location, pProperty->getTextureName());
}
