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

    virtual bool hitBoundingBox(BBRay ray, float &fDistance);
    QVector3D getCenter();

protected:
    float m_Center[3];
};


class BBBoundingBox3D : public BBBoundingBox
{
public:
    BBBoundingBox3D(QList<QVector4D> vertexes = QList<QVector4D>());
    BBBoundingBox3D(const float &px, const float &py, const float &pz,
                    const float &rx, const float &ry, const float &rz,
                    const float &sx, const float &sy, const float &sz,
                    QList<QVector4D> vertexes = QList<QVector4D>());
    virtual ~BBBoundingBox3D();

    void init() override;
    bool hitBoundingBox(BBRay ray, float &fDistance) override;
//    QVector3D getHalfLength();
//    bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                 QVector3D top1, QVector3D top2, QVector3D top3,
//                                 QVector3D right1, QVector3D right2, QVector3D right3,
//                                 QVector3D bottom1, QVector3D bottom2, QVector3D bottom3,
//                                 QMatrix4x4 matrix);

protected:
    void setModelMatrix(const float px, const float py, const float pz,
                        const QQuaternion r,
                        const float sx, const float sy, const float sz) override;
    void draw() override;
    virtual void computeBoxVertexes(QList<QVector4D> vertexes);

    QVector3D m_OriginalBoxVertexes[8];
    QVector3D m_TransformedBoxVertexes[8];
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


//#include <QVector3D>
//#include "renderableobject.h"
//#include "ray.h"
//#include <QMatrix3x3>




////平行于坐标面
//class RectBoundingBox2D : public BoundingBox
//{
//private:
//    QVector3D originBoxVertexes[4];
//    QVector3D transferedBoxVertexes[4];
//    void transformBoundingBoxVertexes(QMatrix4x4 matrix) override;

//public:
//    RectBoundingBox2D(float centerX, float centerY, float centerZ,
//                      float radiusX, float radiusY, float radiusZ);
//    bool hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix) override;
//};

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

////不一定平行坐标面
//class TriangleBoundingBox2D : public BoundingBox
//{
//private:
//    QVector3D mOriginBoxVertexes[3];
//    QVector3D mTransferedBoxVertexes[3];
//    void transformBoundingBoxVertexes(QMatrix4x4 matrix) override;

//public:
//    TriangleBoundingBox2D(QVector3D point1, QVector3D point2, QVector3D point3);
//    bool hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix) override;
//};



//class OBBBoundingBox3D : public BoundingBox3D
//{
//public:
//    OBBBoundingBox3D();
//    OBBBoundingBox3D(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

//private:
//    void computeBoundingBox() override;
//};


