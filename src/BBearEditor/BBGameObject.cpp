#include "BBGameObject.h"
#include "BBCamera.h"

BBGameObject::BBGameObject()
    : BBGameObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBGameObject::BBGameObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBGameObject(position.x(), position.y(), position.z(),
                   rotation.x(), rotation.y(), rotation.z(),
                      scale.x(),    scale.y(),    scale.z())
{

}

BBGameObject::BBGameObject(float px, float py, float pz,
                           float rx, float ry, float rz,
                           float sx, float sy, float sz)
{
    m_Position        = QVector3D(px, py, pz);
    m_Rotation        = QVector3D(rx, ry, rz);
    m_Scale           = QVector3D(sx, sy, sz);
    m_Quaternion      = QQuaternion::fromEulerAngles(m_Rotation);
    m_LocalPosition   = m_Position;
    m_LocalRotation   = m_Rotation;
    m_LocalScale      = m_Scale;
    m_LocalQuaternion = m_Quaternion;
    setModelMatrix(px, py, pz, m_Quaternion, sx, sy, sz);
    m_bActive = true;

    // 零时
    m_strName = "name";
    m_strClassName = "class name";
    m_strIconName = "model";
}

void BBGameObject::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    QVector3D displacement = position - m_Position;
    m_Position = position;
    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());
//    QTreeWidgetItem *pItem = HierarchyTree::mMap.key(this);
//    //有些对象并不是层级视图管理的对象
//    if (pItem)
//    {
//        //设置孩子结点
//        int count = item->childCount();
//        for (int i = 0; i < count; i++)
//        {
//            GameObject *gameObject = HierarchyTree::mMap.value(item->child(i));
//            //移动父节点 其孩子节点相对于父节点的坐标不变 isUpdateLocalTransform为false
//            gameObject->setPosition(gameObject->getPosition() + displacement, false);
//        }
//        //设置相对坐标
//        if (isUpdateLocalTransform)
//        {
//            mLocalPosition = mLocalPosition + displacement;
//        }
//    }
}

void BBGameObject::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    // Rotate the current quaternion
    QQuaternion rot = QQuaternion::fromAxisAndAngle(axis, nAngle);
    // Multiply to the original quaternion
    m_Quaternion = rot * m_Quaternion;
    // Turn to Euler Angle
    m_Rotation = m_Quaternion.toEulerAngles();

    //test
//    float temp = round(mRotation.x());
//    if (temp - mRotation.x() < 0.001)
//        mRotation.setX(temp);
//    temp = round(mRotation.y());
//    if (temp - mRotation.y() < 0.001)
//        mRotation.setY(temp);
//    temp = round(mRotation.z());
//    if (temp - mRotation.z() < 0.001)
//        mRotation.setZ(temp);

    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());

//    //处理孩子结点
//    QTreeWidgetItem *item = HierarchyTree::mMap.key(this);
//    //有些对象并不是层级视图管理的对象
//    if (item)
//    {
//        //所有孩子结点作为集合
//        QList<GameObject*> gameObjects;
//        int count = item->childCount();
//        for (int i = 0; i < count; i++)
//        {
//            GameObject *gameObject = HierarchyTree::mMap.value(item->child(i));
//            gameObjects.append(gameObject);
//        }
//        //中心点不是所有对象的中心 而是父节点的位置
//        CenterPoint *center = new CenterPoint(gameObjects, mPosition);
//        //子节点的相对坐标不变
//        center->setRotation(angle, axis, false);
//        //自身的相对坐标
//        if (isUpdateLocalTransform)
//        {
//            mLocalQuaternion = rot * mLocalQuaternion;
//            mLocalRotation = mLocalQuaternion.toEulerAngles();
//        }
//    }
}

void BBGameObject::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    // handle itself
    m_Rotation = rotation;
    m_Quaternion = QQuaternion::fromEulerAngles(m_Rotation);
    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());

