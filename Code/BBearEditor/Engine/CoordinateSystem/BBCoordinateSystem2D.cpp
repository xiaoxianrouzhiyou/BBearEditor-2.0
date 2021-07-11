#include "BBCoordinateSystem2D.h"
#include "BBCoordinateComponent2D.h"


/**
 * @brief BBCoordinateSystem2D::BBCoordinateSystem2D
 */
BBCoordinateSystem2D::BBCoordinateSystem2D()
    : BBGameObject()
{

}

void BBCoordinateSystem2D::setSelectedObject(BBGameObject *pObject)
{

}

void BBCoordinateSystem2D::stopTransform()
{

}


/**
 * @brief BBPositionCoordinateSystem2D::BBPositionCoordinateSystem2D
 */
BBPositionCoordinateSystem2D::BBPositionCoordinateSystem2D()
    : BBCoordinateSystem2D()
{
    m_pCoordinateComponent = new BBPositionCoordinateComponent2D(0, 0);
}

BBPositionCoordinateSystem2D::~BBPositionCoordinateSystem2D()
{
    BB_SAFE_DELETE(m_pCoordinateComponent);
}

void BBPositionCoordinateSystem2D::init()
{
    m_pCoordinateComponent->init();
}

void BBPositionCoordinateSystem2D::render(BBCamera *pCamera)
{
    m_pCoordinateComponent->render(pCamera);
}

void BBPositionCoordinateSystem2D::resize(float fWidth, float fHeight)
{
    m_pCoordinateComponent->resize(fWidth, fHeight);
}

void BBPositionCoordinateSystem2D::setSelectedAxis(const BBAxisFlags &axis)
{

}

bool BBPositionCoordinateSystem2D::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{

}

void BBPositionCoordinateSystem2D::transform(const BBRay &ray)
{

}
