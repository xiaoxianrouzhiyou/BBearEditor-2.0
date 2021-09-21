#include "BBRay.h"


BBRay::BBRay(const QVector3D &origin, const QVector3D &direction)
{
    // Represented by starting point and direction
    m_NearPoint = origin;
    m_Direction = direction.normalized();
    m_FarPoint = m_NearPoint + m_Direction;
}

BBRay::BBRay(const GLdouble &nearX, const GLdouble &nearY, const GLdouble &nearZ,
             const GLdouble &farX, const GLdouble &farY, const GLdouble &farZ)
{
    // Represented by two end points
    m_NearPoint = QVector3D(nearX, nearY, nearZ);
    m_FarPoint = QVector3D(farX, farY, farZ);
    m_Direction = (m_FarPoint - m_NearPoint).normalized();
}

void BBRay::setRay(const QVector3D &origin, const QVector3D &direction)
{
    // Represented by starting point and direction
    m_NearPoint = origin;
    m_Direction = direction.normalized();
    m_FarPoint = m_NearPoint + m_Direction;
}

QVector3D BBRay::computeIntersectWithXOZPlane(float y) const
{
    // (x-x1)/(x2-x1)=(y-y1)/(y2-y1)
    float temp = (y - m_NearPoint.y()) / (m_FarPoint.y() - m_NearPoint.y());
    float x = temp * (m_FarPoint.x() - m_NearPoint.x()) + m_NearPoint.x();
    float z = temp * (m_FarPoint.z() - m_NearPoint.z()) + m_NearPoint.z();
    return QVector3D(x, y, z);
}

QVector3D BBRay::computeIntersectWithXOYPlane(float z) const
{
    float temp = (z - m_NearPoint.z()) / (m_FarPoint.z() - m_NearPoint.z());
    float x = temp * (m_FarPoint.x() - m_NearPoint.x()) + m_NearPoint.x();
    float y = temp * (m_FarPoint.y() - m_NearPoint.y()) + m_NearPoint.y();
    return QVector3D(x, y, z);
}

QVector3D BBRay::computeIntersectWithYOZPlane(float x) const
{
    float temp = (x - m_NearPoint.x()) / (m_FarPoint.x() - m_NearPoint.x());
    float y = temp * (m_FarPoint.y() - m_NearPoint.y()) + m_NearPoint.y();
    float z = temp * (m_FarPoint.z() - m_NearPoint.z()) + m_NearPoint.z();
    return QVector3D(x, y, z);
}

bool BBRay::computeIntersectWithPlane(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, QVector3D &outIntersection) const
{
    // The cross product of two straight lines in a plane, the plane normal
    QVector3D normal = QVector3D::crossProduct(point1 - point2, point1 - point3);
    return computeIntersectWithPlane(point1, normal, outIntersection);
}

bool BBRay::computeIntersectWithPlane(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, QVector3D &outNormal, QVector3D &outIntersection) const
{
    outNormal = QVector3D::crossProduct(point1 - point2, point1 - point3);
    outNormal.normalize();
    return computeIntersectWithPlane(point1, outNormal, outIntersection);
}

bool BBRay::computeIntersectWithPlane(const QVector3D &point, const QVector3D &normal, QVector3D &outIntersection) const
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
    outIntersection = direction * t + m_NearPoint;
    return true;
}

bool BBRay::computeIntersectWithTriangle(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, QVector3D &outIntersection) const
{
    float intersectionU = 0.0f;
    float intersectionV = 0.0f;
    QVector3D normal;
    return computeIntersectWithTriangle(point1, point2, point3, normal, outIntersection, intersectionU, intersectionV);
}