//    //处理孩子结点
//    QTreeWidgetItem *item = HierarchyTree::mMap.key(this);
//    //有些对象并不是层级视图管理的对象
//    if (item)
//    {
//        //子节点改变绝对坐标 不改变相对坐标
//        int count = item->childCount();
//        for (int i = 0; i < count; i++)
//        {
//            GameObject *childObject = HierarchyTree::mMap.value(item->child(i));
//            //子对象相对于父对象的变换矩阵
//            QMatrix4x4 relativeMatrix;
//            relativeMatrix.translate(childObject->getLocalPosition());
//            relativeMatrix.rotate(childObject->getLocalQuaternion());
//            relativeMatrix.scale(childObject->getLocalScale());
//            //在父对象变换矩阵的基础上得到子对象的变换矩阵
//            QMatrix4x4 childMatrix = getModelMatrix() * relativeMatrix;
//            childObject->setPosition(childMatrix * QVector3D(0, 0, 0), false);
//            //绝对旋转为父节点的绝对四元数*子节点的相对四元数
//            QVector3D gameObjectRotation = (mQuaternion * childObject->getLocalQuaternion()).toEulerAngles();
//            childObject->setRotation(gameObjectRotation, false);
//            //旋转变换 不会改变孩子结点的缩放值
//        }
//        //自己的相对坐标
//        if (isUpdateLocalTransform)
//        {
//            QTreeWidgetItem *parent = item->parent();
//            if (parent)
//            {
//                //如果有父节点 求出相对于父节点的坐标
//                //父节点对应的对象
//                GameObject *gameObject = HierarchyTree::mMap.value(parent);
//                //相对矩阵
//                QMatrix4x4 relativeMatrix = gameObject->getModelMatrix().inverted() * getModelMatrix();
//                //缩放矩阵和旋转矩阵叠加 去掉缩放变换
//                relativeMatrix.scale(QVector3D(1, 1, 1) / mLocalScale);
//                //左上的3x3是旋转矩阵
//                QMatrix3x3 rotMatrix;
//                rotMatrix(0, 0) = relativeMatrix.data()[0];
//                rotMatrix(1, 0) = relativeMatrix.data()[1];
//                rotMatrix(2, 0) = relativeMatrix.data()[2];
//                rotMatrix(0, 1) = relativeMatrix.data()[4];
//                rotMatrix(1, 1) = relativeMatrix.data()[5];
//                rotMatrix(2, 1) = relativeMatrix.data()[6];
//                rotMatrix(0, 2) = relativeMatrix.data()[8];
//                rotMatrix(1, 2) = relativeMatrix.data()[9];
//                rotMatrix(2, 2) = relativeMatrix.data()[10];
//                mLocalQuaternion = QQuaternion::fromRotationMatrix(rotMatrix);
//                mLocalRotation = mLocalQuaternion.toEulerAngles();
//            }
//            else
//            {
//                //top结点 相对坐标就是全局坐标
//                mLocalRotation = mRotation;
//                mLocalQuaternion = mQuaternion;
//            }
//        }
//    }

}

void BBGameObject::setScale(float scale, bool bUpdateLocalTransform)
{
    setScale(QVector3D(scale, scale, scale), bUpdateLocalTransform);
}

void BBGameObject::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    // handle itself
    m_Scale = scale;
    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());

//    //处理孩子结点
//    QTreeWidgetItem *item = HierarchyTree::mMap.key(this);
//    //有些对象并不是层级视图管理的对象
//    if (item)
//    {
//        int count = item->childCount();
//        for (int i = 0; i < count; i++)
//        {
//            GameObject *childObject = HierarchyTree::mMap.value(item->child(i));
//            //子对象相对于父对象的变换矩阵
//            QMatrix4x4 relativeMatrix;
//            relativeMatrix.translate(childObject->getLocalPosition());
//            relativeMatrix.rotate(childObject->getLocalQuaternion());
//            relativeMatrix.scale(childObject->getLocalScale());
//            //在新的父对象变换矩阵的基础上得到子对象的变换矩阵
//            QMatrix4x4 childMatrix = getModelMatrix() * relativeMatrix;
//            childObject->setPosition(childMatrix * QVector3D(0, 0, 0), false);
//            //缩放变换不会改变旋转值
//            //子节点的缩放值是父节点的缩放值乘以子节点相对于父节点的缩放值
//            childObject->setScale(mScale * childObject->getLocalScale(), false);
//        }
//        //自身的相对缩放值
//        if (isUpdateLocalTransform)
//        {
//            QTreeWidgetItem *parent = item->parent();
//            if (parent)
//            {
//                //如果有父节点 求出相对于父节点的坐标
//                //父节点对应的对象
//                GameObject *gameObject = HierarchyTree::mMap.value(parent);
//                //子节点的缩放值是父节点的缩放值乘以子节点相对于父节点的缩放值
//                mLocalScale = mScale / gameObject->getScale();
//            }
//            else
//            {
//                //top结点 相对坐标就是全局坐标
//                mLocalScale = mScale;
//            }
//        }
//    }
}

