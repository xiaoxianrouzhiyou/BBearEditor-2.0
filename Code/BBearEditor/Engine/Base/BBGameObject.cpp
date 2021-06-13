#include "BBGameObject.h"
#include "Render/BBCamera.h"
#include "Scene/BBSceneManager.h"
#include "BBGameObjectSet.h"


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

    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(this);
    // Some objects are not managed by BBHierarchyTreeWidget
    if (pItem)
    {
        // handle children
        for (int i = 0; i < pItem->childCount(); i++)
        {
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pItem->child(i));
            // the localTransform of child is not changed, bUpdateLocalTransform = false
            pGameObject->setPosition(pGameObject->getPosition() + displacement, false);
        }

        // set local
        if (bUpdateLocalTransform)
        {
            m_LocalPosition = m_LocalPosition + displacement;
        }
    }
    BBSceneManager::changeScene();
}

void BBGameObject::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    // Rotate the current quaternion
    QQuaternion rot = QQuaternion::fromAxisAndAngle(axis, nAngle);
    // Multiply to the original quaternion
    m_Quaternion = rot * m_Quaternion;
    // Turn to Euler Angle
    m_Rotation = m_Quaternion.toEulerAngles();
    // test
//    float temp = round(m_Rotation.x());
//    if (temp - m_Rotation.x() < 0.001)
//        m_Rotation.setX(temp);
//    temp = round(m_Rotation.y());
//    if (temp - m_Rotation.y() < 0.001)
//        m_Rotation.setY(temp);
//    temp = round(m_Rotation.z());
//    if (temp - m_Rotation.z() < 0.001)
//        m_Rotation.setZ(temp);
    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());

    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(this);
    // Some objects are not managed by BBHierarchyTreeWidget
    if (pItem)
    {
        // handle children
        // push all children into set
        QList<BBGameObject*> gameObjects;
        for (int i = 0; i < pItem->childCount(); i++)
        {
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pItem->child(i));
            gameObjects.append(pGameObject);
        }
        // The center point is not the center of all objects, but the position of the parent
        BBGameObjectSet *pSet = new BBGameObjectSet(gameObjects, m_Position);
        // the localTransform of child is not changed, bUpdateLocalTransform = false
        pSet->setRotation(nAngle, axis, false);

        if (bUpdateLocalTransform)
        {
            m_LocalQuaternion = rot * m_LocalQuaternion;
            m_LocalRotation = m_LocalQuaternion.toEulerAngles();
        }
    }
    BBSceneManager::changeScene();
}

void BBGameObject::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    // handle itself
    m_Rotation = rotation;
    m_Quaternion = QQuaternion::fromEulerAngles(m_Rotation);
    setModelMatrix(m_Position.x(), m_Position.y(), m_Position.z(),
                   m_Quaternion,
                   m_Scale.x(), m_Scale.y(), m_Scale.z());

    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(this);
    // Some objects are not managed by BBHierarchyTreeWidget
    if (pItem)
    {
        for (int i = 0; i < pItem->childCount(); i++)
        {
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pItem->child(i));
            // The transform matrix of the child object relative to the parent object
            QMatrix4x4 localMatrix;
            localMatrix.translate(pGameObject->getLocalPosition());
            localMatrix.rotate(pGameObject->getLocalQuaternion());
            localMatrix.scale(pGameObject->getLocalScale());

            QMatrix4x4 globalMatrix = m_ModelMatrix * localMatrix;
            pGameObject->setPosition(globalMatrix * QVector3D(0, 0, 0), false);
            pGameObject->setRotation((m_Quaternion * pGameObject->getLocalQuaternion()).toEulerAngles(), false);
        }

        if (bUpdateLocalTransform)
        {
            QTreeWidgetItem *pParent = pItem->parent();
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pParent);
            setLocalTransform(pGameObject);
        }
    }
    BBSceneManager::changeScene();
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

    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(this);
    // Some objects are not managed by BBHierarchyTreeWidget
    if (pItem)
    {
        for (int i = 0; i < pItem->childCount(); i++)
        {
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pItem->child(i));
            // The transform matrix of the child object relative to the parent object
            QMatrix4x4 localMatrix;
            localMatrix.translate(pGameObject->getLocalPosition());
            localMatrix.rotate(pGameObject->getLocalQuaternion());
            localMatrix.scale(pGameObject->getLocalScale());

            QMatrix4x4 globalMatrix = m_ModelMatrix * localMatrix;
            pGameObject->setPosition(globalMatrix * QVector3D(0, 0, 0), false);
            pGameObject->setScale(m_Scale * pGameObject->getLocalScale(), false);
        }

        if (bUpdateLocalTransform)
        {
            QTreeWidgetItem *pParent = pItem->parent();
            if (pParent)
            {
                BBGameObject *pGameObject = BBSceneManager::getGameObject(pParent);
                m_LocalScale = m_Scale / pGameObject->getScale();
            }
            else
            {
                // at the top level
                m_LocalScale = m_Scale;
            }
        }
    }
    BBSceneManager::changeScene();
}

