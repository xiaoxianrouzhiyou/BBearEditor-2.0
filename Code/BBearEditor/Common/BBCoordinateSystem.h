#ifndef BBCOORDINATE_H
#define BBCOORDINATE_H


#include "BBRenderableObject.h"


enum BBAxisName
{
    AxisNULL = 0x01,
    AxisX = 0x02,
    AxisY = 0x04,
    AxisZ = 0x08
};


// Type of bit operation result
Q_DECLARE_FLAGS(BBAxisFlags, BBAxisName)
// override | operator
Q_DECLARE_OPERATORS_FOR_FLAGS(BBAxisFlags)


class BBBoundingBox;
class BBRectBoundingBox2D;
class BBTriangleBoundingBox2D;
class BBQuarterCircleBoundingBox2D;
class BBBoundingBox3D;
class BBGameObjectSet;


// The base class of each component to be rendered in the coordinate system
class BBCoordinateComponent : public BBRenderableObject
{
protected:
    BBCoordinateComponent();
    BBCoordinateComponent(float px, float py, float pz,
                          float rx, float ry, float rz,
                          float sx, float sy, float sz);

    BBAxisFlags m_SelectedAxis;
    static QVector3D m_Red;
    static QVector4D m_RedTransparency;
    static QVector3D m_Green;
    static QVector4D m_GreenTransparency;
    static QVector3D m_Blue;
    static QVector4D m_BlueTransparency;
    static QVector3D m_Yellow;
    static QVector3D m_Gray;
    static QVector4D m_GrayTransparency;

public:
    void setSelectedAxis(const BBAxisFlags &axis);

private:
    void setVertexColor(const BBAxisFlags &axis, bool bSelected);
    void setVertexColor(int start, int end, const QVector3D &color);
    void setVertexColor(int start, int end, const QVector4D &color);
};


// Conical arrow of the position Coordinate system
class BBCoordinateArrow : public BBCoordinateComponent
{
public:
    BBCoordinateArrow();
    BBCoordinateArrow(float px, float py, float pz,
                      float rx, float ry, float rz,
                      float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


// Straight line of coordinate axis
class BBCoordinateAxis : public BBCoordinateComponent
{
public:
    BBCoordinateAxis();
    BBCoordinateAxis(float px, float py, float pz,
                     float rx, float ry, float rz,
                     float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


// Face of the position Coordinate system
class BBCoordinateRectFace : public BBCoordinateComponent
{
public:
    BBCoordinateRectFace();
    BBCoordinateRectFace(float px, float py, float pz,
                         float rx, float ry, float rz,
                         float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


// Quarter circle of rotation Coordinate system
class BBCoordinateQuarterCircle : public BBCoordinateComponent
{
public:
    BBCoordinateQuarterCircle();
    BBCoordinateQuarterCircle(float px, float py, float pz,
                              float rx, float ry, float rz,
                              float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


class BBCoordinateCircle : public BBCoordinateComponent
{
public:
    BBCoordinateCircle();
    BBCoordinateCircle(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


// Tick marks on the Circle
class BBCoordinateTickMark : public BBCoordinateComponent
{
public:
    BBCoordinateTickMark();
    BBCoordinateTickMark(float px, float py, float pz,
                         float rx, float ry, float rz,
                         float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
};


// Sector of rotation angle
class BBCoordinateSector : public BBCoordinateComponent
{
public:
    BBCoordinateSector();
    BBCoordinateSector(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);

    void init() override;
    void render(BBCamera *pCamera) override;

    void setAngle(int nDeltaAngle);
    void reset();

private:
    void draw() override;

    int m_nAngle;
};


// Cube of Scale Coordinate system
class BBCoordinateCube : public BBCoordinateComponent
{
public:
    BBCoordinateCube();
    BBCoordinateCube(float px, float py, float pz,
                     float rx, float ry, float rz,
                     float sx, float sy, float sz);

    void init() override;
    void move(const QVector3D &delta);

private:
    void draw() override;

    float m_fHalfLength;
    static QVector3D m_Sign[8];
};


// Triangle face of Scale Coordinate system
class BBCoordinateTriangleFace : public BBCoordinateComponent
{
public:
    BBCoordinateTriangleFace();
    BBCoordinateTriangleFace(float px, float py, float pz,
                             float rx, float ry, float rz,
                             float sx, float sy, float sz);

    void init() override;

private:
    void draw() override;
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
    inline BBAxisFlags getSelectedAxis() { return m_SelectedAxis; }
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
    void resize(float fWidth, float fHeight) override;

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
    void resize(float fWidth, float fHeight) override;

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
    void resize(float fWidth, float fHeight) override;

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
    void resize(float fWidth, float fHeight) override;

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
