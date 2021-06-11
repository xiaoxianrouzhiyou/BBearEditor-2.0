#ifndef BBSCENEMANAGER_H
#define BBSCENEMANAGER_H


#include <QMap>

class QTreeWidgetItem;
class BBGameObject;

class BBSceneManager
{
public:
    BBSceneManager();
    ~BBSceneManager();

    static void insertObjectMap(QTreeWidgetItem *pItem, BBGameObject *pGameObject);
    static void removeObjectMap(QTreeWidgetItem *pItem);
    static QTreeWidgetItem* getSceneTreeItem(BBGameObject *pGameObject);
    static QList<QTreeWidgetItem*> getSceneTreeItems(QList<BBGameObject*> gameObjects);
    static BBGameObject* getGameObject(QTreeWidgetItem *pItem);

private:
    // Save the mapping between each scene tree item and its corresponding GameObject
    static QMap<QTreeWidgetItem*, BBGameObject*> m_ObjectMap;
};

#endif // BBSCENEMANAGER_H
