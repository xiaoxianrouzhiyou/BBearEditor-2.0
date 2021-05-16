#include "BBGameObject.h"
#include "BBCamera.h"

BBGameObject::BBGameObject()
    : BBGameObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBGameObject::BBGameObject(QVector3D position, QVector3D rotation, QVector3D scale)
    : BBGameObject(position.x(), position.y(), position.z(),
                   rotation.x(), rotation.y(), rotation.z(),
                      scale.x(),    scale.y(),    scale.z())
{

}

BBGameObject::BBGameObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
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

void BBGameObject::setPosition(QVector3D position, bool bUpdateLocalTransform)
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

void BBGameObject::setRotation(int nAngle, QVector3D axis, bool bUpdateLocalTransform)
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

void BBGameObject::setRotation(QVector3D rotation, bool bUpdateLocalTransform)
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

void BBGameObject::setScale(QVector3D scale, bool bUpdateLocalTransform)
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

void BBGameObject::setBaseAttributes(QString name, QString className, QString iconName, bool bActive)
{
    m_strName = name;
    m_strClassName = className;
    m_strIconName = iconName;
    m_bActive = bActive;
}

void BBGameObject::init()
{

}

void BBGameObject::init(const QString path)
{
    Q_UNUSED(path);
}

void BBGameObject::render(BBCamera *pCamera)
{
    Q_UNUSED(pCamera);
}

void BBGameObject::render(QMatrix4x4 modelMatrix, BBCamera *pCamera)
{
    Q_UNUSED(modelMatrix);
    Q_UNUSED(pCamera);
}

void BBGameObject::resize(float fWidth, float fHeight)
{
    Q_UNUSED(fWidth);
    Q_UNUSED(fHeight);
}

bool BBGameObject::hit(BBRay ray, float &fDistance)
{
    Q_UNUSED(ray);
    Q_UNUSED(fDistance);
    return false;
}

void BBGameObject::setModelMatrix(float px, float py, float pz, QQuaternion r, float sx, float sy, float sz)
{
    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(px, py, pz);
    m_ModelMatrix.rotate(r);
    m_ModelMatrix.scale(sx, sy, sz);
}


//void GameObject::setLocalTransform()
//{
//    //不能在构造函数中设置 因为创建对象时 还没有创建树节点 找不到父对象 在插入映射后调用
//    //得到对应的树节点
//    QTreeWidgetItem *item = HierarchyTree::mMap.key(this);
//    if (item)
//    {
//        //如果是层级视图管理的结点
//        QTreeWidgetItem *parent = item->parent();
//        if (parent)
//        {
//            //如果有父节点 求出相对于父节点的坐标
//            //父节点对应的对象
//            GameObject *gameObject = HierarchyTree::mMap.value(parent);
//            mLocalPosition = mPosition - gameObject->getPosition();
//            //子节点的缩放值是父节点的缩放值乘以子节点相对于父节点的缩放值
//            mLocalScale = mScale / gameObject->getScale();
//            //相对矩阵
//            QMatrix4x4 relativeMatrix = gameObject->getModelMatrix().inverted() * getModelMatrix();
//            //缩放矩阵和旋转矩阵叠加 去掉缩放变换
//            relativeMatrix.scale(QVector3D(1, 1, 1) / mLocalScale);
//            //左上的3x3是旋转矩阵
//            QMatrix3x3 rotMatrix;
//            rotMatrix(0, 0) = relativeMatrix.data()[0];
//            rotMatrix(1, 0) = relativeMatrix.data()[1];
//            rotMatrix(2, 0) = relativeMatrix.data()[2];
//            rotMatrix(0, 1) = relativeMatrix.data()[4];
//            rotMatrix(1, 1) = relativeMatrix.data()[5];
//            rotMatrix(2, 1) = relativeMatrix.data()[6];
//            rotMatrix(0, 2) = relativeMatrix.data()[8];
//            rotMatrix(1, 2) = relativeMatrix.data()[9];
//            rotMatrix(2, 2) = relativeMatrix.data()[10];
//            mLocalQuaternion = QQuaternion::fromRotationMatrix(rotMatrix);
//            mLocalRotation = mLocalQuaternion.toEulerAngles();
//        }
//        else
//        {
//            //top结点 相对坐标就是全局坐标
//            mLocalPosition = mPosition;
//            mLocalRotation = mRotation;
//            mLocalQuaternion = mQuaternion;
//            mLocalScale = mScale;
//        }
//    }
//}



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

//bool GameObject::belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                         QVector3D top1, QVector3D top2, QVector3D top3,
//                                         QVector3D right1, QVector3D right2, QVector3D right3,
//                                         QVector3D bottom1, QVector3D bottom2, QVector3D bottom3)
//{
//    //计算对象的包围盒是否位于选区的上下左右四个平面之中
//    //包围盒中心点在外侧的直接排除
//    //包围盒中心点在内测的 进一步计算包围盒的每一个顶点是否在内测
//    return true;
//}



////---------------------------------CenterPoint


//CenterPoint::CenterPoint(QList<GameObject*> objects)
//    : GameObject(), mGameObjects(objects)
//{
//    //中心点为所有对象的中点
//    QVector3D pos;
//    int count = objects.count();
//    for (int i = 0; i < count; i++)
//    {
//        pos += objects.at(i)->getPosition();
//        //保存各对象的初始位置 用于旋转时求新位置 移动操作时 位置需要更新
//        mOriginPosition.append(objects.at(i)->getPosition());
//        //保存最初的缩放倍数关系
//        mOriginScale.append(objects.at(i)->getScale());
//    }
//    pos /= count;
//    GameObject::setPosition(pos);
//}

//CenterPoint::CenterPoint(QList<GameObject*> objects, QVector3D centerPos)
//    : GameObject(centerPos, QVector3D(0, 0, 0), QVector3D(1, 1, 1)), mGameObjects(objects)
//{
//    int count = objects.count();
//    for (int i = 0; i < count; i++)
//    {
//        //保存各对象的初始位置 用于旋转时求新位置 移动操作时 位置需要更新
//        mOriginPosition.append(objects.at(i)->getPosition());
//        //保存最初的缩放倍数关系
//        mOriginScale.append(objects.at(i)->getScale());
//    }
//}

//void CenterPoint::setPosition(QVector3D position, bool isUpdateLocalTransform)
//{
//    //原来的位置和新位置 求出位移
//    QVector3D displacement = position - mPosition;
//    //算出每个对象的新位置
//    int count = mGameObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        GameObject *object = mGameObjects.at(i);
//        object->setPosition(object->getPosition() + displacement, isUpdateLocalTransform);
//        //移动操作时 位置需要更新
//        //平移后旋转前的位置 否则按照刷新的位置和累积的旋转角算出的旋转后的位置会出错
//        mOriginPosition[i] = mOriginPosition[i] + displacement;
//    }
//    //修改中心点的位置
//    GameObject::setPosition(position, isUpdateLocalTransform);
//    //清空旋转角 否则按照刷新的位置和累积的旋转角算出的旋转后的位置会出错
//    //GameObject::setRotation(QVector3D(0, 0, 0));
//}

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
