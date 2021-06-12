#include "BBSceneManager.h"


QMap<QTreeWidgetItem*, BBGameObject*> BBSceneManager::m_ObjectMap;
QString BBSceneManager::m_CurrentSceneFilePath;

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

void BBSceneManager::openScene(const QString &filePath)
{
    m_CurrentSceneFilePath = filePath;
}

void BBSceneManager::saveScene(const QString &filePath)
{
    m_CurrentSceneFilePath.clear();
}
