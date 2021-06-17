#ifndef BBMATERIALPROPERTY_H
#define BBMATERIALPROPERTY_H


enum BBMaterialUniformPropertyType
{
    CameraProjectionMatrix,
    CameraViewMatrix,
    Matrix4,
    Count
};


class BBMaterialProperty
{
public:
    BBMaterialProperty(const BBMaterialUniformPropertyType &eType);
    virtual ~BBMaterialProperty();

private:
    BBMaterialUniformPropertyType m_eType;
};


class BBMatrix4MaterialProperty : public BBMaterialProperty
{
public:
    BBMatrix4MaterialProperty(const BBMaterialUniformPropertyType &eType);
    ~BBMatrix4MaterialProperty();

    inline void setPropertyValue(const float *pPropertyValue) { m_pPropertyValue = pPropertyValue; }
    inline const float* getPropertyValue() { return m_pPropertyValue; }

private:
    // 智能指针 to do
    const float *m_pPropertyValue;
};


#endif // BBMATERIALPROPERTY_H