bool BBRay::computeIntersectWithTriangle(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3,
                                         QVector3D &outNormal, QVector3D &outIntersectionPos, float &outIntersectionU, float &outIntersectionV) const
{
    if (!computeIntersectWithPlane(point1, point2, point3, outNormal, outIntersectionPos))
    {
        return false;
    }
    // whether the intersection is within the triangle
    // Any point on the plane can be expressed as P = A +  u * (C – A) + v * (B - A)
    // u >= 0 v >= 0 u + v <= 1, point is within the triangle
    QVector3D v0 = point3 - point1;
    QVector3D v1 = point2 - point1;
    QVector3D v2 = outIntersectionPos - point1;
    float dot00 = QVector3D::dotProduct(v0, v0);
    float dot01 = QVector3D::dotProduct(v0, v1);
    float dot02 = QVector3D::dotProduct(v0, v2);
    float dot11 = QVector3D::dotProduct(v1, v1);
    float dot12 = QVector3D::dotProduct(v1, v2);
    float temp = 1.0f / (dot00 * dot11 - dot01 * dot01);
    outIntersectionU = (dot11 * dot02 - dot01 * dot12) * temp;
    if (outIntersectionU < 0 || outIntersectionU > 1)
    {
        // qDebug() << "point is not within the triangle, u" << u;
        return false;
    }
    outIntersectionV = (dot00 * dot12 - dot01 * dot02) * temp;
    if (outIntersectionV < 0 || outIntersectionV > 1)
    {
        // qDebug() << "point is not within the triangle, v" << v;
        return false;
    }
    if (outIntersectionU + outIntersectionV <= 1)
    {
        return true;
    }
    else
    {
        // qDebug() << "point is not within the triangle, uv";
        return false;
    }
}

bool BBRay::computeIntersectWithRectangle(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, const QVector3D &point4, QVector3D &outIntersection) const
{
    // Order of the 1234 quadrant
    if (computeIntersectWithTriangle(point1, point2, point3, outIntersection))
    {
        return true;
    }
    else
    {
        if (computeIntersectWithTriangle(point1, point3, point4, outIntersection))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

float BBRay::computeIntersectDistance(const QVector3D &intersection) const
{
    // When it is negative, it intersects the reverse extension line of the ray
    int sign = QVector3D::dotProduct(intersection - m_NearPoint, m_Direction) >= 0 ? 1 : -1;
    return sign * m_NearPoint.distanceToPoint(intersection);
}

bool BBRay::computeIntersectWithCircle(const QVector3D &center, float fRadius, const BBPlaneName &ePlaneName, QVector3D &outIntersection) const
{
    QVector3D normal;
    if (ePlaneName == BBPlaneName::YOZ)
    {
        normal = QVector3D(1, 0, 0);
    }
    else if (ePlaneName == BBPlaneName::XOZ)
    {
        normal = QVector3D(0, 1, 0);
    }
    else if (ePlaneName == BBPlaneName::XOY)
    {
        normal = QVector3D(0, 0, 1);
    }
    else
    {
        return false;
    }

    if (computeIntersectWithPlane(center, normal, outIntersection))
    {
        float d = (outIntersection - center).length();
        if (d <= fRadius)
            return true;
    }

    return false;
}

bool BBRay::computeIntersectWithQuarterCircle(const QVector3D &center, float fRadius, const BBPlaneName &ePlaneName, QVector3D &outIntersection, const QVector3D &quadrantFlag) const
{
    if (!computeIntersectWithCircle(center, fRadius, ePlaneName, outIntersection))
    {
        return false;
    }

    // According to the flag, determine which quadrant the quarter circle belongs to
    QVector3D temp = outIntersection - center;
    if (ePlaneName == BBPlaneName::YOZ)
    {
        if ((temp.y() * quadrantFlag.y()) >= 0 && (temp.z() * quadrantFlag.z()) >= 0)
        {
            return true;
        }
    }
    else if (ePlaneName == BBPlaneName::XOZ)
    {
        if ((temp.x() * quadrantFlag.x()) >= 0 && (temp.z() * quadrantFlag.z()) >= 0)
        {
            return true;
        }
    }
    else//ePlaneName == BBPlaneName::XOY
    {
        if ((temp.x() * quadrantFlag.x()) >= 0 && (temp.y() * quadrantFlag.y()) >= 0)
        {
            return true;
        }
    }
    return false;
}
