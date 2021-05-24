#include "BBHierarchyTreeWidget.h"
#include <QMimeData>
#include <QMenu>
#include "BBGameObject.h"
#include "BBGameObjectSet.h"


QMap<QTreeWidgetItem*, BBGameObject*> BBHierarchyTreeWidget::m_ObjectMap;

BBHierarchyTreeWidget::BBHierarchyTreeWidget(QWidget *parent)
    : BBTreeWidget(parent)
{
    QStringList list;
    list.push_back("Label");
    list.push_back("Type");
    setHeaderLabels(list);
    // Column width of the first column
    setColumnWidth(0, 280);

    QObject::connect(this, SIGNAL(itemSelectionChanged()),
                     this, SLOT(changeSelectedItems()));
    QObject::connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
                     this, SLOT(doubleClickItem(QTreeWidgetItem*, int)));
//    QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
//                     this, SLOT(itemChangedSlot(QTreeWidgetItem*, int)));

    // Popup menu
    setMenu();
}

void BBHierarchyTreeWidget::setMenu()
{
    m_pMenu = new QMenu(this);
    QAction *pActionUndo = new QAction(tr("Undo"));
    pActionUndo->setShortcut(QKeySequence(tr("Ctrl+Z")));
    QAction *pActionRedo = new QAction(tr("Redo"));
    pActionRedo->setShortcut(QKeySequence(tr("Shift+Ctrl+Z")));
    QAction *pActionCopy = new QAction(tr("Copy"));
    pActionCopy->setShortcut(QKeySequence(tr("Ctrl+C")));
    QAction *pActionPaste = new QAction(tr("Paste"));
    pActionPaste->setShortcut(QKeySequence(tr("Ctrl+V")));
    QAction *pActionRename = new QAction(tr("Rename"));
#if defined(Q_OS_WIN32)
    pActionRename->setShortcut(Qt::Key_F2);
#elif defined(Q_OS_MAC)
    pActionRename->setShortcut(Qt::Key_Return);
#endif
    QAction *pActionDelete = new QAction(tr("Delete"));
    pActionDelete->setShortcut(QKeySequence(tr("Ctrl+D")));

    m_pMenu->addAction(pActionUndo);
    m_pMenu->addAction(pActionRedo);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionCopy);
    m_pMenu->addAction(pActionPaste);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionRename);
    m_pMenu->addAction(pActionDelete);

    // connect the trigger event
    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openRenameEditor()));
    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

void BBHierarchyTreeWidget::pasteOne(QTreeWidgetItem *pSource, QTreeWidgetItem* pTranscript)
{
    // copy GameObject in the scene, and insert map
    copyGameObject(m_ObjectMap.value(pSource), pTranscript);
    // handle children
    for (int i = 0; i < pSource->childCount(); i++)
    {
        pasteOne(pSource->child(i), pTranscript->child(i));
    }
}

void BBHierarchyTreeWidget::deleteOne(QTreeWidgetItem *pItem)
{
    // remove corresponding gameobject and map
    BBGameObject *pGameObject = m_ObjectMap.value(pItem);

//    //如果是模型对象 其使用的材质的使用者列表 需要除去该对象
//    if (object->getClassName() == ModelClassName)
//    {
//        Model *model = (Model*) object;
//        model->materialRemoveUser();
//    }

    m_ObjectMap.remove(pItem);
    BBTreeWidget::deleteOne(pItem);
    // cannot delete pGameObject before deleting treeItem
    // because when treeitem changes, removing coordinate system will be triggered, which needs pGameObject
    deleteGameObject(pGameObject);
}

bool BBHierarchyTreeWidget::moveItem()
{
    if (BBTreeWidget::moveItem())
    {
        // successful drop, change local transform of all selected items
        QList<QTreeWidgetItem*> items = selectedItems();
        for (int i = 0; i < items.count(); i++)
        {
            QTreeWidgetItem *pItem = items.at(i);
            BBGameObject *pParent = m_ObjectMap.value(pItem->parent());
            m_ObjectMap.value(pItem)->setLocalTransform(pParent);
        }
        // update the property manager
        // otherwise, the local coordinates is the previous value that is relative to previous parent
        changeSelectedItems();
        return true;
    }
    else
    {
        return false;
    }
}

bool BBHierarchyTreeWidget::moveItemFromOthers(const QMimeData *pMimeData)
{
    QByteArray data;
    if ((data = pMimeData->data(BB_MIMETYPE_BASEOBJECT)) != nullptr)
    {
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        QString filePath;
        dataStream >> filePath;
        if (filePath == BB_PATH_TERRAIN)
        {
            // create terrain
//            createModel(filePath);
        }
        else
        {
            filePath = BB_PATH_RESOURCE_MESH + filePath;
            createModel(filePath);
        }
    }
//    else if ((data = mimeData->data("light")) != nullptr)
//    {
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString fileName;
//        dataStream >> fileName;
//        createLight(fileName);
//    }
    else
    {
        return false;
    }
    moveItemToIndicator();
    return true;
}

