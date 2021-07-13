#include "BBCoordinateSystem2D.h"
#include "BBCoordinateComponent2D.h"
#include "Geometry/BBBoundingBox2D.h"


/**
 * @brief BBCoordinateSystem2D::BBCoordinateSystem2D
 */
BBCoordinateSystem2D::BBCoordinateSystem2D()
    : BBGameObject()
{
    m_pBoundingBoxX = new BBAABBBoundingBox2D(90, 0, 53, 12);
    m_pBoundingBoxY = new BBAABBBoundingBox2D(0, 90, 12, 53);
    m_pBoundingBoxXOY = new BBAABBBoundingBox2D(18, 18, 18, 18);
    m_SelectedAxis = AxisNULL;
    m_pSelectedObject = nullptr;
    m_bTransforming = false;
}

BBCoordinateSystem2D::~BBCoordinateSystem2D()
{
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
}

void BBCoordinateSystem2D::setScreenCoordinate(int x, int y)
{
    m_pBoundingBoxX->setScreenCoordinate(x + 90, y);
    m_pBoundingBoxY->setScreenCoordinate(x, y + 90);
    m_pBoundingBoxXOY->setScreenCoordinate(x + 18, y + 18);
}

void BBCoordinateSystem2D::translate(int nDeltaX, int nDeltaY)
{
    m_pBoundingBoxX->translate(nDeltaX, nDeltaY);
    m_pBoundingBoxY->translate(nDeltaX, nDeltaY);
    m_pBoundingBoxXOY->translate(nDeltaX, nDeltaY);
}

void BBCoordinateSystem2D::setScale(float scale, bool bUpdateLocalTransform)
{
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
}

bool BBCoordinateSystem2D::hitAxis(int x, int y)
{
    BBAxisFlags axis = AxisNULL;
    bool bRet = false;
    if (m_pBoundingBoxX->hit(x, y))
    {
        axis |= AxisX;
        bRet = true;
    }
    if (m_pBoundingBoxY->hit(x, y))
    {
        axis |= AxisY;
        bRet = true;
    }
    setSelectedAxis(axis);
    return bRet;
}

bool BBCoordinateSystem2D::hitFace(int x, int y)
{
    bool bRet = m_pBoundingBoxXOY->hit(x, y);
    if (bRet)
    {
        setSelectedAxis(AxisX | AxisY);
    }
    return bRet;
}

void BBCoordinateSystem2D::setSelectedObject(BBGameObject *pObject)
{
    m_pSelectedObject = pObject;
    if (pObject != nullptr)
    {
        setScreenCoordinate(pObject->getScreenX(), pObject->getScreenY());
        setRotation(pObject->getRotation());
    }
    else
    {
        setScreenCoordinate(0, 0);
        setRotation(QVector3D(0, 0, 0));
        setSelectedAxis(BBAxisName::AxisNULL);
    }
}

void BBCoordinateSystem2D::setSelectedAxis(const BBAxisFlags &axis)
{
    m_SelectedAxis = axis;
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
    QPoint pos;
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

void BBPositionCoordinateSystem2D::setScreenCoordinate(int x, int y)
{
    BBCoordinateSystem2D::setScreenCoordinate(x, y);
    m_pCoordinateComponent->setScreenCoordinate(x, y);
}

void BBPositionCoordinateSystem2D::translate(int nDeltaX, int nDeltaY)
{
    BBCoordinateSystem2D::translate(nDeltaX, nDeltaY);
    m_pCoordinateComponent->translate(nDeltaX, nDeltaY);
}

void BBPositionCoordinateSystem2D::setSelectedAxis(const BBAxisFlags &axis)
{
    BBCoordinateSystem2D::setSelectedAxis(axis);
    m_pCoordinateComponent->setSelectedAxis(axis);
}

void BBPositionCoordinateSystem2D::transform(int x, int y)
{
    BB_PROCESS_ERROR_RETURN(m_bTransforming);

    // displacement can be computed
    if (!m_LastMousePos.isNull())
    {
        if (m_SelectedAxis & BBAxisName::AxisX)
        {
            translate(x - m_LastMousePos.x(), 0);
        }
        if (m_SelectedAxis & BBAxisName::AxisY)
        {
            translate(0, y - m_LastMousePos.y());
        }
        m_pSelectedObject->setScreenCoordinate(m_pCoordinateComponent->getScreenX(), m_pCoordinateComponent->getScreenY());
    }

    // record and wait the next frame
    m_LastMousePos = QPoint(x, y);
}
