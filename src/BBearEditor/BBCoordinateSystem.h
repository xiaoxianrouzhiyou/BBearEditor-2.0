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
Q_DECLARE_INCOMPATIBLE_FLAGS(BBAxisFlags)



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

//    QMatrix4x4 getModelMatrix(QVector3D cameraPos);
//    bool hitBoundingBox(QMatrix4x4 matrix, BoundingBox *boundingBox1,
//                    BoundingBox *boundingBox2, BoundingBox *boundingBox3,
//                    AxisFlags axisFlags1, AxisFlags axisFlags2, AxisFlags axisFlags3,
//                    float &distance);

//    BBRay m_Ray;
    BBAxisFlags m_SelectedAxis;
    QVector3D m_LastMousePos;
    BBGameObject *m_pSelectedObject;

public:
//    inline void setRay(const BBRay ray) { m_Ray = ray; }
    inline BBAxisFlags getSelectedAxis() { return m_SelectedAxis; }
    void setSelectedObject(BBGameObject *pObject);
    void resetLastMousePos();

    virtual void setSelectedAxis(BBAxisFlags axis) = 0;
};


class BBPositionCoordinateSystem : public BBCoordinateSystem
{
public:
    BBPositionCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setSelectedAxis(BBAxisFlags axis) override;
    bool move(BBRay ray);

private:
//    Arrow *mArrow;
//    Axis *mAxis;
//    RectSurface *mSurface;
//    BoundingBox3D *xBoundingBox;
//    BoundingBox3D *yBoundingBox;
//    BoundingBox3D *zBoundingBox;
//    RectBoundingBox2D *yozSurface;
//    RectBoundingBox2D *xozSurface;
//    RectBoundingBox2D *xoySurface;
};

////旋转坐标轴
//class RotationCoordinate : public Coordinate
//{
//public:
//    RotationCoordinate();
//    void init() override;
//    void render(Camera camera) override;
//    void resize(float width, float height) override;
//    bool rotate(Ray ray);
//    void stopRotate();
//    void setSelectedAxis(AxisFlags axis) override;

//private:
//    QuarterRound *mQuarterRound;
//    QuarterRoundBoundingBox2D *yozSurface;
//    QuarterRoundBoundingBox2D *xozSurface;
//    QuarterRoundBoundingBox2D *xoySurface;
//    bool isRotating;
//    Round *mRound;
//    TickLine *mTickLine;
//    Sector *mSector;
//    QVector3D mLastVector;
//};

////缩放坐标轴
//class ScaleCoordinate : public Coordinate
//{
//public:
//    ScaleCoordinate();
//    void init() override;
//    void render(Camera camera) override;
//    void resize(float width, float height) override;
//    bool scale(Ray ray);
//    void stopScale();
//    void setSelectedAxis(AxisFlags axis) override;

//private:
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
//};


////管理三大坐标轴的类
//class TransformCoordinate : public GameObject
//{
//public:
//    TransformCoordinate();
//    void init() override;
//    void render(Camera camera) override;
//    void resize(float width, float height) override;
//    void setRay(Ray ray);
//    void stopTransform();
//    void setSelectedObject(GameObject *object);
//    void setSelectedObjects(QList<GameObject*> objects, CenterPoint *center);
//    void transform(Ray ray);
//    void setCoordinateMode(char key);
//    bool getIsTransform();
//    GameObject *getSelectedObject();
//    char getTransformModeKey();
//    void update();

//private:
//    PositionCoordinate *mPositionCoordinate;
//    RotationCoordinate *mRotationCoordinate;
//    ScaleCoordinate *mScaleCoordinate;
//    GameObject *mSelectedObject;
//    QList<GameObject*> mSelectedObjects;
//    char mModeKey;
//    bool mIsTransform;
//};



#endif // BBCOORDINATE_H
