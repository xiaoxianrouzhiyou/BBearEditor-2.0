#include "BBHierarchyTreeWidget.h"
#include <QMimeData>
#include <QMenu>

//QMap<QTreeWidgetItem*, GameObject*> HierarchyTree::mMap;

BBHierarchyTreeWidget::BBHierarchyTreeWidget(QWidget *parent)
    : BBTreeWidget(parent)
{
    QStringList list;
    list.push_back("Label");
    list.push_back("Type");
    setHeaderLabels(list);
    // Column width of the first column
    setColumnWidth(0, 170);

//    QObject::connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
//                     this, SLOT(itemDoubleClickedSlot(QTreeWidgetItem*, int)));
//    QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
//                     this, SLOT(itemChangedSlot(QTreeWidgetItem*, int)));
//    QObject::connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChangedSlot()));

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
//    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
//    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
//    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openEditor()));
//    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
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

    //移动到指示器指示处
    if (m_pIndicatorItem)
    {
//        QTreeWidgetItem *parent = NULL;
//        //位置的索引值
//        int index;
//        if (indicatorPos == IndicatorPos::RECT)
//        {
//            //成为indicatorItem的孩子
//            parent = indicatorItem;
//            index = 0;
//        }
//        else if (indicatorPos == IndicatorPos::TOP)
//        {
//            //成为indicatorItem的兄弟
//            parent = indicatorItem->parent();
//            if (parent)
//                index = parent->indexOfChild(indicatorItem);
//            else
//                index = indexOfTopLevelItem(indicatorItem);
//        }
//        else//indicatorPos == IndicatorPos::BOTTOM
//        {
//            //成为indicatorItem的兄弟
//            parent = indicatorItem->parent();
//            if (parent)
//                index = parent->indexOfChild(indicatorItem) + 1;
//            else
//                index = indexOfTopLevelItem(indicatorItem) + 1;
//        }
//        //最后一个结点从树中移除
//        takeTopLevelItem(topLevelItemCount() - 1);
//        //结点添加在新的位置
//        if (parent)
//        {
//            parent->insertChild(index, pItem);
//            GameObject *obj = mMap.value(pItem);
//            //成为新父节点的子节点 修改相对坐标
//            obj->setLocalTransform();
//            //场景中的对象移动到parent处
//            obj->setPosition(mMap.value(parent)->getPosition());
//        }
//        else
//        {
//            //成为top结点
//            insertTopLevelItem(index, pItem);
//        }
//        //选中新结点
//        setCurrentItem(pItem);
//        //展开parent 新的结点可见
//        if (parent)
//        {
//            setItemExpanded(parent, true);
//        }
    }
    // if m_pIndicatorItem is NULL, means adding to the end. No need to move
}


//QIcon HierarchyTree::getClassIcon(QString className)
//{
//    return QIcon(":/icon/resources/icons/" + className + ".png");
//}

//void HierarchyTree::addGameObjectSlot(GameObject *gameObject)
//{
//    QTreeWidgetItem* item = new QTreeWidgetItem({gameObject->getName(), gameObject->getClassName()});
//    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable
//                   | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
//    //被选中 表示active
//    if (gameObject->getActive())
//        item->setCheckState(0, Qt::Checked);
//    else
//        item->setCheckState(0, Qt::Unchecked);
//    item->setIcon(1, getClassIcon(gameObject->getIconName()));
//    addTopLevelItem(item);
//    mMap.insert(item, gameObject);
//    //设置相对坐标
//    gameObject->setLocalTransform();
//    //选中新增item 这句一定要在插入map之后
//    //否则current变化触发的槽函数itemSelectionChangedSlot将在map中找不到对应的gameObject
//    setCurrentItem(item);
//}

//void HierarchyTree::itemDoubleClickedSlot(QTreeWidgetItem *item, int column)
//{
//    Q_UNUSED(column);
//    lookAtGameObjectSignal(mMap.value(item));
//}

//void HierarchyTree::deleteAction()
//{
//    //不需要确认对话框
//    //过滤选中项 父亲孩子同时选中 只处理父亲
//    filterSelectedItems();
//    //遍历所选项
//    QList<QTreeWidgetItem*> items = selectedItems();
//    if (items.count() == 0)
//        return;
//    //真正开始删除
//    for (int i = 0; i < items.count(); i++)
//    {
//        BaseTree::deleteAction(items.at(i));
//    }
//    setCurrentItem(NULL);
//}

////递归 删除场景中的gameobj 删除item及其孩子与gameobj的映射
//void HierarchyTree::deleteOne(QTreeWidgetItem *item)
//{
//    GameObject *object = mMap.value(item);
//    //如果是模型对象 其使用的材质的使用者列表 需要除去该对象
//    if (object->getClassName() == ModelClassName)
//    {
//        Model *model = (Model*) object;
//        model->materialRemoveUser();
//    }

//    deleteSceneGameObject(object);
//    mMap.remove(item);
//    BaseTree::deleteOne(item);
//}

//void HierarchyTree::pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript)
//{
//    //场景中拷贝同样的对象 并插入映射
//    //位置与父节点对象的位置一样
//    QVector3D position;
//    if (transcript->parent())
//    {
//        position = mMap.value(transcript->parent())->getPosition();
//    }
//    //top结点初始位置与源对象一样
//    else
//    {
//        position = mMap.value(source)->getPosition();
//    }
//    copyGameObject(mMap.value(source), transcript, position);
//}

