#include "BBSceneManager.h"
#include "Serializer/BBScene.pb.h"
#include <QTreeWidgetItem>
#include "BBUtils.h"
#include "Render/BBEditViewOpenGLWidget.h"
#include "BBScene.h"


QMap<QTreeWidgetItem*, BBGameObject*> BBSceneManager::m_ObjectMap;
QString BBSceneManager::m_CurrentSceneFilePath;
BBEditViewOpenGLWidget* BBSceneManager::m_pEditViewOpenGLWidget = NULL;


void BBSceneManager::insertObjectMap(QTreeWidgetItem *pItem, BBGameObject *pGameObject)
{
    m_ObjectMap.insert(pItem, pGameObject);
}

void BBSceneManager::removeObjectMap(QTreeWidgetItem *pItem)
{
    m_ObjectMap.remove(pItem);
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

bool BBSceneManager::isSceneChanged()
{
    // to do
    return true;
}

void BBSceneManager::openScene(const QString &filePath)
{
    // clear the last opened scene
    removeScene();

    int nLength = -1;
    char *pData = BBUtils::loadFileContent(filePath.toStdString().c_str(), nLength);
    BB_PROCESS_ERROR_RETURN(pData);

    BBSerializer::BBScene scene;
    scene.ParseFromString(pData);

    // load scene + tree
    int count = scene.gameobject_size();
    for (int i = 0; i < count; i++)
    {
        m_pEditViewOpenGLWidget->createModel(scene.gameobject(i));
    }
    // reconstruct the parent connect of the tree

    BB_SAFE_DELETE(pData);

    m_CurrentSceneFilePath = filePath;
}

void BBSceneManager::saveScene(const QString &filePath)
{
    BBSerializer::BBScene scene;

    // for finding index of children
    QList<QTreeWidgetItem*> items;
    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        items.append(it.key());
    }

    int index = 0;
    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        QTreeWidgetItem *pKey = it.key();
        BBSerializer::BBHierarchyTreeWidgetItem *pItem = scene.add_item();
        pItem->set_index(index);
        for (int i = 0; i < pKey->childCount(); i++)
        {
            int childIndex = items.indexOf(pKey->child(i));
            pItem->add_children(childIndex);
        }

        BBSerializer::BBGameObject *pGameObject = scene.add_gameobject();
        pGameObject->set_index(index);

        index++;
    }

    int nLength = scene.ByteSizeLong() + 1;
    char szBuffer[nLength] = {0};
    scene.SerializeToArray(szBuffer, nLength);
    BBUtils::saveToFile(filePath.toStdString().c_str(), szBuffer, nLength);

    m_CurrentSceneFilePath.clear();
}

void BBSceneManager::removeScene()
{
    BB_PROCESS_ERROR_RETURN(m_pEditViewOpenGLWidget);
    m_pEditViewOpenGLWidget->getScene()->clear();
    for (QMap<QTreeWidgetItem*, BBGameObject*>::Iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
    {
        delete it.key();
    }
    m_ObjectMap.clear();
    m_CurrentSceneFilePath.clear();
}