void BBHierarchyTreeWidget::moveItemToIndicator()
{
    // Objects dragged from the file list or prefab list
    // The default item created is the last one at the top level
    QTreeWidgetItem *pItem = topLevelItem(topLevelItemCount() - 1);

    // move to m_pIndicatorItem
    if (m_pIndicatorItem)
    {
        // pos
        int index = -1;
        QTreeWidgetItem *pParent = getParentOfMovingItem(index);
        // remove item that needs to move
        takeTopLevelItem(topLevelItemCount() - 1);
        // change local coordinate
        BBGameObject *pGameObject = m_ObjectMap.value(pItem);
        if (!pGameObject)
            return;
        BBGameObject *pParentGameObject = m_ObjectMap.value(pParent);
        pGameObject->setLocalTransform(pParentGameObject);
        // add in the new position
        if (pParent)
        {
            pParent->insertChild(index, pItem);
        }
        else
        {
            insertTopLevelItem(index, pItem);
        }
        // focus on
        setCurrentItem(pItem);
        // expand parent, show new item
        if (pParent)
        {
            setItemExpanded(pParent, true);
        }
    }
    // if m_pIndicatorItem is NULL, means adding to the end. No need to move
}

QIcon BBHierarchyTreeWidget::getClassIcon(const QString &className)
{
    return QIcon(BB_PATH_RESOURCE_ICON + className + ".png");
}

void BBHierarchyTreeWidget::addGameObject(BBGameObject *pGameObject)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem({pGameObject->getName(), pGameObject->getClassName()});
    pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable
                    | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

    // Checked means active
    if (pGameObject->getActivity())
        pItem->setCheckState(0, Qt::Checked);
    else
        pItem->setCheckState(0, Qt::Unchecked);

    pItem->setIcon(1, getClassIcon(pGameObject->getIconName()));

    addTopLevelItem(pItem);
    m_ObjectMap.insert(pItem, pGameObject);
    // parent = NULL, since pItem is in the top level
    pGameObject->setLocalTransform(NULL);
    // setCurrentItem needs to be placed in back of insert map
    // otherwise, when currentItem is changed and trigger changeSelectedItems that cannot find pGameObject in map
    setCurrentItem(pItem);
}

void BBHierarchyTreeWidget::addGameObject(BBGameObject *pGameObject, QTreeWidgetItem *pItem)
{
    m_ObjectMap.insert(pItem, pGameObject);

    BBGameObject *pParent = m_ObjectMap.value(pItem->parent());
    pGameObject->setLocalTransform(pParent);

    // show property in inspector
    setItemSelected(pItem, true);
    setItemExpanded(pItem, true);
    changeSelectedItems();
}

void BBHierarchyTreeWidget::selectPickedItem(BBGameObject *pGameObject)
{
    if (pGameObject == NULL)
    {
        setCurrentItem(NULL);
    }
    else
    {
        setCurrentItem(m_ObjectMap.key(pGameObject));
    }
}

void BBHierarchyTreeWidget::selectPickedItems(QList<BBGameObject*> gameObjects)
{
    QMap<QTreeWidgetItem*, BBGameObject*>::Iterator itr;
    for (itr = m_ObjectMap.begin(); itr != m_ObjectMap.end(); itr++)
    {
        QTreeWidgetItem *pItem = itr.key();
        BBGameObject *pGameObject = itr.value();
        if (gameObjects.contains(pGameObject))
        {
            pItem->setSelected(true);
        }
        else
        {
            pItem->setSelected(false);
        }
    }
}

void BBHierarchyTreeWidget::updateMultipleSelectedItems(BBGameObject *pGameObject)
{
    // get item corresponding object
    QTreeWidgetItem *pItem = m_ObjectMap.key(pGameObject);
    if (pItem->isSelected())
    {
        // If it is already selected, uncheck it
        pItem->setSelected(false);
        // show indicator and bounding box ...
        // If the parent of the item is selected, the bounding box of the item still needs to be displayed
        QList<QTreeWidgetItem*> items = selectedItems();
        QTreeWidgetItem *parent = NULL;
        for (parent = pItem->parent(); parent; parent = parent->parent())
        {
            if (items.contains(parent))
            {
                break;
            }
        }
        if (parent == NULL)
        {
            // all itself and its parent are not selected
            pGameObject->setVisibility(false);
        }
    }
    else
    {
        // If it is not selected, select it
        pItem->setSelected(true);
    }
}

