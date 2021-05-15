#ifndef BBRAY_H
#define BBRAY_H


#include <QtOpenGL>
#include <QVector3D>


class BBRay
{
public:
    BBRay(GLdouble nearX, GLdouble nearY, GLdouble nearZ, GLdouble farX, GLdouble farY, GLdouble farZ);

    QVector3D computeIntersectWithXOZPlane(float y);
    QVector3D computeIntersectWithXOYPlane(float z);
    QVector3D computeIntersectWithYOZPlane(float x);

private:
    QVector3D m_NearPoint;
    QVector3D m_FarPoint;
};


//enum PlaneName
//{
//    XOY = 0,
//    XOZ = 1,
//    YOZ = 2
//};

//class Ray
//{
//public:

//    bool computeIntersectWithPlane(QVector3D point1, QVector3D point2,
//                                        QVector3D point3, QVector3D &intersection);
//    bool computeIntersectWithPlane(QVector3D point, QVector3D normal, QVector3D &intersection);
//    bool computeIntersectWithTriangle(QVector3D point1, QVector3D point2,
//                                      QVector3D point3, QVector3D &intersection);
//    bool computeIntersectWithRectangle(QVector3D point1, QVector3D point2,
//                                      QVector3D point3, QVector3D point4, QVector3D &intersection);
//    //平行坐标轴的圆
//    bool computeIntersectWithRound(QVector3D center, float radius, PlaneName plane, QVector3D &intersection);
//    bool computeIntersectWithQuarterRound(QVector3D center, float radius,
//                                          PlaneName plane, QVector3D &intersection,
//                                          int xSign, int ySign, int zSign);
//    bool equal(Ray ray);
//    float computeIntersectDistance(QVector3D intersection);
//    QVector3D getNearPoint();
//    QVector3D getFarPoint();


//};


#endif // BBRAY_H
