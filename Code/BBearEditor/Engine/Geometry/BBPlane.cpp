#include "BBPlane.h"

BBPlane::BBPlane()
    : BBPlane(QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 0, 1))
{

}

BBPlane::BBPlane(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3)
{
    m_Point1 = point1;
    m_Point2 = point2;
    m_Point3 = point3;
    m_Normal = QVector3D::normal(point1, point2, point3);
}

float BBPlane::distance(const QVector3D &point) const
{
    return point.distanceToPlane(m_Point1, m_Point2, m_Point3);
}
