#include "BBRenderableObject2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBMaterial.h"


BBRenderableObject2D::BBRenderableObject2D()
    : BBRenderableObject2D(0, 0, 100, 100)
{

}

BBRenderableObject2D::BBRenderableObject2D(int nCenterX, int nCenterY, int nWidth, int nHeight)
    : BBRenderableObject()
{
    m_nHalfWidth = nWidth / 2.0f;
    m_nHalfHeight = nHeight / 2.0f;

    setPosition(QVector3D(nCenterX, nCenterY, 0.0f));
    setScale(QVector3D(m_nHalfWidth, m_nHalfHeight, 0.0f));
}

BBRenderableObject2D::~BBRenderableObject2D()
{

}

void BBRenderableObject2D::init()
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);
    BBRenderableObject::init();
}

void BBRenderableObject2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}

