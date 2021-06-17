#include "BBMaterialProperty.h"
#include "BBUtils.h"


/**
 * @brief BBMaterialProperty::BBMaterialProperty
 * @param eType
 */
BBMaterialProperty::BBMaterialProperty(const BBMaterialUniformPropertyType &eType)
{
    m_eType = eType;
}

BBMaterialProperty::~BBMaterialProperty()
{

}


/**
 * @brief BBMatrix4MaterialProperty::BBMatrix4MaterialProperty
 */
BBMatrix4MaterialProperty::BBMatrix4MaterialProperty(const BBMaterialUniformPropertyType &eType)
    : BBMaterialProperty(eType)
{
    m_pPropertyValue = nullptr;
}

BBMatrix4MaterialProperty::~BBMatrix4MaterialProperty()
{
    BB_SAFE_DELETE(m_pPropertyValue);
}


/**
 * @brief BBVector4MaterialProperty::BBVector4MaterialProperty
 * @param eType
 */
BBVector4MaterialProperty::BBVector4MaterialProperty(const BBMaterialUniformPropertyType &eType)
    : BBMaterialProperty(eType)
{
    m_pPropertyValue = nullptr;
}

BBVector4MaterialProperty::~BBVector4MaterialProperty()
{
    BB_SAFE_DELETE(m_pPropertyValue);
}
