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

//bool Ray::computeIntersectWithPlane(QVector3D point1, QVector3D point2,
//                                    QVector3D point3, QVector3D &intersection)
//{
//    //平面两直线的叉集 求平面法向量
//    QVector3D normal = QVector3D::crossProduct(point1 - point2, point1 - point3);
//    return computeIntersectWithPlane(point1, normal, intersection);
//}

//bool Ray::computeIntersectWithPlane(QVector3D point, QVector3D normal, QVector3D &intersection)
//{
//    //直线的方向向量
//    QVector3D direction = farPoint - nearPoint;
//    //鼠标没有移动到场景中
//    if (direction.isNull())
//    {
//        //qDebug() << "鼠标没有移动到场景中";
//        return false;
//    }
//    //直线的参数方程与平面的点法式联立 求出参数方程的参数t
//    float temp = QVector3D::dotProduct(normal, direction);
//    if (temp == 0)
//    {
//        //直线与平面平行
//        //qDebug() << "直线与平面平行";
//        return false;
//    }
//    float t = QVector3D::dotProduct(point - nearPoint, normal) / temp;
//    //将t代回直线的参数方程 求出交点
//    intersection = direction * t + nearPoint;
//    return true;
//}

//bool Ray::computeIntersectWithTriangle(QVector3D point1, QVector3D point2,
//                                       QVector3D point3, QVector3D &intersection)
//{
//    if (!computeIntersectWithPlane(point1, point2, point3, intersection))
//    {
//        return false;
//    }
//    //判断交点是否在三角型内
//    //平面任意一点可表示为P = A +  u * (C – A) + v * (B - A)
//    //当u >= 0 v >= 0 u + v <= 1时 点在三角形内部
//    QVector3D v0 = point3 - point1;
//    QVector3D v1 = point2 - point1;
//    QVector3D v2 = intersection - point1;
//    float dot00 = QVector3D::dotProduct(v0, v0);
//    float dot01 = QVector3D::dotProduct(v0, v1);
//    float dot02 = QVector3D::dotProduct(v0, v2);
//    float dot11 = QVector3D::dotProduct(v1, v1);
//    float dot12 = QVector3D::dotProduct(v1, v2);
//    float temp = 1.0f / (dot00 * dot11 - dot01 * dot01);
//    float u = (dot11 * dot02 - dot01 * dot12) * temp;
//    if (u < 0 || u > 1)
//    {
//        //qDebug() << "点不在三角形内u" << u;
//        return false;
//    }
//    float v = (dot00 * dot12 - dot01 * dot02) * temp;
//    if (v < 0 || v > 1)
//    {
//        //qDebug() << "点不在三角形内v" << v;
//        return false;
//    }
//    if (u + v <= 1)
//    {
//        return true;
//    }
//    else
//    {
//        //qDebug() << "点不在三角形内uv";
//        return false;
//    }
//}

//bool Ray::computeIntersectWithRectangle(QVector3D point1, QVector3D point2,
//                                        QVector3D point3, QVector3D point4, QVector3D &intersection)
//{
//    //1234象限的顺序
//    if (computeIntersectWithTriangle(point1, point2, point3, intersection))
//    {
//        return true;
//    }
//    else
//    {
//        if (computeIntersectWithTriangle(point1, point3, point4, intersection))
//        {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//}

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

//float Ray::computeIntersectDistance(QVector3D intersection)
//{
//    return nearPoint.distanceToPoint(intersection);
//}

//QVector3D Ray::getNearPoint()
//{
//    return nearPoint;
//}

//QVector3D Ray::getFarPoint()
//{
//    return farPoint;
//}
