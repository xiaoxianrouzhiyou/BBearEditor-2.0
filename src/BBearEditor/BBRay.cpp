#include "BBRay.h"

BBRay::BBRay(GLdouble nearX, GLdouble nearY, GLdouble nearZ, GLdouble farX, GLdouble farY, GLdouble farZ)
{
    m_NearPoint.setX(nearX);
    m_NearPoint.setY(nearY);
    m_NearPoint.setZ(nearZ);
    m_FarPoint.setX(farX);
    m_FarPoint.setY(farY);
    m_FarPoint.setZ(farZ);
}

QVector3D BBRay::computeIntersectWithXOZPlane(float y)
{
    // (x-x1)/(x2-x1)=(y-y1)/(y2-y1)
    float temp = (y - m_NearPoint.y()) / (m_FarPoint.y() - m_NearPoint.y());
    float x = temp * (m_FarPoint.x() - m_NearPoint.x()) + m_NearPoint.x();
    float z = temp * (m_FarPoint.z() - m_NearPoint.z()) + m_NearPoint.z();
    return QVector3D(x, y, z);
}

QVector3D BBRay::computeIntersectWithXOYPlane(float z)
{
    float temp = (z - m_NearPoint.z()) / (m_FarPoint.z() - m_NearPoint.z());
    float x = temp * (m_FarPoint.x() - m_NearPoint.x()) + m_NearPoint.x();
    float y = temp * (m_FarPoint.y() - m_NearPoint.y()) + m_NearPoint.y();
    return QVector3D(x, y, z);
}

QVector3D BBRay::computeIntersectWithYOZPlane(float x)
{
    float temp = (x - m_NearPoint.x()) / (m_FarPoint.x() - m_NearPoint.x());
    float y = temp * (m_FarPoint.y() - m_NearPoint.y()) + m_NearPoint.y();
    float z = temp * (m_FarPoint.z() - m_NearPoint.z()) + m_NearPoint.z();
    return QVector3D(x, y, z);
}

bool BBRay::computeIntersectWithPlane(QVector3D point1, QVector3D point2,
                                      QVector3D point3, QVector3D &intersection)
{
    // The cross product of two straight lines in a plane, the plane normal
    QVector3D normal = QVector3D::crossProduct(point1 - point2, point1 - point3);
    return computeIntersectWithPlane(point1, normal, intersection);
}

bool BBRay::computeIntersectWithPlane(QVector3D point, QVector3D normal, QVector3D &intersection)
{
    QVector3D direction = m_FarPoint - m_NearPoint;
    // The mouse does not move into the scene
    if (direction.isNull())
    {
        // qDebug() << "The mouse does not move into the scene";
        return false;
    }
    // The parametric equation of the line and point-normal equation of the plane
    // to find the parameter t of the parametric equation
    float temp = QVector3D::dotProduct(normal, direction);
    if (temp == 0)
    {
        // line is parallel to the plane
        return false;
    }
    float t = QVector3D::dotProduct(point - m_NearPoint, normal) / temp;
    // Use t in the parametric equation to calculate the point of intersection
    intersection = direction * t + m_NearPoint;
    return true;
}

bool BBRay::computeIntersectWithTriangle(QVector3D point1, QVector3D point2,
                                         QVector3D point3, QVector3D &intersection)
{
    if (!computeIntersectWithPlane(point1, point2, point3, intersection))
    {
        return false;
    }
    // whether the intersection is within the triangle
    // Any point on the plane can be expressed as P = A +  u * (C – A) + v * (B - A)
    // u >= 0 v >= 0 u + v <= 1, point is within the triangle
    QVector3D v0 = point3 - point1;
    QVector3D v1 = point2 - point1;
    QVector3D v2 = intersection - point1;
    float dot00 = QVector3D::dotProduct(v0, v0);
    float dot01 = QVector3D::dotProduct(v0, v1);
    float dot02 = QVector3D::dotProduct(v0, v2);
    float dot11 = QVector3D::dotProduct(v1, v1);
    float dot12 = QVector3D::dotProduct(v1, v2);
    float temp = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * temp;
    if (u < 0 || u > 1)
    {
        // qDebug() << "point is not within the triangle, u" << u;
        return false;
    }
    float v = (dot00 * dot12 - dot01 * dot02) * temp;
    if (v < 0 || v > 1)
    {
        // qDebug() << "point is not within the triangle, v" << v;
        return false;
    }
    if (u + v <= 1)
    {
        return true;
    }
    else
    {
        // qDebug() << "point is not within the triangle, uv";
        return false;
    }
}

bool BBRay::computeIntersectWithRectangle(QVector3D point1, QVector3D point2,
                                          QVector3D point3, QVector3D point4, QVector3D &intersection)
{
    // Order of the 1234 quadrant
    if (computeIntersectWithTriangle(point1, point2, point3, intersection))
    {
        return true;
    }
    else
    {
        if (computeIntersectWithTriangle(point1, point3, point4, intersection))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

float BBRay::computeIntersectDistance(QVector3D intersection)
{
    return m_NearPoint.distanceToPoint(intersection);
}


//bool Ray::computeIntersectWithRound(QVector3D center, float radius, PlaneName plane, QVector3D &intersection)
//{
//    QVector3D normal;
//    if (plane == PlaneName::YOZ)
//    {
//        normal = QVector3D(1, 0, 0);
//    }
//    else if (plane == PlaneName::XOZ)
//    {
//        normal = QVector3D(0, 1, 0);
//    }
//    else if (plane == PlaneName::XOY)
//    {
//        normal = QVector3D(0, 0, 1);
//    }
//    else
//    {
//        return false;
//    }

//    if (computeIntersectWithPlane(center, normal, intersection))
//    {
//        float distance = (intersection - center).length();
//        if (distance <= radius)
//            return true;
//    }

//    return false;
//}

//bool Ray::computeIntersectWithQuarterRound(QVector3D center, float radius,
//                                      PlaneName plane, QVector3D &intersection,
//                                      int xSign, int ySign, int zSign)
//{
//    //根据符号判断是哪一个象限的四分之一圆
//    if (!computeIntersectWithRound(center, radius, plane, intersection))
//    {
//        return false;
//    }

//    QVector3D temp = intersection - center;
//    if (plane == PlaneName::YOZ)
//    {
//        //同号 相乘为非负
//        if ((temp.y() * ySign) >= 0 && (temp.z() * zSign) >= 0)
//        {
//            return true;
//        }
//    }
//    else if (plane == PlaneName::XOZ)
//    {
//        if ((temp.x() * xSign) >= 0 && (temp.z() * zSign) >= 0)
//        {
//            return true;
//        }
//    }
//    else//plane == PlaneName::XOY
//    {
//        if ((temp.x() * xSign) >= 0 && (temp.y() * ySign) >= 0)
//        {
//            return true;
//        }
//    }
//    return false;
//}

//bool Ray::equal(Ray ray)
//{
//    if (nearPoint == ray.nearPoint && farPoint == ray.farPoint)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
