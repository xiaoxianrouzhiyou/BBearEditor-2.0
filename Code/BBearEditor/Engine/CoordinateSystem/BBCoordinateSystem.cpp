#include "BBCoordinateSystem.h"
#include "Utils/BBUtils.h"
#include "Render/BBCamera.h"
#include "Geometry/BBBoundingBox.h"


/**
 * @brief BBCoordinateSystem::BBCoordinateSystem
 */
BBCoordinateSystem::BBCoordinateSystem()
    : BBGameObject()
{
    m_pSelectedObject = NULL;
    m_bTransforming = false;
}

BBCoordinateSystem::~BBCoordinateSystem()
{
    BB_SAFE_DELETE(m_pSelectedObject);
}

void BBCoordinateSystem::render(BBCamera *pCamera)
{
    // The coordinate system will not become smaller as the camera is zoomed out
    // need to scale according to the distance
    float fDistance = (pCamera->getPosition() - m_Position).length();
    setScale(fDistance / 6.5f);
}

bool BBCoordinateSystem::hit(const BBRay &ray,
                             BBBoundingBox *pBoundingBox1, const BBAxisFlags &axis1,
                             BBBoundingBox *pBoundingBox2, const BBAxisFlags &axis2,
                             BBBoundingBox *pBoundingBox3, const BBAxisFlags &axis3,
                             float &fDistance)
{
    // axis1 is the coordinate axis name corresponding to pBoundingBox1
    // size of m_pCoordinateRectFace is 0.3
    float d1, d2, d3;
    bool bResult1 = pBoundingBox1->hit(ray, d1);
    bool bResult2 = pBoundingBox2->hit(ray, d2);
    bool bResult3 = pBoundingBox3->hit(ray, d3);
    if (!bResult1 && !bResult2 && !bResult3)
    {
        setSelectedAxis(BBAxisName::AxisNULL);
        return false;
    }

    if (d1 < d2)
    {
        if (d1 < d3)
        {
            // yoz is nearest
            setSelectedAxis(axis1);
            fDistance = d1;
        }
        else
        {
            // xoy is nearest
            setSelectedAxis(axis3);
            fDistance = d3;
        }
    }
    else
    {
        if (d1 < d3)
        {
            // xoz is nearest
            setSelectedAxis(axis2);
            fDistance = d2;
        }
        else
        {
            if (d2 < d3)
            {
                // xoz is nearest
                setSelectedAxis(axis2);
                fDistance = d2;
            }
            else
            {
                // xoy is nearest
                setSelectedAxis(axis3);
                fDistance = d3;
            }
        }
    }

    return true;
}

void BBCoordinateSystem::setSelectedObject(BBGameObject *pObject)
{
    m_pSelectedObject = pObject;
    if (pObject != NULL)
    {
        setPosition(pObject->getPosition());
        setRotation(pObject->getRotation());
    }
    else
    {
        setPosition(QVector3D(0, 0, 0));
        setRotation(QVector3D(0, 0, 0));
        setSelectedAxis(BBAxisName::AxisNULL);
    }
}

void BBCoordinateSystem::stopTransform()
{
    // When no movement, reset mouse pos
    QVector3D pos;
    m_LastMousePos = pos;
    m_bTransforming = false;
}


/**
 * @brief BBPositionCoordinateSystem::BBPositionCoordinateSystem
 */
BBPositionCoordinateSystem::BBPositionCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateArrow = new BBCoordinateArrow();
    m_pCoordinateAxis = new BBCoordinateAxis();
    m_pCoordinateRectFace = new BBCoordinateRectFace();
    m_pBoundingBoxX = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.75f, 0.0f, 0.0f, 0.45f, 0.1f, 0.1f);
    m_pBoundingBoxY = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.75f, 0.0f, 0.1f, 0.45f, 0.1f);
    m_pBoundingBoxZ = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.0f, 0.75f, 0.1f, 0.1f, 0.45f);
    m_pBoundingBoxYOZ = new BBRectBoundingBox2D(0.0f, 0.15f, 0.15f, 0.0f, 0.15f, 0.15f);
    m_pBoundingBoxXOZ = new BBRectBoundingBox2D(0.15f, 0.0f, 0.15f, 0.15f, 0.0f, 0.15f);
    m_pBoundingBoxXOY = new BBRectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
}

BBPositionCoordinateSystem::~BBPositionCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateArrow);
    BB_SAFE_DELETE(m_pCoordinateAxis);
    BB_SAFE_DELETE(m_pCoordinateRectFace);
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxZ);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
}

void BBPositionCoordinateSystem::init()
{
    m_pCoordinateArrow->init();
    m_pCoordinateAxis->init();
    m_pCoordinateRectFace->init();
}

void BBPositionCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);
    m_pCoordinateArrow->render(pCamera);
    m_pCoordinateAxis->render(pCamera);
    m_pCoordinateRectFace->render(pCamera);
}

void BBPositionCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    // Modifying m_Position directly will cause an error
    // The position of the bounding box needs to be updated at the same time
    // otherwise, will trigger incorrect mouse events
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateArrow->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateAxis->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateRectFace->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxX->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
}

void BBPositionCoordinateSystem::setScale(float scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);
    m_pCoordinateArrow->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateAxis->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateRectFace->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
}

void BBPositionCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    // change color of indicator
    m_pCoordinateArrow->setSelectedAxis(axis);
    m_pCoordinateAxis->setSelectedAxis(axis);

    m_SelectedAxis = axis;
}

bool BBPositionCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // if hitting m_pCoordinateRectFace, no need to handle axis
        float fDistance;
        if (!hit(ray,
                 m_pBoundingBoxYOZ, BBAxisName::AxisY | BBAxisName::AxisZ,
                 m_pBoundingBoxXOZ, BBAxisName::AxisX | BBAxisName::AxisZ,
                 m_pBoundingBoxXOY, BBAxisName::AxisX | BBAxisName::AxisY,
                 fDistance))
        {
            // handle axis
            BB_END(!hit(ray,
                        m_pBoundingBoxX, BBAxisName::AxisX,
                        m_pBoundingBoxY, BBAxisName::AxisY,
                        m_pBoundingBoxZ, BBAxisName::AxisZ,
                        fDistance));
        }

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBPositionCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        if ((m_SelectedAxis == BBAxisName::AxisY)
         || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY)))
        {
            mousePos = ray.computeIntersectWithXOYPlane(m_Position.z());
        }
        else if ((m_SelectedAxis == BBAxisName::AxisX
               || m_SelectedAxis == BBAxisName::AxisZ)
              || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ)))
        {
            mousePos = ray.computeIntersectWithXOZPlane(m_Position.y());
        }
        else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            mousePos = ray.computeIntersectWithYOZPlane(m_Position.x());
        }

        // displacement can be computed
        if (!m_LastMousePos.isNull())
        {
            QVector3D mouseDisplacement = mousePos - m_LastMousePos;
            if (m_SelectedAxis & BBAxisName::AxisX)
            {
                // The length of the projection of the mouse's displacement on the axis
                float d = mouseDisplacement.x();
                setPosition(m_Position + QVector3D(d, 0, 0));
            }
            if (m_SelectedAxis & BBAxisName::AxisY)
            {
                float d = mouseDisplacement.y();
                setPosition(m_Position + QVector3D(0, d, 0));
            }
            if (m_SelectedAxis & BBAxisName::AxisZ)
            {
                float d = mouseDisplacement.z();
                setPosition(m_Position + QVector3D(0, 0, d));
            }
            m_pSelectedObject->setPosition(m_Position);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
    }
}


/**
 * @brief BBRotationCoordinateSystem::BBRotationCoordinateSystem
 */
BBRotationCoordinateSystem::BBRotationCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateQuarterCircle = new BBCoordinateQuarterCircle();
    m_pBoundingBoxYOZ = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::YOZ);
    m_pBoundingBoxXOZ = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::XOZ);
    m_pBoundingBoxXOY = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::XOY);
    m_pCoordinateCircle = new BBCoordinateCircle();
    m_pCoordinateTickMark = new BBCoordinateTickMark();
    m_pCoordinateSector = new BBCoordinateSector();
}

BBRotationCoordinateSystem::~BBRotationCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateQuarterCircle);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
    BB_SAFE_DELETE(m_pCoordinateCircle);
    BB_SAFE_DELETE(m_pCoordinateTickMark);
    BB_SAFE_DELETE(m_pCoordinateSector);
}

void BBRotationCoordinateSystem::init()
{
    m_pCoordinateQuarterCircle->init();
    m_pCoordinateCircle->init();
    m_pCoordinateTickMark->init();
    m_pCoordinateSector->init();
}

void BBRotationCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);

    // When transforming, render circle. otherwise, render coordinate axis
    if (m_bTransforming)
    {
        m_pCoordinateCircle->render(pCamera);
        m_pCoordinateTickMark->render(pCamera);
        m_pCoordinateSector->render(pCamera);
    }
    else
    {
        // coordinate axis always faces camera
        QVector3D dir = pCamera->getPosition() - m_Position;
        dir.setX(dir.x() >= 0 ? 1 : -1);
        dir.setY(dir.y() >= 0 ? 1 : -1);
        dir.setZ(dir.z() >= 0 ? 1 : -1);
        m_pCoordinateQuarterCircle->setScale(m_pCoordinateQuarterCircle->getScale() * dir);
        m_pCoordinateQuarterCircle->render(pCamera);
        // bounding box also needs mirror transform
        m_pBoundingBoxYOZ->setQuadrantFlag(dir);
        m_pBoundingBoxXOZ->setQuadrantFlag(dir);
        m_pBoundingBoxXOY->setQuadrantFlag(dir);
    }
}

void BBRotationCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateQuarterCircle->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateCircle->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateTickMark->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateSector->setPosition(position, bUpdateLocalTransform);
}

void BBRotationCoordinateSystem::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);
    m_pCoordinateQuarterCircle->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateCircle->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateTickMark->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateSector->setScale(scale, bUpdateLocalTransform);
}

void BBRotationCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    m_pCoordinateQuarterCircle->setSelectedAxis(axis);

    // m_pCoordinateCircle is based on face YOZ and needs to rotate
    QVector3D rot;
    if (m_SelectedAxis == BBAxisName::AxisY)
    {

        rot = QVector3D(0, -90, 90);
    }
    else if (m_SelectedAxis == BBAxisName::AxisZ)
    {
        rot = QVector3D(0, -90, 0);
    }
    m_pCoordinateCircle->setRotation(rot);
    m_pCoordinateTickMark->setRotation(rot);
    m_pCoordinateSector->setRotation(rot);

    m_SelectedAxis = axis;
}

bool BBRotationCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // hit face YOZ, rotate around AxisX
        float fDistance;
        BB_END(!hit(ray,
                    m_pBoundingBoxYOZ, BBAxisName::AxisX,
                    m_pBoundingBoxXOZ, BBAxisName::AxisY,
                    m_pBoundingBoxXOY, BBAxisName::AxisZ,
                    fDistance));

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBRotationCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        QVector3D rotAxis;

        if (m_SelectedAxis == BBAxisName::AxisX)
        {
            mousePos = ray.computeIntersectWithYOZPlane(m_Position.x());
            rotAxis = QVector3D(1, 0, 0);
        }
        else if (m_SelectedAxis == BBAxisName::AxisY)
        {
            mousePos = ray.computeIntersectWithXOZPlane(m_Position.y());
            rotAxis = QVector3D(0, 1, 0);
        }
        else if (m_SelectedAxis == BBAxisName::AxisZ)
        {
            mousePos = ray.computeIntersectWithXOYPlane(m_Position.z());
            rotAxis = QVector3D(0, 0, 1);
        }
        else
        {
            return;
        }

        // whether displacement can be computed
        if (m_LastMousePos.isNull())
        {
            // init
            m_pCoordinateSector->reset();
        }
        else
        {
            // compute the intersection angle between two of mouse pos dir
            QVector3D mousePosDir = (mousePos - m_Position).normalized();
            float c = QVector3D::dotProduct(m_LastMousePosDir, mousePosDir);
            int nDeltaAngle = round(acosf(c) / 3.141593f * 180);
            // compute the sign of angle
            QVector3D crossResult = QVector3D::crossProduct(m_LastMousePosDir, mousePosDir);
            float sign = crossResult.x() + crossResult.y() + crossResult.z();
            sign = sign > 0 ? 1 : -1;
            nDeltaAngle = sign * nDeltaAngle;

            m_pCoordinateSector->setAngle(nDeltaAngle);

            m_pSelectedObject->setRotation(nDeltaAngle, rotAxis);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
        m_LastMousePosDir = (m_LastMousePos - m_Position).normalized();
    }
}


/**
 * @brief BBScaleCoordinateSystem::BBScaleCoordinateSystem
 */
BBScaleCoordinateSystem::BBScaleCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateCube = new BBCoordinateCube();
    m_pCoordinateAxis = new BBCoordinateAxis();
    m_pCoordinateTriangleFace = new BBCoordinateTriangleFace();
    m_pBoundingBoxX = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.69f, 0.0f, 0.0f, 0.39f, 0.08f, 0.08f);
    m_pBoundingBoxY = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.69f, 0.0f, 0.08f, 0.39f, 0.08f);
    m_pBoundingBoxZ = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.0f, 0.69f, 0.08f, 0.08f, 0.39f);
    m_pBoundingBoxYOZ = new BBRectBoundingBox2D(0.0f, 0.15f, 0.15f, 0.0f, 0.15f, 0.15f);
    m_pBoundingBoxXOZ = new BBRectBoundingBox2D(0.15f, 0.0f, 0.15f, 0.15f, 0.0f, 0.15f);
    m_pBoundingBoxXOY = new BBRectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
    m_pBoundingBoxXYZ = new BBTriangleBoundingBox2D(QVector3D(0.3f, 0.0f, 0.0f),
                                                    QVector3D(0.0f, 0.3f, 0.0f),
                                                    QVector3D(0.0f, 0.0f, 0.3f));
}

