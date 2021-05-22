#include "BBGameObjectSet.h"

BBGameObjectSet::BBGameObjectSet(const QList<BBGameObject*> &objects)
    : BBGameObjectSet(objects, QVector3D(0, 0, 0))
{
    // compute center of all objects
    QVector3D centerPos;
    for (int i = 0; i < objects.count(); i++)
    {
        centerPos += objects.at(i)->getPosition();
    }
    centerPos /= objects.count();
    BBGameObject::setPosition(centerPos);
}

BBGameObjectSet::BBGameObjectSet(const QList<BBGameObject*> &objects, const QVector3D &centerPos)
    : BBGameObject(centerPos, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_GameObjectSet = objects;
    for (int i = 0; i < objects.count(); i++)
    {
        // m_OriginalPosition is position before setRotation
        m_OriginalPositions.append(objects.at(i)->getPosition());
        m_OriginalScales.append(objects.at(i)->getScale());
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

}

void BBGameObjectSet::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{

}

void BBGameObjectSet::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{

}


//void CenterPoint::setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform)
//{
//    GameObject::setRotation(angle, axis);
//    int count = mGameObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        GameObject *object = mGameObjects.at(i);
//        //相对于中心点旋转
//        QMatrix4x4 matrix;
//        matrix.translate(mPosition);
//        matrix.rotate(mQuaternion);
//        matrix.scale(mScale);
//        matrix.translate(-mPosition);
//        //各对象旋转的角(变化值)与中心点的旋转角一致
//        object->setRotation(angle, axis, isUpdateLocalTransform);
//        //位置也会发生变化
//        object->setPosition(matrix * mOriginPosition.at(i), isUpdateLocalTransform);
//    }
//}

//void CenterPoint::setRotation(QVector3D rotation, bool isUpdateLocalTransform)
//{
//    //直接设置数值给选中的对象 集合旋转角不累计 注释掉下面一行
//    //GameObject::setRotation(rotation);
//    int count = mGameObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        GameObject *object = mGameObjects.at(i);
//        //相对于中心点旋转
//        //QMatrix4x4 matrix;
//        //matrix.translate(mPosition);
//        //matrix.rotate(mQuaternion);
//        //matrix.scale(mScale);
//        //matrix.translate(-mPosition);
//        //各对象旋转角(非变化值)直接设为指定值
//        object->setRotation(rotation, isUpdateLocalTransform);
//        //位置不发生变化
//        //object->setPosition(matrix * mOriginPosition.at(i));
//    }
//}

//void CenterPoint::setScale(QVector3D scale, bool isUpdateLocalTransform)
//{
//    GameObject::setScale(scale);
//    int count = mGameObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        GameObject *object = mGameObjects.at(i);
//        //相对于中心点缩放
//        QMatrix4x4 matrix;
//        matrix.translate(mPosition);
//        matrix.rotate(mQuaternion);
//        matrix.scale(mScale);
//        matrix.translate(-mPosition);
//        //各对象保持原来的缩放倍数关系
//        object->setScale(scale * mOriginScale.at(i), isUpdateLocalTransform);
//        //位置也会发生变化
//        object->setPosition(matrix * mOriginPosition.at(i), isUpdateLocalTransform);
//    }
//}
