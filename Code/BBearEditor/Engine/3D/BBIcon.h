#ifndef BBICON_H
#define BBICON_H


#include "Base/BBRenderableObject.h"

class BBRectBoundingBox2D;

class BBIcon : public BBRenderableObject
{
public:
    BBIcon();
    BBIcon(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    ~BBIcon();

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(const QQuaternion &quaternion, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void init(const QString &path) override;
    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const BBFrustum &frustum) override;

private:
    BBRectBoundingBox2D *m_pBoundingBox2D;
};

#endif // BBICON_H
