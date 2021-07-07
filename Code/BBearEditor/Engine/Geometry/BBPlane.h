#ifndef BBPLANE_H
#define BBPLANE_H


#include <QVector3D>

class BBPlane
{
public:
    BBPlane();
    BBPlane(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3);

    float distance(const QVector3D &point);

private:
    QVector3D m_Point1;
    QVector3D m_Point2;
    QVector3D m_Point3;
};

#endif // BBPLANE_H
