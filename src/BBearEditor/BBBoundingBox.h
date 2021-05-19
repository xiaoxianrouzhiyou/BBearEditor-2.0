#ifndef BBBOUNDINGBOX_H
#define BBBOUNDINGBOX_H


#include "BBRenderableObject.h"
#include "BBRay.h"
#include <QMatrix4x4>


struct FloatData
{
    float v[3];
};

class BBBoundingBox : public BBRenderableObject
{
public:
    BBBoundingBox();
    BBBoundingBox(const float &px, const float &py, const float &pz,
                  const float &rx, const float &ry, const float &rz,
                  const float &sx, const float &sy, const float &sz);
    virtual ~BBBoundingBox();

    bool hit(BBRay ray, float &fDistance) override;

    QVector3D getCenter();

protected:
    void setModelMatrix(float px, float py, float pz,
                        const QQuaternion &r,
                        float sx, float sy, float sz) override;

    float m_Center[3];
    int m_nBoxVertexCount;
    QVector3D *m_pOriginalBoxVertexes;
    QVector3D *m_pTransformedBoxVertexes;
};


// Parallel to the coordinate plane
class BBRectBoundingBox2D : public BBBoundingBox
{
public:
    BBRectBoundingBox2D(const float fCenterX, const float fCenterY, const float fCenterZ,
                        const float fHalfLengthX, const float fHalfLengthY, const float fHalfLengthZ);
    virtual ~BBRectBoundingBox2D();

    bool hit(BBRay ray, float &fDistance) override;
};


// Not necessarily parallel to the coordinate plane
class BBTriangleBoundingBox2D : public BBBoundingBox
{
public:
    BBTriangleBoundingBox2D(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3);

    bool hit(BBRay ray, float &fDistance) override;
};


class BBBoundingBox3D : public BBBoundingBox
{
public:
    BBBoundingBox3D(QList<QVector4D> vertexes = QList<QVector4D>());
    BBBoundingBox3D(const float &px, const float &py, const float &pz,
                    const float &rx, const float &ry, const float &rz,
                    const float &sx, const float &sy, const float &sz,
                    QList<QVector4D> vertexes = QList<QVector4D>());
    BBBoundingBox3D(const float &px, const float &py, const float &pz,
                    const float &rx, const float &ry, const float &rz,
                    const float &sx, const float &sy, const float &sz,
                    const float fCenterX, const float fCenterY, const float fCenterZ,
                    const float fHalfLengthX, const float fHalfLengthY, const float fHalfLengthZ);
    virtual ~BBBoundingBox3D();

    void init() override;
    bool hit(BBRay ray, float &fDistance) override;
//    QVector3D getHalfLength();
//    bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                 QVector3D top1, QVector3D top2, QVector3D top3,
//                                 QVector3D right1, QVector3D right2, QVector3D right3,
//                                 QVector3D bottom1, QVector3D bottom2, QVector3D bottom3,
//                                 QMatrix4x4 matrix);

protected:
    void draw() override;
    virtual void computeBoxVertexes(QList<QVector4D> vertexes);

    float m_Axis[3][3];
    float m_HalfLength[3];
};


class BBAABBBoundingBox3D : public BBBoundingBox3D
{
public:
    BBAABBBoundingBox3D(QList<QVector4D> vertexes = QList<QVector4D>());
    BBAABBBoundingBox3D(const float &px, const float &py, const float &pz,
                        const float &rx, const float &ry, const float &rz,
                        const float &sx, const float &sy, const float &sz,
                        QList<QVector4D> vertexes = QList<QVector4D>());

private:
    void computeBoxVertexes(QList<QVector4D> vertexes) override;
};


#endif // BBBOUNDINGBOX_H







////平行于坐标面
//class QuarterRoundBoundingBox2D : public BoundingBox
//{
//private:
//    QVector3D mOriginCenter;
//    //圆上的一个点
//    QVector3D mOriginPoint;
//    QVector3D mTransferedCenter;
//    QVector3D mTransferedPoint;
//    PlaneName mPlane;
//    int mXSign;
//    int mYSign;
//    int mZSign;
//    void transformBoundingBoxVertexes(QMatrix4x4 matrix) override;

//public:
//    QuarterRoundBoundingBox2D(float centerX, float centerY, float centerZ, float radius, PlaneName plane);
//    bool hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix) override;
//    void setSign(int xSign, int ySign, int zSign);
//};




//class OBBBoundingBox3D : public BoundingBox3D
//{
//public:
//    OBBBoundingBox3D();
//    OBBBoundingBox3D(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

//private:
//    void computeBoundingBox() override;
//};