//void HierarchyTree::copyGameObjectInsertMap(QTreeWidgetItem *item, GameObject *gameObject)
//{
//    //在场景中拷贝的对象 需要将树节点和对象插入映射中
//    mMap.insert(item, gameObject);
//    //设置相对坐标
//    gameObject->setLocalTransform();
//    //属性栏显示该项对应属性
//    setCurrentItem(item);
//    setItemSelected(item, true);
//}

//int HierarchyTree::getDragIconColumn()
//{
//    return 1;
//}

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

//bool HierarchyTree::dragDropItem()
//{
//    if (BaseTree::dragDropItem())
//    {
//        //成功落下 修改所有选中结点的相对坐标
//        QList<QTreeWidgetItem*> items = selectedItems();
//        int count = items.count();
//        for (int i = 0; i < count; i++)
//        {
//            mMap.value(items.at(i))->setLocalTransform();
//        }
//        //刷新一次属性栏 否则相对坐标显示的是改变父节点之前的值
//        itemSelectionChangedSlot();
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//void HierarchyTree::selectPickedObject(GameObject *gameObject)
//{
//    //如果gameobject为空 item也为空
//    setCurrentItem(mMap.key(gameObject));
//}

//void HierarchyTree::itemSelectionChangedSlot()
//{
//    QList<QTreeWidgetItem*> items = selectedItems();
//    int count = items.count();
//    if (count == 1)
//    {
//        //单选 显示对应属性 场景中选中对应对象
//        GameObject *gameObject = mMap.value(items.first());
//        setCoordinateSelectedObject(gameObject);
//        //属性栏显示该项对应属性
//        showGameObjectProperty(gameObject);
//    }
//    else
//    {
//        if (count == 0)
//        {
//            //不显示对应属性
//            showGameObjectProperty(NULL);
//            //不选对象 场景中坐标系也不选择
//            setCoordinateSelectedObject(NULL);
//        }
//        else
//        {
//            QList<GameObject*> gameObjects;
//            for (int i = 0; i < count; i++)
//            {
//                //父子结点都被选中 多滤掉孩子节点对应的对象
//                QTreeWidgetItem *parent;
//                for (parent = items.at(i)->parent(); parent; parent = parent->parent())
//                {
//                    //遍历祖先 看是否也被选中
//                    if (items.contains(parent))
//                    {
//                        //祖先和子孙同时被选中 只处理祖先
//                        //处理祖先时 会对所有子孙进行处理
//                        break;
//                    }
//                }
//                //选中项对应的gameobj加入对象集合
//                if (parent == NULL)
//                {
//                    //祖先没有被选中 该对象需要加入列表进行处理
//                    gameObjects.append(mMap.value(items.at(i)));
//                }
//            }
//            count = gameObjects.count();
//            if (count == 1)
//            {
//                //过滤后 真正处理的只有一个对象
//                GameObject *gameObject = gameObjects.first();
//                setCoordinateSelectedObject(gameObject);
//                //属性栏显示该项对应属性
//                showGameObjectProperty(gameObject);
//            }
//            else
//            {
//                //真正处理多个对象 坐标系选中所有对象的重心
//                CenterPoint *center = new CenterPoint(gameObjects);
//                center->setBaseAttributes("multiple", "Set", "set");
//                //所有对象都是不可见 center为不可见图标
//                center->setActive(false);

//                for (int i = 0; i < count; i++)
//                {
//                    if (gameObjects.at(i)->getActive())
//                    {
//                        center->setActive(true);
//                        break;
//                    }
//                }
//                setCoordinateSelectedObjects(gameObjects, center);
//                //属性栏显示集合的transform
//                showSetProperty(gameObjects, center);
//            }
//        }
//    }
//}

//void HierarchyTree::selectedObjects(QList<GameObject*> gameObjects)
//{
//    QMap<QTreeWidgetItem*, GameObject*>::Iterator itr;
//    for (itr = mMap.begin(); itr != mMap.end(); itr++)
//    {
//        GameObject *gameObject = itr.value();
//        QTreeWidgetItem *item = itr.key();
//        if (gameObjects.contains(gameObject))
//        {
//            item->setSelected(true);
//        }
//        else
//        {
//            item->setSelected(false);
//        }
//    }
//}

//void HierarchyTree::updateMultipleSelectObjects(GameObject *gameObject)
//{
//    //得到对象对应的树节点 没被选中就将其选中 已经选中 则取消选中
//    QTreeWidgetItem *item = mMap.key(gameObject);
//    if (item->isSelected())
//    {
//        item->setSelected(false);
//        //操作对象包围盒 指示器等隐藏 点击对象会使包围盒显示
//        //如果结点的父节点被选中 包围盒还是要显示
//        QList<QTreeWidgetItem*> items = selectedItems();
//        QTreeWidgetItem *parent;
//        for (parent = item->parent(); parent; parent = parent->parent())
//        {
//            //遍历祖先 看是否也被选中
//            if (items.contains(parent))
//            {
//                //祖先也被选中 包围盒还是显示
//                break;
//            }
//        }
//        if (parent == NULL)
//        {
//            gameObject->setVisible(false);
//        }
//    }
//    else
//    {
//        item->setSelected(true);
//    }
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