void BBGameObject::setLocalTransform(BBGameObject *pParent)
{
    if (pParent)
    {
        m_LocalPosition = m_Position - pParent->getPosition();
        m_LocalScale = m_Scale / pParent->getScale();
        QMatrix4x4 relativeMatrix = pParent->getModelMatrix().inverted() * m_ModelMatrix;
        // remove scale from the matrix that includes rotation and scale
        relativeMatrix.scale(QVector3D(1, 1, 1) / m_LocalScale);
        // 3x3 in the top-left is rotation matrix
        QMatrix3x3 rotMatrix;
        rotMatrix(0, 0) = relativeMatrix.data()[0];
        rotMatrix(1, 0) = relativeMatrix.data()[1];
        rotMatrix(2, 0) = relativeMatrix.data()[2];
        rotMatrix(0, 1) = relativeMatrix.data()[4];
        rotMatrix(1, 1) = relativeMatrix.data()[5];
        rotMatrix(2, 1) = relativeMatrix.data()[6];
        rotMatrix(0, 2) = relativeMatrix.data()[8];
        rotMatrix(1, 2) = relativeMatrix.data()[9];
        rotMatrix(2, 2) = relativeMatrix.data()[10];
        m_LocalQuaternion = QQuaternion::fromRotationMatrix(rotMatrix);
        m_LocalRotation = m_LocalQuaternion.toEulerAngles();
    }
    else
    {
        //at the top level, local = global
        m_LocalPosition = m_Position;
        m_LocalRotation = m_Rotation;
        m_LocalQuaternion = m_Quaternion;
        m_LocalScale = m_Scale;
    }
}

void BBGameObject::setBaseAttributes(const QString &name, const QString &className, const QString &iconName, bool bActive)
{
    m_strName = name;
    m_strClassName = className;
    m_strIconName = iconName;
    m_bActive = bActive;
}

void BBGameObject::init()
{

}

void BBGameObject::init(const QString &path)
{
    Q_UNUSED(path);
}

void BBGameObject::render(BBCamera *pCamera)
{
    Q_UNUSED(pCamera);
}

void BBGameObject::render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera)
{
    Q_UNUSED(modelMatrix);
    Q_UNUSED(pCamera);
}

void BBGameObject::resize(float fWidth, float fHeight)
{
    Q_UNUSED(fWidth);
    Q_UNUSED(fHeight);
}

bool BBGameObject::hit(const BBRay &ray, float &fDistance)
{
    Q_UNUSED(ray);
    Q_UNUSED(fDistance);
    return m_bActive;
}

bool BBGameObject::belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                           const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                           const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                           const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3)
{
    // 4 planes, object in the middle of top bottom left right planes is selected
    // Whether the bounding box of object is placed in the middle of 4 planes
    // Eliminate objects whose the center point of the bounding box is on the outside
    // If the center point of the bounding box is inside,
    // further calculate whether each vertex of the bounding box is inside
    return true;
}

void BBGameObject::setModelMatrix(float px, float py, float pz,
                                  const QQuaternion &r,
                                  float sx, float sy, float sz)
{
    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(px, py, pz);
    m_ModelMatrix.rotate(r);
    m_ModelMatrix.scale(sx, sy, sz);
}






//void GameObject::setVisible(bool isVisible)
//{
//    //孩子对象的可见性
//    QTreeWidgetItem *item = HierarchyTree::mMap.key(this);
//    if (item)
//    {
//        int count = item->childCount();
//        for (int i = 0; i < count; i++)
//        {
//            HierarchyTree::mMap.value(item->child(i))->setVisible(isVisible);
//        }
//    }
//}


//void GameObject::lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor)
//{
//    Q_UNUSED(distFactor);
//    viewCenter = mPosition + QVector3D(0, 0.5, 0);
//    pos = viewCenter + QVector3D(0, 0.5, 4);
//}
