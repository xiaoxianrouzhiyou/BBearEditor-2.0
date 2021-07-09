#ifndef BBPLANE_H
#define BBPLANE_H


#include <QVector3D>

class BBPlane
{
public:
    BBPlane();
    BBPlane(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3);

    float distance(const QVector3D &point) const;
    static float distance(const QVector3D &pointOnPlane, const QVector3D &planeNormal, const QVector3D &point);

    static BBPlane invert(const BBPlane &plane);

    QVector3D getPoint1() const { return m_Point1; }
    QVector3D getPoint2() const { return m_Point2; }
    QVector3D getPoint3() const { return m_Point3; }
    QVector3D getNormal() const { return m_Normal; }

private:
    QVector3D m_Point1;
    QVector3D m_Point2;
    QVector3D m_Point3;
    QVector3D m_Normal;
};

#endif // BBPLANE_H
