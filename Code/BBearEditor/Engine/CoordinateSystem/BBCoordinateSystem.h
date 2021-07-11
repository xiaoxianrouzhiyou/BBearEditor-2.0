#ifndef BBCOORDINATE_H
#define BBCOORDINATE_H


#include "CoordinateSystem/BBCoordinateComponent.h"


class BBBoundingBox;
class BBRectBoundingBox2D;
class BBTriangleBoundingBox2D;
class BBQuarterCircleBoundingBox2D;
class BBBoundingBox3D;
class BBGameObjectSet;


enum BBCoordinateSystemSpaceMode
{
    Space2D = 0,
    Space3D = 1
};


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
    BBRectBoundingBox2D *m_pBoundingBoxYOZ;
    BBRectBoundingBox2D *m_pBoundingBoxXOZ;
    BBRectBoundingBox2D *m_pBoundingBoxXOY;
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
    BBQuarterCircleBoundingBox2D *m_pBoundingBoxYOZ;
    BBQuarterCircleBoundingBox2D *m_pBoundingBoxXOZ;
    BBQuarterCircleBoundingBox2D *m_pBoundingBoxXOY;
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
    BBRectBoundingBox2D *m_pBoundingBoxYOZ;
    BBRectBoundingBox2D *m_pBoundingBoxXOZ;
    BBRectBoundingBox2D *m_pBoundingBoxXOY;
    BBTriangleBoundingBox2D *m_pBoundingBoxXYZ;

    QVector3D m_SelectedObjectOriginalScale;
};


// Class that manages the three Coordinate system
class BBTransformCoordinateSystem : public BBGameObject
{
public:
    BBTransformCoordinateSystem();
    virtual ~BBTransformCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setSelectedObject(BBGameObject *pObject);
    void setSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet);
    bool mouseMoveEvent(const BBRay &ray, bool bMousePressed);
    void setCoordinateSystem(char modeKey);
    bool isTransforming() { return m_bTransforming; }
    void stopTransform();
    void update();
    inline BBGameObject* getSelectedObject() { return m_pSelectedObject; }
    inline char getTransformModeKey() { return m_ModeKey; }

private:
    void switchSpaceMode(const BBCoordinateSystemSpaceMode &eMode);

    BBCoordinateSystemSpaceMode m_eSpaceMode;

    BBPositionCoordinateSystem *m_pPositionCoordinateSystem;
    BBRotationCoordinateSystem *m_pRotationCoordinateSystem;
    BBScaleCoordinateSystem *m_pScaleCoordinateSystem;

    char m_PositionCoordinateSystemModeKey = 'W';
    char m_RotationCoordinateSystemModeKey = 'E';
    char m_ScaleCoordinateSystemModeKey = 'R';
    char m_ModeKey;

    BBGameObject *m_pSelectedObject;
    QList<BBGameObject*> m_SelectedObjects;
    bool m_bTransforming;
};


#endif // BBCOORDINATE_H