void BBGameObject::setLocalTransform(BBGameObject *pParent)
{
    if (pParent)
    {
        m_LocalPosition = m_Position - pParent->getPosition();
        m_LocalScale = m_Scale / pParent->getScale();
        QMatrix4x4 localMatrix = pParent->getModelMatrix().inverted() * m_ModelMatrix;
        // remove scale from the matrix that includes rotation and scale
        localMatrix.scale(QVector3D(1, 1, 1) / m_LocalScale);
        // 3x3 in the top-left is rotation matrix
        QMatrix3x3 rotMatrix;
        rotMatrix(0, 0) = localMatrix.data()[0];
        rotMatrix(1, 0) = localMatrix.data()[1];
        rotMatrix(2, 0) = localMatrix.data()[2];
        rotMatrix(0, 1) = localMatrix.data()[4];
        rotMatrix(1, 1) = localMatrix.data()[5];
        rotMatrix(2, 1) = localMatrix.data()[6];
        rotMatrix(0, 2) = localMatrix.data()[8];
        rotMatrix(1, 2) = localMatrix.data()[9];
        rotMatrix(2, 2) = localMatrix.data()[10];
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
    BBSceneManager::changeScene();
}

void BBGameObject::setVisibility(bool bVisible)
{
    m_bVisible = bVisible;
    // handle children
    QTreeWidgetItem *pItem = BBSceneManager::getSceneTreeItem(this);
    // Some objects are not managed by BBHierarchyTreeWidget
    if (pItem)
    {
        for (int i = 0; i < pItem->childCount(); i++)
        {
            BBGameObject *pGameObject = BBSceneManager::getGameObject(pItem->child(i));
            if (pGameObject)
                pGameObject->setVisibility(bVisible);
        }
    }
    BBSceneManager::changeScene();
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
    m_strFilePath = path;
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
    Q_UNUSED(left1);
    Q_UNUSED(left2);
    Q_UNUSED(left3);
    Q_UNUSED(top1);
    Q_UNUSED(top2);
    Q_UNUSED(top3);
    Q_UNUSED(right1);
    Q_UNUSED(right2);
    Q_UNUSED(right3);
    Q_UNUSED(bottom1);
    Q_UNUSED(bottom2);
    Q_UNUSED(bottom3);
    // 4 planes, object in the middle of top bottom left right planes is selected
    // Whether the bounding box of object is placed in the middle of 4 planes
    // Eliminate objects whose the center point of the bounding box is on the outside
    // If the center point of the bounding box is inside,
    // further calculate whether each vertex of the bounding box is inside
    return true;
}

void BBGameObject::showCloseUp(QVector3D &outPosition, QVector3D &outViewCenter, float fDistFactor)
{
    Q_UNUSED(fDistFactor);
    outViewCenter = m_Position + QVector3D(0, 0.5, 0);
    outPosition = outViewCenter + QVector3D(0, 0.5, 4);
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
