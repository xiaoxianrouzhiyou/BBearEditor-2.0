#ifndef BBMATERIALPROPERTY_H
#define BBMATERIALPROPERTY_H


#include "BBBaseRenderComponent.h"

enum BBMaterialUniformPropertyType
{
    CameraProjectionMatrix,
    CameraViewMatrix,
    Matrix4,
    Vector4,
    Sampler2D,
    Count
};


class BBMaterialProperty
{
public:
    BBMaterialProperty(const BBMaterialUniformPropertyType &eType, const char *name);
    virtual ~BBMaterialProperty();

    virtual BBMaterialProperty* clone() = 0;
    inline char* getName() { return m_Name; }

protected:
    BBMaterialUniformPropertyType m_eType;
    char m_Name[64];
};


class BBMatrix4MaterialProperty : public BBMaterialProperty
{
public:
    BBMatrix4MaterialProperty(const char *name);
    ~BBMatrix4MaterialProperty();

    BBMaterialProperty* clone() override;

    inline void setPropertyValue(const float *pPropertyValue) { m_pPropertyValue = pPropertyValue; }
    inline const float* getPropertyValue() { return m_pPropertyValue; }

private:
    // 智能指针 to do
    const float *m_pPropertyValue;
};


class BBVector4MaterialProperty : public BBMaterialProperty
{
public:
    BBVector4MaterialProperty(const char *name);
    ~BBVector4MaterialProperty();

    BBMaterialProperty* clone() override;

    inline void setPropertyValue(const float *pPropertyValue) { m_pPropertyValue = pPropertyValue; }
    inline const float* getPropertyValue() { return m_pPropertyValue; }

private:
    // 智能指针 to do
    const float *m_pPropertyValue;
};


class BBSampler2DMaterialProperty : public BBMaterialProperty
{
public:
    BBSampler2DMaterialProperty(const char *name);
    ~BBSampler2DMaterialProperty();

    BBMaterialProperty* clone() override;

    inline void setTextureName(GLuint textureName) { m_TextureName = textureName; }
    inline GLuint getTextureName() const { return m_TextureName; }

private:
    // 智能指针 to do
    GLuint m_TextureName;
};


#endif // BBMATERIALPROPERTY_H
