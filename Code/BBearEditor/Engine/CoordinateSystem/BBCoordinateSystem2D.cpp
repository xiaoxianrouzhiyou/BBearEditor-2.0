#include "BBCoordinateSystem2D.h"
#include "BBCoordinateComponent2D.h"
#include "Geometry/BBBoundingBox.h"


/**
 * @brief BBCoordinateSystem2D::BBCoordinateSystem2D
 */
BBCoordinateSystem2D::BBCoordinateSystem2D()
    : BBGameObject()
{
    m_pBoundingBoxX = new BBRectBoundingBox2D(0.75f, 0.0f, 0.0f, 0.45f, 0.1f, 0.0f);
    m_pBoundingBoxY = new BBRectBoundingBox2D(0.0f, 0.75f, 0.0f, 0.1f, 0.45f, 0.0f);
    m_pBoundingBoxXOY = new BBRectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
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

bool BBCoordinateSystem2D::hitAxis(int x, int y, float &fDistance)
{

}

bool BBCoordinateSystem2D::hitFace(int x, int y, float &fDistance)
{

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
        float fDistance;
        if (!hitFace(x, y, fDistance))
        {
            // handle axis
            BB_END(!hitAxis(x, y, fDistance));
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
