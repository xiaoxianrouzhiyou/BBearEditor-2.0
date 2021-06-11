#include "BBSceneManager.h"


QMap<QTreeWidgetItem*, BBGameObject*> BBSceneManager::m_ObjectMap;

BBSceneManager::BBSceneManager()
{

}

BBSceneManager::~BBSceneManager()
{

}

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

QList<QTreeWidgetItem*> BBSceneManager::getSceneTreeItems(QList<BBGameObject*> gameObjects)
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
