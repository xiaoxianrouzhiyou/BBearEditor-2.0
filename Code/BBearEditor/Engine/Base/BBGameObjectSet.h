#ifndef BBGAMEOBJECTSET_H
#define BBGAMEOBJECTSET_H


#include "BBGameObject.h"


// manage a set of BBGameObject
class BBGameObjectSet : public BBGameObject
{
public:
    BBGameObjectSet(const QList<BBGameObject*> &objects);
    BBGameObjectSet(const QList<BBGameObject*> &objects, const QVector3D &centerPos);

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

private:
    QList<BBGameObject*> m_GameObjectSet;
    QList<QVector3D> m_OriginalPositions;
    QList<QVector3D> m_OriginalScales;
};

#endif // BBGAMEOBJECTSET_H
