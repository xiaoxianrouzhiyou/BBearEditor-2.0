#ifndef BBHIERARCHYTREEWIDGET_H
#define BBHIERARCHYTREEWIDGET_H

#include "BBTreeWidget.h"

class BBGameObject;
class BBHierarchyTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBHierarchyTreeWidget(QWidget *parent = 0);
    QString getMimeType() { return BB_MIMETYPE_HIERARCHYTREEWIDGET; }

private:
    void setMenu();
    bool moveItemFromOthers(const QMimeData *pMimeData) override;
    void moveItemToIndicator();
    QIcon getClassIcon(QString className);
    int getDragIconColumnIndex() override { return 1; }

signals:
    void createModel(QString filePath);

public slots:
    void addGameObjectItem(BBGameObject *pGameObject);


//    //保存每个item与其对应的GameObject的地址的映射
//    static QMap<QTreeWidgetItem*, GameObject*> mMap;

//public slots:
//    void itemDoubleClickedSlot(QTreeWidgetItem *item, int column);
//    void selectPickedObject(GameObject *gameObject);
//    void finishRename() override;
//    void deleteAction() override;
//    void cancelSelectedItems();

//private slots:
//    void itemChangedSlot(QTreeWidgetItem *item, int column);
//    void renameItemName(GameObject *gameObject);
//    void changeGameObjectActivation(GameObject *gameObject, bool isActive);
//    void copyGameObjectInsertMap(QTreeWidgetItem *item, GameObject *gameObject);
//    void itemSelectionChangedSlot();
//    void selectedObjects(QList<GameObject*> gameObjects);
//    void updateMultipleSelectObjects(GameObject *gameObject);

//signals:
//    void lookAtGameObjectSignal(GameObject *gameObject);
//    void setCoordinateSelectedObject(GameObject *gameObject);
//    void setCoordinateSelectedObjects(QList<GameObject*> gameObjects, CenterPoint *center);
//    void deleteSceneGameObject(GameObject *gameObject);
//    void showGameObjectProperty(GameObject *gameObject);
//    void showSetProperty(QList<GameObject*> gameObjects, CenterPoint *center);
//    void updateNameInInspector(GameObject *gameObject, QString newName);
//    void changeButtonActiveCheckStateInInspector(GameObject *gameObject, bool isActive);
//    void createLight(QString fileName);
//    void copyGameObject(GameObject *sourceObject, QTreeWidgetItem* transcript, QVector3D position);
//    void cancelFileListSelectedItems();

//private:
//    void deleteOne(QTreeWidgetItem *item) override;
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    bool moveItemFromFileList(const QMimeData *mimeData) override;
//    bool dragDropItem() override;
//    void focusInEvent(QFocusEvent *event) override;
};

#endif // BBHIERARCHYTREEWIDGET_H
