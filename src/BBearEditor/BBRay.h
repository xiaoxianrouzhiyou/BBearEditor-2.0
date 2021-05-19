#ifndef BBRAY_H
#define BBRAY_H


#include <QtOpenGL>
#include <QVector3D>


enum BBPlaneName
{
    XOY = 0,
    XOZ = 1,
    YOZ = 2
};


class BBRay
{
public:
    BBRay(GLdouble nearX, GLdouble nearY, GLdouble nearZ, GLdouble farX, GLdouble farY, GLdouble farZ);

    QVector3D computeIntersectWithXOZPlane(float y);
    QVector3D computeIntersectWithXOYPlane(float z);
    QVector3D computeIntersectWithYOZPlane(float x);

    bool computeIntersectWithPlane(QVector3D point1, QVector3D point2,
                                   QVector3D point3, QVector3D &intersection);
    bool computeIntersectWithPlane(QVector3D point, QVector3D normal, QVector3D &intersection);
    bool computeIntersectWithTriangle(QVector3D point1, QVector3D point2,
                                      QVector3D point3, QVector3D &intersection);
    bool computeIntersectWithRectangle(QVector3D point1, QVector3D point2,
                                       QVector3D point3, QVector3D point4, QVector3D &intersection);

    float computeIntersectDistance(QVector3D intersection);

    // Circle parallel to coordinate plane
    bool computeIntersectWithCircle(const QVector3D &center, float fRadius,
                                    const BBPlaneName &ePlaneName, QVector3D &intersection);
    bool computeIntersectWithQuarterCircle(const QVector3D &center, float fRadius,
                                           const BBPlaneName &ePlaneName, QVector3D &intersection,
                                           const QVector3D &quadrantFlag);

private:
    QVector3D m_NearPoint;
    QVector3D m_FarPoint;
};




//class Ray
//{
//public:


//    bool equal(Ray ray);
//    QVector3D getNearPoint();
//    QVector3D getFarPoint();


//};


#endif // BBRAY_H
