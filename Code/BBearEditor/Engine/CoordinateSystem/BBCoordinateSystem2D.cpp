#include "BBCoordinateSystem2D.h"
#include "BBCoordinateComponent2D.h"
#include "Geometry/BBBoundingBox2D.h"


/**
 * @brief BBCoordinateSystem2D::BBCoordinateSystem2D
 */
BBCoordinateSystem2D::BBCoordinateSystem2D()
    : BBGameObject()
{
    m_pBoundingBoxX = new BBAABBBoundingBox2D(0.75f, 0.0f, 0.45f, 0.1f, 120, 120);
    m_pBoundingBoxY = new BBAABBBoundingBox2D(0.0f, 0.75f, 0.1f, 0.45f, 120, 120);
    m_pBoundingBoxXOY = new BBAABBBoundingBox2D(0.15f, 0.15f, 0.15f, 0.15f, 120, 120);
    m_bTransforming = false;
}

BBCoordinateSystem2D::~BBCoordinateSystem2D()
{
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
}

void BBCoordinateSystem2D::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    m_pBoundingBoxX->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
}

void BBCoordinateSystem2D::setScale(float scale, bool bUpdateLocalTransform)
{
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
}

bool BBCoordinateSystem2D::hitAxis(int x, int y)
{
    BB_PROCESS_ERROR_RETURN_FALSE(m_pBoundingBoxX->hit(x, y));
    BB_PROCESS_ERROR_RETURN_FALSE(m_pBoundingBoxY->hit(x, y));
    return true;
}

bool BBCoordinateSystem2D::hitFace(int x, int y)
{
    return m_pBoundingBoxXOY->hit(x, y);
}

void BBCoordinateSystem2D::setSelectedObject(BBGameObject *pObject)
{

}

bool BBCoordinateSystem2D::mouseMoveEvent(int x, int y, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // if hitting m_pCoordinateRectFace, no need to handle axis
        if (!hitFace(x, y))
        {
            // handle axis
            BB_END(!hitAxis(x, y));
        }

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(x, y);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBCoordinateSystem2D::stopTransform()
{
    // When no movement, reset mouse pos
    QVector3D pos;
    m_LastMousePos = pos;
    m_bTransforming = false;
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

void BBPositionCoordinateSystem2D::transform(int x, int y)
{

}
