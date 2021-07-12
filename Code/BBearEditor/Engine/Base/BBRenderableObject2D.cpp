#include "BBRenderableObject2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBMaterial.h"


BBRenderableObject2D::BBRenderableObject2D()
    : BBRenderableObject2D(0, 0, 100, 100)
{

}

BBRenderableObject2D::BBRenderableObject2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject()
{
    setSize(nWidth, nHeight);

    setPosition(QVector3D(x, y, 0.0f));
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

void BBRenderableObject2D::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    m_nScreenX = position.x() * m_nWidth;
    m_nScreenY = position.y() * m_nHeight;
    BBRenderableObject::setPosition(position, bUpdateLocalTransform);
}

void BBRenderableObject2D::setSize(int nWidth, int nHeight)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    setScale(QVector3D(m_nWidth, m_nHeight, 0));
}

bool BBRenderableObject2D::hit(int x, int y)
{
    return true;
}