void BBHierarchyTreeWidget::changeSelectedItems()
{
    QList<QTreeWidgetItem*> items = selectedItems();
    int count = items.count();
    if (count == 0)
    {
        // do not select object in OpenGL view
        setCoordinateSystemSelectedObject(NULL);
        // do not show properties in inspector
        showGameObjectProperty(NULL);
    }
    else if (count == 1)
    {
        // single selection
        BBGameObject *pGameObject = m_ObjectMap.value(items.first());
        // select corresponding object in OpenGL view
        setCoordinateSystemSelectedObject(pGameObject);
        // show properties in inspector
        showGameObjectProperty(pGameObject);
    }
    else
    {
        QList<BBGameObject*> gameObjects;
        for (int i = 0; i < count; i++)
        {
            // Ancestors and descendants are selected at the same time, only ancestors are processed
            QTreeWidgetItem *parent;
            for (parent = items.at(i)->parent(); parent; parent = parent->parent())
            {
                // Traverse ancestors to see if they are also selected
                if (items.contains(parent))
                {
                    break;
                }
            }
            if (parent == NULL)
            {
                gameObjects.append(m_ObjectMap.value(items.at(i)));
            }
            // if there is parent, the item can be processed when its parent is processed
        }
        count = gameObjects.count();
        if (count == 1)
        {
            // after filter, there is just an item that needs to be processed
            BBGameObject *pGameObject = gameObjects.first();
            setCoordinateSystemSelectedObject(pGameObject);
            // show properties in inspector
            showGameObjectProperty(pGameObject);
        }
        else
        {
            // The center of gravity of all objects selected in the coordinate system
            BBGameObjectSet *pSet = new BBGameObjectSet(gameObjects);
            pSet->setBaseAttributes("multiple", "Set", "set");
            // When all objects are invisible, pSet shows an invisible icon
            pSet->setActivity(false);
            for (int i = 0; i < count; i++)
            {
                if (gameObjects.at(i)->getActivity())
                {
                    pSet->setActivity(true);
                    break;
                }
            }
            setCoordinateSystemSelectedObjects(gameObjects, pSet);
            // show properties of the set in inspector
//            showSetProperty(gameObjects, pSet);
        }
    }
}

void BBHierarchyTreeWidget::doubleClickItem(QTreeWidgetItem *pItem, int nColumn)
{
    Q_UNUSED(nColumn);
    lookAtGameObject(m_ObjectMap.value(pItem));
}

void BBHierarchyTreeWidget::deleteAction()
{
    // no need to show BBConfirmationDialog
    filterSelectedItems();
    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count() == 0)
        return;

    for (int i = 0; i < items.count(); i++)
    {
        BBTreeWidget::deleteAction(items.at(i));
    }
    setCurrentItem(NULL);
}



//bool HierarchyTree::moveItemFromFileList(const QMimeData *mimeData)
//{
//    //只接受网格文件
//    QByteArray data = mimeData->data(FileList::getMimeType());
//    QDataStream dataStream(&data, QIODevice::ReadOnly);
//    QString filePath;
//    dataStream >> filePath;
//    QString suffix = filePath.mid(filePath.lastIndexOf('.') + 1);
//    if (FileList::meshSuffixs.contains(suffix))
//    {
//        createModel(filePath);
//    }
//    else
//    {
//        return false;
//    }
//    //新建的树节点根据指示器位置放置
//    moveItemToIndicator();
//    return true;
//}

//void HierarchyTree::cancelSelectedItems()
//{
//    setCurrentItem(NULL);
//}

//void HierarchyTree::finishRename()
//{
//    if (editingItem == NULL)
//        return;

//    //是否对名字进行了修改
//    //新名字不为空
//    QString name = edit->text();
//    QString preName = editingItem->text(0);
//    if (preName != name && !name.isEmpty())
//    {
//        //修改对应GameObject的名字
//        GameObject *gameObject = mMap.value(editingItem);
//        gameObject->setName(name);
//        //更新属性栏
//        updateNameInInspector(gameObject, name);
//        //item重命名
//        editingItem->setText(0, name);
//        //重命名后 如果在剪贴板中 移除掉
//        if (clipBoardItems.contains(editingItem))
//        {
//            clipBoardItems.removeOne(editingItem);
//        }
//    }

//    BaseTree::finishRename();
//}

//void HierarchyTree::itemChangedSlot(QTreeWidgetItem *item, int column)
//{
//    GameObject *gameObject = mMap.value(item);
//    bool isActive = item->checkState(column);
//    gameObject->setActive(isActive);
//    //属性栏的可见性按钮状态刷新
//    changeButtonActiveCheckStateInInspector(gameObject, isActive);
//    //选中当前项 只点击复选框 不会选中当前项
//    setCurrentItem(item);
//}

//void HierarchyTree::renameItemName(GameObject *gameObject)
//{
//    QTreeWidgetItem *item = mMap.key(gameObject);
//    item->setText(0, gameObject->getName());
//    //重命名后 如果在剪贴板中 移除掉
//    if (clipBoardItems.contains(item))
//    {
//        clipBoardItems.removeOne(item);
//    }
//}

//void HierarchyTree::changeGameObjectActivation(GameObject *gameObject, bool isActive)
//{
//    if (isActive)
//        mMap.key(gameObject)->setCheckState(0, Qt::Checked);
//    else
//        mMap.key(gameObject)->setCheckState(0, Qt::Unchecked);
//    //checkState修改会调用itemChangedSlot
//}

//void HierarchyTree::focusInEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    cancelFileListSelectedItems();
//}
