#ifndef BBBOUNDINGBOX_H
#define BBBOUNDINGBOX_H


#include "Base/BBRenderableObject.h"
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
    BBBoundingBox(float px, float py, float pz,
                  float rx, float ry, float rz,
                  float sx, float sy, float sz);
    virtual ~BBBoundingBox();

    bool hit(const BBRay &ray, float &fDistance) override;

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
class BBRectBoundingBox3D : public BBBoundingBox
{
public:
    BBRectBoundingBox3D(float fCenterX, float fCenterY, float fCenterZ,
                        float fHalfLengthX, float fHalfLengthY, float fHalfLengthZ);

    virtual ~BBRectBoundingBox3D();

    void init() override;

    bool hit(const BBRay &ray, float &fDistance) override;
};


// Not necessarily parallel to the coordinate plane
class BBTriangleBoundingBox3D : public BBBoundingBox
{
public:
    BBTriangleBoundingBox3D(const QVector3D &point1, const QVector3D &point2, const QVector3D &point3);

    bool hit(const BBRay &ray, float &fDistance) override;
};


// Parallel to the coordinate plane
class BBQuarterCircleBoundingBox3D : public BBBoundingBox
{
public:
    BBQuarterCircleBoundingBox3D(float fCenterX, float fCenterY, float fCenterZ,
                                 float fRadius, const BBPlaneName &ePlaneName);

    bool hit(const BBRay &ray, float &fDistance) override;
    inline void setQuadrantFlag(const QVector3D &flag) { m_QuadrantFlag = flag; }

private:
    BBPlaneName m_eSelectedPlaneName;
    QVector3D m_QuadrantFlag;
};


class BBBoundingBox3D : public BBBoundingBox
{
public:
    BBBoundingBox3D(const QList<QVector4D> &vertexes = QList<QVector4D>());
    BBBoundingBox3D(float px, float py, float pz,
                    float rx, float ry, float rz,
                    float sx, float sy, float sz,
                    const QList<QVector4D> &vertexes = QList<QVector4D>());
    BBBoundingBox3D(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale,
                    const QVector3D &center, const QVector3D &halfLength);
    BBBoundingBox3D(float px, float py, float pz,
                    float rx, float ry, float rz,
                    float sx, float sy, float sz,
                    float fCenterX, float fCenterY, float fCenterZ,
                    float fHalfLengthX, float fHalfLengthY, float fHalfLengthZ);
    virtual ~BBBoundingBox3D();

    void init() override;
    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const BBFrustum &frustum) override;

    QVector3D getHalfLength();

protected:
    virtual void computeBoxVertexes(const QList<QVector4D> &vertexes);

    float m_Axis[3][3];
    float m_HalfLength[3];
};


class BBAABBBoundingBox3D : public BBBoundingBox3D
{
public:
    BBAABBBoundingBox3D(const QList<QVector4D> &vertexes = QList<QVector4D>());
    BBAABBBoundingBox3D(float px, float py, float pz,
                        float rx, float ry, float rz,
                        float sx, float sy, float sz,
                        const QList<QVector4D> &vertexes = QList<QVector4D>());
    BBAABBBoundingBox3D(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale,
                        const QVector3D &center, const QVector3D &halfLength);

    bool computeIntersectWithPlane(const QVector3D &point, const QVector3D &normal);

private:
    void computeBoxVertexes(const QList<QVector4D> &vertexes) override;
    QVector3D getMax();
    QVector3D getMin();
};


#endif // BBBOUNDINGBOX_H
