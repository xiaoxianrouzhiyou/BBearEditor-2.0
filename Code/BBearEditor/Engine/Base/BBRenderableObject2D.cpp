#include "BBRenderableObject2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBMaterial.h"
#include "IO/BBMaterialFileManager.h"


BBRenderableObject2D::BBRenderableObject2D()
    : BBRenderableObject2D(0, 0, 100, 100)
{

}

BBRenderableObject2D::BBRenderableObject2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject(x, y, nWidth, nHeight)
{

}

BBRenderableObject2D::~BBRenderableObject2D()
{

}

void BBRenderableObject2D::init()
{
    m_pCurrentMaterial = BBMaterialFileManager::getUIMaterial();
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0, 0.0f, 0.0f);
    BBRenderableObject::init();
}

void BBRenderableObject2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}

bool BBRenderableObject2D::hit(int x, int y)
{
    return true;
}