BBScaleCoordinateSystem::~BBScaleCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateCube);
    BB_SAFE_DELETE(m_pCoordinateAxis);
    BB_SAFE_DELETE(m_pCoordinateTriangleFace);
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxZ);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
    BB_SAFE_DELETE(m_pBoundingBoxXYZ);
}

void BBScaleCoordinateSystem::init()
{
    m_pCoordinateCube->init();
    m_pCoordinateAxis->init();
    m_pCoordinateTriangleFace->init();
}

void BBScaleCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);
    m_pCoordinateCube->render(pCamera);
    m_pCoordinateAxis->render(pCamera);
    m_pCoordinateTriangleFace->render(pCamera);
}

void BBScaleCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateCube->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateAxis->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxX->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setPosition(position, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateCube->setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateAxis->setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxX->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxY->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setRotation(rotation, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);

    // when transforming, cube and axis will change as scale operation
    if (m_bTransforming)
    {
        QVector3D delta = m_pSelectedObject->getScale() - m_SelectedObjectOriginalScale;
        m_pCoordinateCube->move(delta);
        // the normalized scale is not 0 but 1
        m_pCoordinateAxis->setScale(scale * (delta + QVector3D(1, 1, 1)), bUpdateLocalTransform);
    }
    else
    {
        // after stopTransform, go back
        m_pCoordinateCube->move(QVector3D(0, 0, 0));
        m_pCoordinateAxis->setScale(scale, bUpdateLocalTransform);
    }
    m_pCoordinateCube->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setScale(scale, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    // change color of indicator
    m_pCoordinateCube->setSelectedAxis(axis);
    m_pCoordinateAxis->setSelectedAxis(axis);

    m_SelectedAxis = axis;
}

bool BBScaleCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // if hitting m_pCoordinateRectFace, no need to handle axis
        float fDistance;
        if (!hit(ray,
                 m_pBoundingBoxYOZ, BBAxisName::AxisY | BBAxisName::AxisZ,
                 m_pBoundingBoxXOZ, BBAxisName::AxisX | BBAxisName::AxisZ,
                 m_pBoundingBoxXOY, BBAxisName::AxisX | BBAxisName::AxisY,
                 fDistance))
        {
            // handle axis
            BB_END(!hit(ray,
                        m_pBoundingBoxX, BBAxisName::AxisX,
                        m_pBoundingBoxY, BBAxisName::AxisY,
                        m_pBoundingBoxZ, BBAxisName::AxisZ,
                        fDistance));
        }
        else
        {
            // Two axes are selected, determine whether the third axis is also selected
            // and perform collision detection on the triangle in the middle
            float d;
            if (m_pBoundingBoxXYZ->hit(ray, d))
            {
                if (d <= fDistance)
                {
                    // The middle triangle collision point is closer, select the three axes
                    setSelectedAxis(BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ);
                }
            }
        }

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBScaleCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        QMatrix4x4 rotMatrix;
        rotMatrix.rotate(m_Quaternion);
        if ((m_SelectedAxis == BBAxisName::AxisY)
         || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY)))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(0, 0, 1), mousePos);
        }
        else if ((m_SelectedAxis == BBAxisName::AxisX || m_SelectedAxis == BBAxisName::AxisZ)
              || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ)))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(0, 1, 0), mousePos);
        }
        else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(1, 0, 0), mousePos);
        }
        else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(1, 1, 1), mousePos);
        }

        // whether displacement can be computed
        if (m_LastMousePos.isNull())
        {
            // init for the scale of cube and axis
            m_SelectedObjectOriginalScale = m_pSelectedObject->getScale();
        }
        else
        {
            QVector3D mouseDisplacement = mousePos - m_LastMousePos;
            QVector3D scale = m_pSelectedObject->getScale();
            QVector3D dir;
            if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ))
            {
                dir = QVector3D(1, -1, 1).normalized();
                scale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * QVector3D(1, 1, 1).normalized();
            }
            else
            {
                if (m_SelectedAxis == BBAxisName::AxisX)
                {
                    // The length of the projection of the mouse's displacement on the axis
                    dir = QVector3D(1, 0, 0).normalized();
                }
                else if (m_SelectedAxis == BBAxisName::AxisY)
                {
                    dir = QVector3D(0, 1, 0).normalized();
                }
                else if (m_SelectedAxis == BBAxisName::AxisZ)
                {
                    dir = QVector3D(0, 0, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
                {
                    dir = QVector3D(0, 1, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ))
                {
                    dir = QVector3D(1, 0, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY))
                {
                    dir = QVector3D(1, 1, 0).normalized();
                }
                scale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
            }
            m_pSelectedObject->setScale(scale);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
    }
}
