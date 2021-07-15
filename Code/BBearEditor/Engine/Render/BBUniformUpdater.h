#ifndef BBUNIFORMUPDATER_H
#define BBUNIFORMUPDATER_H


#include "BBBaseRenderComponent.h"
#include "BBLinkedList.h"
#include "BBMaterialProperty.h"

/* must declare the class in advance */
class BBUniformUpdater;

typedef void (BBUniformUpdater::*BBUpdateUniformFunc)(GLint location, void *ptr, void *pPropertyValue);

class BBUniformUpdater : public BBBaseRenderComponent, public BBLinkedList
{
public:
    BBUniformUpdater(GLint location, const BBUpdateUniformFunc &updateFunc, BBMaterialProperty *pTargetProperty);
    ~BBUniformUpdater();

    inline GLint getLocation() { return m_Location; }
    inline BBUpdateUniformFunc getUpdateUniformFunc() { return m_UpdateUniformFunc; }
    inline BBMaterialProperty* getTargetProperty() { return m_pTargetProperty; }

    BBUniformUpdater* clone();

    void updateUniform(GLint location, void *pCamera, void *pPropertyValue);
    void updateCameraProjectionMatrix(GLint location, void *pCamera, void *pPropertyValue);
    void updateCameraViewMatrix(GLint location, void *pCamera, void *pPropertyValue);
    void updateCanvas(GLint location, void *pCanvas, void *pPropertyValue);
    void updateMatrix4(GLint location, void *pCamera, void *pPropertyValue);
    void updateVector4(GLint location, void *pCamera, void *pPropertyValue);
    void updateSampler2D(GLint location, void *pCamera, void *pPropertyValue);

private:
    GLint m_Location;
    BBUpdateUniformFunc m_UpdateUniformFunc;
    BBMaterialProperty *m_pTargetProperty;
};

#endif // BBUNIFORMUPDATER_H
