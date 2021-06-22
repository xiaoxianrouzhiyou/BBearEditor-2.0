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

    void init(const QString &path) override;
    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                 const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                 const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                 const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3) override;

private:
    BBRectBoundingBox2D *m_pBoundingBox2D;
};

#endif // BBICON_H
