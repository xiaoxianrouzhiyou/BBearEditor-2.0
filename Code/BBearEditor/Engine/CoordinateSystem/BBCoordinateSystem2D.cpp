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
    BB_SAFE_DELETE(m_pCoordinateComponent);
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
}

void BBCoordinateSystem2D::init()
{
    m_pCoordinateComponent->init();
}

void BBCoordinateSystem2D::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == nullptr)
        return;
    m_pCoordinateComponent->render(pCamera);
}

void BBCoordinateSystem2D::setScreenCoordinate(int x, int y)
{
    BBGameObject::setScreenCoordinate(x, y);
    m_pCoordinateComponent->setScreenCoordinate(x, y);
    m_pBoundingBoxX->setScreenCoordinate(x + 90, y);
    m_pBoundingBoxY->setScreenCoordinate(x, y + 90);
    m_pBoundingBoxXOY->setScreenCoordinate(x + 18, y + 18);
}

void BBCoordinateSystem2D::translate(int nDeltaX, int nDeltaY)
{
    m_pCoordinateComponent->translate(nDeltaX, nDeltaY);
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
    m_pCoordinateComponent->setSelectedAxis(axis);
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


/**
 * @brief BBRotationCoordinateSystem2D::BBRotationCoordinateSystem2D
 */
BBRotationCoordinateSystem2D::BBRotationCoordinateSystem2D()
    : BBCoordinateSystem2D()
{
    m_pCoordinateComponent = new BBRotationCoordinateComponent2D(0, 0);
    m_pBoundingBoxXOY = new BBQuarterCircleBoundingBox2D(0, 0, 135);
    m_pCoordinateCircle = new BBCoordinateCircle2D(0, 0, 120, 120);
    m_pCoordinateTickMark = new BBCoordinateTickMark2D(0, 0, 120, 120);
    m_pCoordinateSector = new BBCoordinateSector2D(0, 0, 120, 120);
}

BBRotationCoordinateSystem2D::~BBRotationCoordinateSystem2D()
{
    BB_SAFE_DELETE(m_pCoordinateCircle);
    BB_SAFE_DELETE(m_pCoordinateTickMark);
    BB_SAFE_DELETE(m_pCoordinateSector);
}

void BBRotationCoordinateSystem2D::init()
{
    m_pCoordinateComponent->init();
    m_pCoordinateCircle->init();
    m_pCoordinateTickMark->init();
    m_pCoordinateSector->init();
}

void BBRotationCoordinateSystem2D::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == nullptr)
        return;
    m_pCoordinateComponent->render(pCamera);
    if (m_bTransforming)
    {
        m_pCoordinateCircle->render(pCamera);
        m_pCoordinateTickMark->render(pCamera);
        m_pCoordinateSector->render(pCamera);
    }
}

void BBRotationCoordinateSystem2D::setScreenCoordinate(int x, int y)
{
    BBGameObject::setScreenCoordinate(x, y);
    m_pCoordinateComponent->setScreenCoordinate(x, y);
    m_pCoordinateCircle->setScreenCoordinate(x, y);
    m_pCoordinateTickMark->setScreenCoordinate(x, y);
    m_pCoordinateSector->setScreenCoordinate(x, y);
    m_pBoundingBoxX->setScreenCoordinate(x + 90, y);
    m_pBoundingBoxY->setScreenCoordinate(x, y + 90);
    m_pBoundingBoxXOY->setScreenCoordinate(x, y);
}

void BBRotationCoordinateSystem2D::stopTransform()
{
    BBCoordinateSystem2D::stopTransform();
    m_pCoordinateSector->reset();
}

void BBRotationCoordinateSystem2D::transform(int x, int y)
{
    BB_PROCESS_ERROR_RETURN(m_bTransforming);

    // displacement can be computed
    if (!m_LastMousePos.isNull())
    {
        if (m_SelectedAxis & (BBAxisName::AxisX | BBAxisName::AxisY))
        {
            // compute the intersection angle between two of mouse pos dir
            QVector3D mousePos(x, y, 0);
            QVector3D mousePosDir = (mousePos - m_Position).normalized();
            QVector3D lastMousePos(m_LastMousePos.x(), m_LastMousePos.y(), 0);
            QVector3D lastMousePosDir = (lastMousePos - m_Position).normalized();
            float c = QVector3D::dotProduct(lastMousePosDir, mousePosDir);
            int nDeltaAngle = round(acosf(c) / 3.141593f * 180);
            // compute the sign of angle
            QVector3D crossResult = QVector3D::crossProduct(lastMousePosDir, mousePosDir);
            float sign = crossResult.x() + crossResult.y() + crossResult.z();
            sign = sign > 0 ? 1 : -1;
            nDeltaAngle = sign * nDeltaAngle;

            m_pCoordinateSector->setAngle(nDeltaAngle);
            m_pSelectedObject->setRotation(nDeltaAngle, QVector3D(0, 0, 1));
        }
    }

    // record and wait the next frame
    m_LastMousePos = QPoint(x, y);
}


/**
 * @brief BBScaleCoordinateSystem2D::BBScaleCoordinateSystem2D
 */
BBScaleCoordinateSystem2D::BBScaleCoordinateSystem2D()
    : BBCoordinateSystem2D()
{
    m_pCoordinateComponent = new BBScaleCoordinateComponent2D(0, 0);
    m_nDeltaX = 0;
    m_nDeltaY = 0;
}

void BBScaleCoordinateSystem2D::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBCoordinateSystem2D::setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateComponent->setRotation(rotation, bUpdateLocalTransform);
}

bool BBScaleCoordinateSystem2D::mouseMoveEvent(int x, int y, bool bMousePressed)
{
    QVector3D point(x, y, 0);
    point -= m_Position;
    point = m_Quaternion.inverted() * point;
    point += m_Position;

    BBCoordinateSystem2D::mouseMoveEvent(point.x(), point.y(), bMousePressed);
}

void BBScaleCoordinateSystem2D::stopTransform()
{
    BBCoordinateSystem2D::stopTransform();
    m_nDeltaX = 0;
    m_nDeltaY = 0;
    ((BBScaleCoordinateComponent2D*)m_pCoordinateComponent)->reset();
}

void BBScaleCoordinateSystem2D::transform(int x, int y)
{
    BB_PROCESS_ERROR_RETURN(m_bTransforming);

    // displacement can be computed
    if (!m_LastMousePos.isNull())
    {
        if (m_SelectedAxis & BBAxisName::AxisX)
        {
            m_nDeltaX += x - m_LastMousePos.x();
        }
        if (m_SelectedAxis & BBAxisName::AxisY)
        {
            m_nDeltaY += y - m_LastMousePos.y();
        }
        ((BBScaleCoordinateComponent2D*)m_pCoordinateComponent)->scale(m_nDeltaX, m_nDeltaY);
    }

    // record and wait the next frame
    m_LastMousePos = QPoint(x, y);
}
