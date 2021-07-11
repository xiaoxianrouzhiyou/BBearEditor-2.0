#ifndef BBCOORDINATECOMPONENT_H
#define BBCOORDINATECOMPONENT_H


#include "Base/BBRenderableObject.h"
#include "Utils/BBUtils.h"


// The base class of each component to be rendered in the coordinate system
class BBCoordinateComponent : public BBRenderableObject
{
protected:
    BBCoordinateComponent();
    BBCoordinateComponent(float px, float py, float pz,
                          float rx, float ry, float rz,
                          float sx, float sy, float sz);

    void init() override;

    BBAxisFlags m_SelectedAxis;

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
};


class BBCoordinateCircle : public BBCoordinateComponent
{
public:
    BBCoordinateCircle();
    BBCoordinateCircle(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);

    void init() override;
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
};


#endif // BBCOORDINATECOMPONENT_H
