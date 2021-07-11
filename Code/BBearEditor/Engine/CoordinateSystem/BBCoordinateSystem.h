#ifndef BBCOORDINATE_H
#define BBCOORDINATE_H


#include "CoordinateSystem/BBCoordinateComponent.h"


class BBBoundingBox;
class BBRectBoundingBox3D;
class BBTriangleBoundingBox3D;
class BBQuarterCircleBoundingBox3D;
class BBBoundingBox3D;


// base class of pos rot scale 3 Coordinate System
class BBCoordinateSystem : public BBGameObject
{
protected:
    BBCoordinateSystem();
    virtual ~BBCoordinateSystem();

    void render(BBCamera *pCamera) override;

    bool hit(const BBRay &ray,
             BBBoundingBox *pBoundingBox1, const BBAxisFlags &axis1,
             BBBoundingBox *pBoundingBox2, const BBAxisFlags &axis2,
             BBBoundingBox *pBoundingBox3, const BBAxisFlags &axis3,
             float &fDistance);
    virtual void transform(const BBRay &ray) = 0;

    BBAxisFlags m_SelectedAxis;
    QVector3D m_LastMousePos;
    BBGameObject *m_pSelectedObject;
    bool m_bTransforming;

public:
    void setSelectedObject(BBGameObject *pObject);
    virtual void setSelectedAxis(const BBAxisFlags &axis) = 0;
    virtual bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) = 0;
    void stopTransform();
};


class BBPositionCoordinateSystem : public BBCoordinateSystem
{
public:
    BBPositionCoordinateSystem();
    virtual ~BBPositionCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setScale(float scale, bool bUpdateLocalTransform = true) override;

    void setSelectedAxis(const BBAxisFlags &axis) override;
    bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) override;

private:
    void transform(const BBRay &ray) override;

    BBCoordinateArrow *m_pCoordinateArrow;
    BBCoordinateAxis *m_pCoordinateAxis;
    BBCoordinateRectFace *m_pCoordinateRectFace;

    BBBoundingBox3D *m_pBoundingBoxX;
    BBBoundingBox3D *m_pBoundingBoxY;
    BBBoundingBox3D *m_pBoundingBoxZ;
    BBRectBoundingBox3D *m_pBoundingBoxYOZ;
    BBRectBoundingBox3D *m_pBoundingBoxXOZ;
    BBRectBoundingBox3D *m_pBoundingBoxXOY;
};


class BBRotationCoordinateSystem : public BBCoordinateSystem
{
public:
    BBRotationCoordinateSystem();
    ~BBRotationCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setSelectedAxis(const BBAxisFlags &axis) override;
    bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) override;

private:
    void transform(const BBRay &ray) override;

    BBCoordinateQuarterCircle *m_pCoordinateQuarterCircle;
    BBQuarterCircleBoundingBox3D *m_pBoundingBoxYOZ;
    BBQuarterCircleBoundingBox3D *m_pBoundingBoxXOZ;
    BBQuarterCircleBoundingBox3D *m_pBoundingBoxXOY;
    // when transforming, appear
    BBCoordinateCircle *m_pCoordinateCircle;
    BBCoordinateTickMark *m_pCoordinateTickMark;
    BBCoordinateSector *m_pCoordinateSector;

    QVector3D m_LastMousePosDir;
};


class BBScaleCoordinateSystem : public BBCoordinateSystem
{
public:
    BBScaleCoordinateSystem();
    ~BBScaleCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setSelectedAxis(const BBAxisFlags &axis) override;
    bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) override;

private:
    void transform(const BBRay &ray) override;

    BBCoordinateCube *m_pCoordinateCube;
    BBCoordinateAxis *m_pCoordinateAxis;
    BBCoordinateTriangleFace *m_pCoordinateTriangleFace;

    BBBoundingBox3D *m_pBoundingBoxX;
    BBBoundingBox3D *m_pBoundingBoxY;
    BBBoundingBox3D *m_pBoundingBoxZ;
    BBRectBoundingBox3D *m_pBoundingBoxYOZ;
    BBRectBoundingBox3D *m_pBoundingBoxXOZ;
    BBRectBoundingBox3D *m_pBoundingBoxXOY;
    BBTriangleBoundingBox3D *m_pBoundingBoxXYZ;

    QVector3D m_SelectedObjectOriginalScale;
};


#endif // BBCOORDINATE_H
