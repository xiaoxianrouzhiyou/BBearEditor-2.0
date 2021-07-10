#include "BBGameObjectSet.h"

BBGameObjectSet::BBGameObjectSet(const QList<BBGameObject*> &objects)
    : BBGameObjectSet(objects, QVector3D(0, 0, 0))
{
    // compute center of all objects
    QVector3D centerPos;
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        centerPos += m_GameObjectSet.at(i)->getPosition();
    }
    centerPos /= m_GameObjectSet.count();
    BBGameObject::setPosition(centerPos);
}

BBGameObjectSet::BBGameObjectSet(const QList<BBGameObject*> &objects, const QVector3D &centerPos)
    : BBGameObject(centerPos, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_GameObjectSet = filterSelectedObjects(objects);
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        // m_OriginalPosition is position before setRotation
        m_OriginalPositions.append(m_GameObjectSet.at(i)->getPosition());
        m_OriginalScales.append(m_GameObjectSet.at(i)->getScale());
    }
}

void BBGameObjectSet::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    QVector3D displacement = position - m_Position;
    // compute the new position of each object
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        BBGameObject *pObject = m_GameObjectSet.at(i);
        pObject->setPosition(pObject->getPosition() + displacement, bUpdateLocalTransform);

        m_OriginalPositions[i] = m_OriginalPositions[i] + displacement;
    }

    BBGameObject::setPosition(position, bUpdateLocalTransform);
}

void BBGameObjectSet::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis);
    // Relative to center point
    QMatrix4x4 matrix;
    matrix.translate(m_Position);
    matrix.rotate(m_Quaternion);
    matrix.scale(m_Scale);
    matrix.translate(-m_Position);
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        BBGameObject *pObject = m_GameObjectSet.at(i);
        pObject->setRotation(nAngle, axis, bUpdateLocalTransform);
        // change position at the same time
        pObject->setPosition(matrix * m_OriginalPositions.at(i), bUpdateLocalTransform);
    }
}

void BBGameObjectSet::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    // Set the value directly
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        BBGameObject *pObject = m_GameObjectSet.at(i);
        pObject->setRotation(rotation, bUpdateLocalTransform);
        // do not change position
    }
}

void BBGameObjectSet::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBGameObject::setScale(scale);
    // Relative to center point
    QMatrix4x4 matrix;
    matrix.translate(m_Position);
    matrix.rotate(m_Quaternion);
    matrix.scale(m_Scale);
    matrix.translate(-m_Position);
    for (int i = 0; i < m_GameObjectSet.count(); i++)
    {
        BBGameObject *pObject = m_GameObjectSet.at(i);
        pObject->setScale(scale * m_OriginalScales.at(i), bUpdateLocalTransform);
        // change position at the same time
        pObject->setPosition(matrix * m_OriginalPositions.at(i), bUpdateLocalTransform);
    }
}

QList<BBGameObject*> BBGameObjectSet::filterSelectedObjects(const QList<BBGameObject*> &gameObjects)
{
    QList<BBGameObject*> objectSet2D;
    QList<BBGameObject*> objectSet3D;
    int count = gameObjects.count();
    for (int i = 0; i < count; i++)
    {
        BBGameObject *pObject = gameObjects[i];
        if (pObject->getClassName() == BB_CLASSNAME_CANVAS)
        {
            objectSet2D.append(pObject);
        }
        else
        {
            objectSet3D.append(pObject);
        }
    }
    // When there are UI and 3D objects at the same time, remove the UI
    if (objectSet3D.count() > 0)
    {
        return objectSet3D;
    }
    else
    {
        return objectSet2D;
    }
}
