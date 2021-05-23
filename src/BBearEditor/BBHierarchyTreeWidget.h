#ifndef BBHIERARCHYTREEWIDGET_H
#define BBHIERARCHYTREEWIDGET_H

#include "BBTreeWidget.h"

class BBGameObject;
class BBGameObjectSet;
class BBHierarchyTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBHierarchyTreeWidget(QWidget *parent = 0);
    QString getMimeType() { return BB_MIMETYPE_HIERARCHYTREEWIDGET; }

    // Save the mapping between each item and its corresponding GameObject
    static QMap<QTreeWidgetItem*, BBGameObject*> m_ObjectMap;

private:
    void setMenu();
    void deleteOne(QTreeWidgetItem *pItem) override;

    bool moveItemFromOthers(const QMimeData *pMimeData) override;
    void moveItemToIndicator();
    QIcon getClassIcon(const QString &className);
    int getDragIconColumnIndex() override { return 1; }

signals:
    void createModel(const QString &filePath);
    void setCoordinateSystemSelectedObject(BBGameObject *pGameObject);
    void setCoordinateSystemSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet);
    void showGameObjectProperty(BBGameObject *pGameObject);
    void deleteGameObject(BBGameObject *pGameObject);

private slots:
    void addGameObject(BBGameObject *pGameObject);
    void selectPickedItem(BBGameObject *pGameObject);
    void selectPickedItems(QList<BBGameObject*> gameObjects);
    void updateMultipleSelectedItems(BBGameObject *pGameObject);
    void changeSelectedItems();


//public slots:
//    void itemDoubleClickedSlot(QTreeWidgetItem *item, int column);
//    void finishRename() override;
//    void deleteAction() override;
//    void cancelSelectedItems();

//private slots:
//    void itemChangedSlot(QTreeWidgetItem *item, int column);
//    void renameItemName(GameObject *gameObject);
//    void changeGameObjectActivation(GameObject *gameObject, bool isActive);
//    void copyGameObjectInsertMap(QTreeWidgetItem *item, GameObject *gameObject);
//    void updateMultipleSelectObjects(GameObject *gameObject);

//signals:
//    void lookAtGameObjectSignal(GameObject *gameObject);
//    void showSetProperty(QList<GameObject*> gameObjects, CenterPoint *center);
//    void updateNameInInspector(GameObject *gameObject, QString newName);
//    void changeButtonActiveCheckStateInInspector(GameObject *gameObject, bool isActive);
//    void createLight(QString fileName);
//    void copyGameObject(GameObject *sourceObject, QTreeWidgetItem* transcript, QVector3D position);
//    void cancelFileListSelectedItems();

//private:
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    bool moveItemFromFileList(const QMimeData *mimeData) override;
//    bool dragDropItem() override;
//    void focusInEvent(QFocusEvent *event) override;
};

#endif // BBHIERARCHYTREEWIDGET_H
