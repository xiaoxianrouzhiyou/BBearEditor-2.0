#include "BBGameObject.h"
#include "Render/BBCamera.h"
#include "Scene/BBSceneManager.h"
#include "BBGameObjectSet.h"
#include <QTreeWidgetItem>


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

/**
 * @brief BBGameObject::BBGameObject            2D
 * @param x
 * @param y
 * @param nWidth
 * @param nHeight
 */
BBGameObject::BBGameObject(int x, int y, int nWidth, int nHeight)
{
    setSize(nWidth, nHeight);
    setScreenCoordinate(x, y);
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
    BBSceneManager::changeScene(this);
}

void BBGameObject::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    // Rotate the current quaternion
    QQuaternion rot = QQuaternion::fromAxisAndAngle(axis, nAngle);
    // Multiply to the original quaternion
    m_Quaternion = rot * m_Quaternion;
    // Turn to Euler Angle
    m_Rotation = m_Quaternion.toEulerAngles();

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
    BBSceneManager::changeScene(this);
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
    BBSceneManager::changeScene(this);
}

void BBGameObject::setRotation(const QQuaternion &quaternion, bool bUpdateLocalTransform)
{
    m_Quaternion = quaternion;
    m_Rotation = m_Quaternion.toEulerAngles();
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
    BBSceneManager::changeScene(this);
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
    BBSceneManager::changeScene(this);
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
    BBSceneManager::changeScene(this);
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
    BBSceneManager::changeScene(this);
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

void BBGameObject::render()
{

}

void BBGameObject::render(BBCamera *pCamera)
{
    Q_UNUSED(pCamera);
}

void BBGameObject::render(BBCanvas *pCanvas)
{
    Q_UNUSED(pCanvas);
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

void BBGameObject::insertInRenderQueue(BBRenderQueue *pQueue)
{
    Q_UNUSED(pQueue);
}

void BBGameObject::removeFromRenderQueue(BBRenderQueue *pQueue)
{
    Q_UNUSED(pQueue);
}

void BBGameObject::setCurrentMaterial(BBMaterial *pMaterial)
{
    Q_UNUSED(pMaterial);
}

void BBGameObject::setCurrentMaterial(int nExtraMaterialIndex)
{
    Q_UNUSED(nExtraMaterialIndex);
}

void BBGameObject::setExtraMaterial(int nMaterialIndex, BBMaterial *pMaterial)
{
    Q_UNUSED(nMaterialIndex);
    Q_UNUSED(pMaterial);
}

void BBGameObject::rollbackMaterial()
{

}

void BBGameObject::restoreMaterial()
{

}

void BBGameObject::setMatrix4(const std::string &uniformName, const float *pMatrix4)
{
    Q_UNUSED(uniformName);
    Q_UNUSED(pMatrix4);
}

void BBGameObject::setVector4(const std::string &uniformName, float x, float y, float z, float w)
{
    Q_UNUSED(uniformName);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(z);
    Q_UNUSED(w);
}

void BBGameObject::setTexture(const std::string &uniformName, GLuint textureName)
{
    Q_UNUSED(uniformName);
    Q_UNUSED(textureName);
}

void BBGameObject::openLight()
{

}

void BBGameObject::closeLight()
{

}

bool BBGameObject::hit(const BBRay &ray, float &fDistance)
{
    Q_UNUSED(ray);
    Q_UNUSED(fDistance);
    return m_bActive;
}

bool BBGameObject::hit(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    return m_bActive;
}

bool BBGameObject::belongToSelectionRegion(const BBFrustum &frustum)
{
    Q_UNUSED(frustum);
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

void BBGameObject::setScreenCoordinateWithSwitchingOriginalPoint(int x, int y)
{
    BBSceneManager::getCamera()->switchCoordinate(x, y);
    setScreenCoordinate(x, y);
}

void BBGameObject::setScreenCoordinate(int x, int y)
{
    setPosition(QVector3D(x, y, 0.0f));
}

void BBGameObject::setSize(int nWidth, int nHeight)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    setScale(QVector3D(m_nWidth, m_nHeight, 0));
}

void BBGameObject::translate(int nDeltaX, int nDeltaY)
{
    setScreenCoordinate(m_Position.x() + nDeltaX, m_Position.y() + nDeltaY);
}

void BBGameObject::setModelMatrix(float px, float py, float pz,
                                  const QQuaternion &r,
                                  float sx, float sy, float sz)
{
    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(px, py, pz);
    m_ModelMatrix.rotate(r);
    m_ModelMatrix.scale(sx, sy, sz);

    m_ViewModelMatrix_IT = (BBSceneManager::getCamera()->getViewMatrix() * m_ModelMatrix).inverted().transposed();
}
