#include "BBSceneManager.h"
#include "Serializer/BBScene.pb.h"
#include <QTreeWidgetItem>
#include "Utils/BBUtils.h"
#include "Render/BBEditViewOpenGLWidget.h"
#include "BBScene.h"
#include "Base/BBGameObject.h"
#include "SceneManager/BBHierarchyTreeWidget.h"


QMap<QTreeWidgetItem*, BBGameObject*> BBSceneManager::m_ObjectMap;
QString BBSceneManager::m_CurrentSceneFilePath;
BBEditViewOpenGLWidget* BBSceneManager::m_pEditViewOpenGLWidget = NULL;
BBScene* BBSceneManager::m_pScene = NULL;
BBHierarchyTreeWidget* BBSceneManager::m_pHierarchyTreeWidget = NULL;
bool BBSceneManager::m_bSceneChanged = true;


void BBSceneManager::bindEditViewOpenGLWidget(BBEditViewOpenGLWidget *pWidget)
{
    m_pEditViewOpenGLWidget = pWidget;
    m_pScene = m_pEditViewOpenGLWidget->getScene();
}

void BBSceneManager::insertObjectMap(QTreeWidgetItem *pItem, BBGameObject *pGameObject)
{
    m_ObjectMap.insert(pItem, pGameObject);
    changeScene();
}

void BBSceneManager::removeObjectMap(QTreeWidgetItem *pItem)
{
    m_ObjectMap.remove(pItem);
    changeScene();
}

QTreeWidgetItem* BBSceneManager::getSceneTreeItem(BBGameObject *pGameObject)
{
    return m_ObjectMap.key(pGameObject);
}

QList<QTreeWidgetItem*> BBSceneManager::getSceneTreeItems(const QList<BBGameObject*> &gameObjects)
{
    QList<QTreeWidgetItem*> items;

    QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it;
    for (it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        QTreeWidgetItem *pItem = it.key();
        BBGameObject *pGameObject = it.value();
        if (gameObjects.contains(pGameObject))
        {
            items.append(pItem);
        }
    }

    return items;
}

BBGameObject* BBSceneManager::getGameObject(QTreeWidgetItem *pItem)
{
    return m_ObjectMap.value(pItem);
}

bool BBSceneManager::isSceneSwitched(const QString &filePath)
{
    if (m_CurrentSceneFilePath == filePath)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void BBSceneManager::changeScene(BBGameObject *pGameObject)
{
    if (!pGameObject
            || pGameObject->getClassName() == BB_CLASSNAME_MODEL
            || pGameObject->getClassName() == BB_CLASSNAME_DIRECTIONAL_LIGHT
            || pGameObject->getClassName() == BB_CLASSNAME_POINT_LIGHT
            || pGameObject->getClassName() == BB_CLASSNAME_SPOT_LIGHT)
    {
        BB_PROCESS_ERROR_RETURN(!m_bSceneChanged);
        m_bSceneChanged = true;
        m_pEditViewOpenGLWidget->updateEditViewTitle();
    }
}

void BBSceneManager::openScene(const QString &filePath)
{
    // clear the last opened scene
    removeScene();

    int nLength = -1;
    char *pData = BBUtils::loadFileContent(filePath.toStdString().c_str(), nLength);
    BB_PROCESS_ERROR_RETURN(pData);

    BBSerializer::BBScene scene;
    scene.ParseFromArray(pData, nLength);

    QList<QTreeWidgetItem*> items;

    // load scene + tree
    int count = scene.gameobject_size();
    for (int i = 0; i < count; i++)
    {
        BBSerializer::BBGameObject gameObject = scene.gameobject(i);
        QString className = QString::fromStdString(gameObject.classname());
        BBGameObject *pGameObject = NULL;
        if (className == BB_CLASSNAME_MODEL)
        {
            pGameObject = m_pEditViewOpenGLWidget->createModel(gameObject);
        }
        items.append(m_ObjectMap.key(pGameObject));
    }
    // reconstruct the parent connect of the tree
    m_pHierarchyTreeWidget->takeTopLevelItems();
    QList<QTreeWidgetItem*> topLevelItems;
    for (int i = 0; i < count; i++)
    {
        QTreeWidgetItem *pItem = items.at(i);
        int parentIndex = scene.item(i).parentindex();
        if (parentIndex < 0)
        {
            topLevelItems.append(pItem);
        }
        else
        {
            QTreeWidgetItem *pParent = items.at(parentIndex);
            pParent->addChild(pItem);
        }
    }
    m_pHierarchyTreeWidget->reconstruct(topLevelItems);

    BB_SAFE_DELETE(pData);
    m_CurrentSceneFilePath = filePath;
    m_pEditViewOpenGLWidget->updateEditViewTitle();
}

void BBSceneManager::saveScene(const QString &filePath)
{
    BBSerializer::BBScene scene;

    // for finding index of items
    QList<QTreeWidgetItem*> items;
    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        items.append(it.key());
    }

    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        QTreeWidgetItem *pKey = it.key();
        BBGameObject *pValue = it.value();

        BBSerializer::BBHierarchyTreeWidgetItem *pItem = scene.add_item();
        if (pKey->parent())
        {
            pItem->set_parentindex(items.indexOf(pKey->parent()));
        }
        else
        {
            pItem->set_parentindex(-1);
        }

        BBSerializer::BBGameObject *pGameObject = scene.add_gameobject();

        pGameObject->set_name(pValue->getName().toStdString().c_str());
        pGameObject->set_classname(pValue->getClassName().toStdString().c_str());
        pGameObject->set_filepath(pValue->getFilePath().toStdString().c_str());

        BBSerializer::BBVector3f *pPosition = pGameObject->mutable_position();
        setVector3f(pValue->getPosition(), pPosition);
        BBSerializer::BBVector3f *pLocalPosition = pGameObject->mutable_localposition();
        setVector3f(pValue->getLocalPosition(), pLocalPosition);

        BBSerializer::BBVector3f *pRotation = pGameObject->mutable_rotation();
        setVector3f(pValue->getRotation(), pRotation);
        BBSerializer::BBVector3f *pLocalRotation = pGameObject->mutable_localrotation();
        setVector3f(pValue->getLocalRotation(), pLocalRotation);

        BBSerializer::BBVector3f *pScale = pGameObject->mutable_scale();
        setVector3f(pValue->getScale(), pScale);
        BBSerializer::BBVector3f *pLocalScale = pGameObject->mutable_localscale();
        setVector3f(pValue->getLocalScale(), pLocalScale);
    }

    int nLength = scene.ByteSizeLong() + 1;
    char szBuffer[nLength] = {0};
    scene.SerializeToArray(szBuffer, nLength);
    BBUtils::saveToFile(filePath.toStdString().c_str(), szBuffer, nLength);

    m_bSceneChanged = false;
    m_CurrentSceneFilePath = filePath;
    m_pEditViewOpenGLWidget->updateEditViewTitle();
}

void BBSceneManager::removeScene()
{
    BB_PROCESS_ERROR_RETURN(m_pEditViewOpenGLWidget);
    m_pScene->clear();
    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        delete it.key();
    }
    m_ObjectMap.clear();

    m_bSceneChanged = false;
    m_CurrentSceneFilePath.clear();
}

void BBSceneManager::setVector3f(const QVector3D &value, BBSerializer::BBVector3f *&pOutVector3f)
{
    pOutVector3f->set_x(value.x());
    pOutVector3f->set_y(value.y());
    pOutVector3f->set_z(value.z());
}

