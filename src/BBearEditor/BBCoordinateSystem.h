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
class BBBoundingBox3D;


// The base class of each component to be rendered in the coordinate system
class BBCoordinateComponent : public BBRenderableObject
{
protected:
    BBCoordinateComponent();
    BBCoordinateComponent(const float px, const float py, const float pz,
                          const float rx, const float ry, const float rz,
                          const float sx, const float sy, const float sz);

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
    void setSelectedAxis(BBAxisFlags axis);

private:
    void setVertexColor(BBAxisFlags axis, bool bSelected);
    void setVertexColor(int start, int end, QVector3D color);
    void setVertexColor(int start, int end, QVector4D color);
};


// Conical arrow of the position Coordinate system
class BBCoordinateArrow : public BBCoordinateComponent
{
public:
    BBCoordinateArrow();
    BBCoordinateArrow(const float px, const float py, const float pz,
                      const float rx, const float ry, const float rz,
                      const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


// Straight line of coordinate axis
class BBCoordinateAxis : public BBCoordinateComponent
{
public:
    BBCoordinateAxis();
    BBCoordinateAxis(const float px, const float py, const float pz,
                     const float rx, const float ry, const float rz,
                     const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


// Face of the position Coordinate system
class BBCoordinateRectFace : public BBCoordinateComponent
{
public:
    BBCoordinateRectFace();
    BBCoordinateRectFace(const float px, const float py, const float pz,
                         const float rx, const float ry, const float rz,
                         const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


// Quarter circle of rotation Coordinate system
class BBCoordinateQuarterCircle : public BBCoordinateComponent
{
public:
    BBCoordinateQuarterCircle();
    BBCoordinateQuarterCircle(const float px, const float py, const float pz,
                              const float rx, const float ry, const float rz,
                              const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


class BBCoordinateCircle : public BBCoordinateComponent
{
public:
    BBCoordinateCircle();
    BBCoordinateCircle(const float px, const float py, const float pz,
                       const float rx, const float ry, const float rz,
                       const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


// Tick marks on the Circle
class BBCoordinateTickMark : public BBCoordinateComponent
{
public:
    BBCoordinateTickMark();
    BBCoordinateTickMark(const float px, const float py, const float pz,
                         const float rx, const float ry, const float rz,
                         const float sx, const float sy, const float sz);

    void init() override;

private:
    void draw() override;
};


//// Sector of rotation angle
//class BBCoordinateSector : public BBCoordinateComponent
//{
//public:
//    BBCoordinateSector();
//    BBCoordinateSector(const float px, const float py, const float pz,
//                       const float rx, const float ry, const float rz,
//                       const float sx, const float sy, const float sz);

//    void init() override;
//    void render(QMatrix4x4 modelMatrix, BBCamera *pCamera) override;
//    void reset(QVector3D position);
//    int setRotateAngle(int nowAngle, float nowCosRadius);

//private:
//    void draw() override;
//    unsigned short *mIndexes;
//    int mIndexCount;
//    //旋转角度
//    int mAngle;
//    float mLastCosRadius;
//    bool mIsUpper180;
//    bool mIsLowerMinus180;
//};


// Cube of Scale Coordinate system
class BBCoordinateCube : public BBCoordinateComponent
{
public:
    BBCoordinateCube();
    BBCoordinateCube(const float px, const float py, const float pz,
                     const float rx, const float ry, const float rz,
                     const float sx, const float sy, const float sz);

    void init() override;
    void changePositionAfterScale(const float x, const float y, const float z);

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
    BBCoordinateTriangleFace(const float px, const float py, const float pz,
                             const float rx, const float ry, const float rz,
                             const float sx, const float sy, const float sz);

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

    QMatrix4x4 getRenderModelMatrix(const QVector3D cameraPos);
    bool hit(BBRay ray,
             BBBoundingBox *pBoundingBox1, BBAxisFlags axis1,
             BBBoundingBox *pBoundingBox2, BBAxisFlags axis2,
             BBBoundingBox *pBoundingBox3, BBAxisFlags axis3,
             float &fDistance);

    BBAxisFlags m_SelectedAxis;
    QVector3D m_LastMousePos;
    BBGameObject *m_pSelectedObject;

public:
    inline BBAxisFlags getSelectedAxis() { return m_SelectedAxis; }
    void setSelectedObject(BBGameObject *pObject);
    void resetLastMousePos();

    virtual void setSelectedAxis(BBAxisFlags axis) = 0;
    virtual bool mouseMoveEvent(BBRay ray) = 0;
};


class BBPositionCoordinateSystem : public BBCoordinateSystem
{
public:
    BBPositionCoordinateSystem();
    virtual ~BBPositionCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setSelectedAxis(BBAxisFlags axis) override;
    bool mouseMoveEvent(BBRay ray) override;

private:
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

    void setSelectedAxis(BBAxisFlags axis) override;
    bool mouseMoveEvent(BBRay ray) override;

//    void stopRotate();

private:
//    QuarterRound *mQuarterRound;
//    QuarterRoundBoundingBox2D *yozSurface;
//    QuarterRoundBoundingBox2D *xozSurface;
//    QuarterRoundBoundingBox2D *xoySurface;
//    bool isRotating;
//    Round *mRound;
//    TickLine *mTickLine;
//    Sector *mSector;
//    QVector3D mLastVector;
};


class BBScaleCoordinateSystem : public BBCoordinateSystem
{
public:
    BBScaleCoordinateSystem();
    ~BBScaleCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setSelectedAxis(BBAxisFlags axis) override;
    bool mouseMoveEvent(BBRay ray) override;

//    void stopScale();

private:
//    Axis *mAxis;
//    Cube *mCube;
//    TriangleSurface *surface;
//    RectBoundingBox2D *yozSurface;
//    RectBoundingBox2D *xozSurface;
//    RectBoundingBox2D *xoySurface;
//    BoundingBox3D *xBoundingBox;
//    BoundingBox3D *yBoundingBox;
//    BoundingBox3D *zBoundingBox;
//    TriangleBoundingBox2D *xyzSurface;
//    bool mIsScaling;
//    QVector3D mSelectedObjectScale;
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

    bool mouseMoveEvent(const BBRay ray);
    void setCoordinateSystemMode(const char key);
    void setSelectedObject(BBGameObject *pObject);
    bool isTransforming() { return m_bTransforming; }
    void stopTransform();
//    void setSelectedObjects(QList<GameObject*> objects, CenterPoint *center);


//    GameObject *getSelectedObject();
//    char getTransformModeKey();
//    void update();

private:
    BBPositionCoordinateSystem *m_pPositionCoordinateSystem;
    BBRotationCoordinateSystem *m_pRotationCoordinateSystem;
    BBScaleCoordinateSystem *m_pScaleCoordinateSystem;

    char m_PositionCoordinateSystemModeKey = 'W';
    char m_RotationCoordinateSystemModeKey = 'E';
    char m_ScaleCoordinateSystemModeKey = 'R';
    char m_ModeKey;

    BBGameObject *m_pSelectedObject;
    bool m_bTransforming;

//    QList<GameObject*> mSelectedObjects;
};



#endif // BBCOORDINATE_H
