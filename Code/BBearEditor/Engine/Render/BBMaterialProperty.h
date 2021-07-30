#ifndef BBMATERIALPROPERTY_H
#define BBMATERIALPROPERTY_H


#include "BBBaseRenderComponent.h"

enum BBMaterialUniformPropertyType
{
    CameraProjectionMatrix,
    CameraInverseProjectionMatrix,
    CameraViewMatrix,
    CameraInverseViewMatrix,
    LightProjectionMatrix,
    LightViewMatrix,
    Float,
    Matrix4,
    Vector4,
    Sampler2D,
    Count
};


enum BBVector4MaterialPropertyFactoryType
{
    Default,
    Color
};


class BBMaterialProperty
{
public:
    BBMaterialProperty(const BBMaterialUniformPropertyType &eType, const char *name);
    virtual ~BBMaterialProperty();

    virtual BBMaterialProperty* clone() = 0;
    BBMaterialUniformPropertyType getType() { return m_eType; }
    inline char* getName() { return m_Name; }
    inline QString getNameInPropertyManager() { return m_NameInPropertyManager; }

protected:
    BBMaterialUniformPropertyType m_eType;
    char m_Name[64];
    QString m_NameInPropertyManager;
};


class BBFloatMaterialProperty : public BBMaterialProperty
{
public:
    BBFloatMaterialProperty(const char *name);
    ~BBFloatMaterialProperty();

    BBMaterialProperty* clone() override;

    inline void setPropertyValue(const float fPropertyValue) { m_fPropertyValue = fPropertyValue; }
    inline const float getPropertyValue() { return m_fPropertyValue; }

private:
    float m_fPropertyValue;
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
    BBVector4MaterialPropertyFactoryType getFactoryType() { return m_eFactoryType; }

private:
    // 智能指针 to do
    const float *m_pPropertyValue;
    BBVector4MaterialPropertyFactoryType m_eFactoryType;
};


class BBSampler2DMaterialProperty : public BBMaterialProperty
{
public:
    BBSampler2DMaterialProperty(const char *name, int nSlotIndex);
    ~BBSampler2DMaterialProperty();

    BBMaterialProperty* clone() override;

    void setTextureName(GLuint textureName, const QString &resourcePath = "");
    inline GLuint getTextureName() const { return m_TextureName; }
    inline int getSlotIndex() { return m_nSlotIndex; }
    inline QString getResourcePath() { return m_ResourcePath; }

private:
    // 智能指针 to do
    GLuint m_TextureName;
    int m_nSlotIndex;
    QString m_ResourcePath;
};


#endif // BBMATERIALPROPERTY_H
