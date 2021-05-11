#include "BBTreeWidget.h"
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QDrag>

//#include <QPen>
//#include <QLine>
//#include <QDebug>
//#include <QApplication>
//#include <QDesktopWidget>

//#include <QScrollBar>
//#include <limits.h>
//#include <QTextItem>
//#include <QDropEvent>
//#include <QDragMoveEvent>
//#include <QPaintEvent>
//#include <QMenu>
//#include <QTime>


//--------------BBLineEdit


BBLineEdit::BBLineEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

void BBLineEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    finishEdit();
}

void BBLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        finishEdit();
    }
    else
    {
        QLineEdit::keyPressEvent(event);
    }
}



//--------------BBTreeWidget


BBTreeWidget::BBTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
//    editingItem = NULL;
    m_pIndicatorItem = NULL;
    m_pLastItem = NULL;
    m_eIndicatorPos = BBIndicatorPos::RECT;

    setDragEnabled(true);
    setAcceptDrops(true);
    // move, not copy
    setDefaultDropAction(Qt::MoveAction);
    // Internal drag, you cannot move to other trees, and you cannot move from other trees.
//    setDragDropMode(QAbstractItemView::InternalMove);
    // You can press shift and cmd to select multiple
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDropIndicatorShown(false);
}

BBTreeWidget::~BBTreeWidget()
{
    BB_SAFE_DELETE(m_pMenu);
}

void BBTreeWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    // When selecting ancestors and descendants at the same time, filter out descendants
    filterSelectedItems();

    // The path of each item is stored in mimeData
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    struct Info
    {
        int y;
        QPixmap icon;
        QString text;
    };
    QList<Info> infos;

    //用于计算最终拖拽图标的大小
    QFontMetrics fm = fontMetrics();
    int pixmapWidth = 0;
    int pixmapHeight = 0;

    QList<QTreeWidgetItem*> items = selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        // Relative path of each item
        QString levelPath = getLevelPath(items.at(i));
        dataStream << levelPath;
        Info info;
        QRect rect = visualItemRect(items.at(i));
        info.y = rect.top();
        // Final icon height, Accumulate
        pixmapHeight += rect.height();
        info.text = items.at(i)->text(0);
        int textWidth = fm.width(info.text);
        if (textWidth > pixmapWidth)
            pixmapWidth = textWidth;
        // The icon of the item
        info.icon = items.at(i)->icon(getDragIconColumnIndex()).pixmap(rect.height() * devicePixelRatio());
        info.icon.setDevicePixelRatio(devicePixelRatio());
        info.icon = info.icon.scaled(rect.height() * devicePixelRatio(), rect.height() * devicePixelRatio(),
                                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        // infos, inserting sort according to the upper and lower position relationship
        int j;
        for (j = 0; j < infos.count(); j++)
        {
            if (infos.at(j).y > info.y)
            {
                break;
            }
        }
        infos.insert(j, info);
    }

    QMimeData *pMimeData = new QMimeData;
    // Give this data a unique identifying tag
    pMimeData->setData(getMimeType(), itemData);

    int itemHeight = pixmapHeight / infos.count();
    // width, add icon and margin pixels
    QPixmap pixmap((pixmapWidth + itemHeight + 6) * devicePixelRatio(), pixmapHeight * devicePixelRatio());
    pixmap.setDevicePixelRatio(devicePixelRatio());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(QColor("#d6dfeb"));
    painter.setFont(QFont("Arial", 9));
    // Draw the icon and text of each item onto the final icon
    for (int i = 0; i < infos.count(); i++)
    {
        int y = itemHeight * i;
        painter.drawPixmap(0, y, infos.at(i).icon);
        painter.drawText(QRect(itemHeight + 6, y, pixmapWidth, itemHeight), Qt::AlignLeft | Qt::AlignVCenter, infos.at(i).text);
    }
    painter.end();

    QDrag drag(this);
    drag.setMimeData(pMimeData);
    drag.setPixmap(pixmap);
    drag.setHotSpot(QPoint(0, 0));
    drag.exec(Qt::MoveAction);
}

bool BBTreeWidget::moveItem()
{
    // drop target
    if (m_pIndicatorItem)
    {
        // drop position of moving item
        QTreeWidgetItem *pParent = NULL;
        // drop index of moving item
        int index = -1;
        if (m_eIndicatorPos == BBIndicatorPos::RECT)
        {
            // become the child of m_pIndicatorItem
            pParent = m_pIndicatorItem;
            // If add to the end, use parent->childCount()
            // But when item is dragged to its parent, deleting itself makes parent->childCount() decrease
            // add itself at the end is wrong
            // Add to the first
            index = 0;
        }
        else if (m_eIndicatorPos == BBIndicatorPos::TOP)
        {
            // become the front sibling of m_pIndicatorItem
            pParent = m_pIndicatorItem->parent();
            if (pParent)
                index = pParent->indexOfChild(m_pIndicatorItem);
            else
                index = indexOfTopLevelItem(m_pIndicatorItem);
        }
        else//m_eIndicatorPos == BBIndicatorPos::BOTTOM
        {
            // become the back sibling of m_pIndicatorItem
            pParent = m_pIndicatorItem->parent();
            if (pParent)
                index = pParent->indexOfChild(m_pIndicatorItem) + 1;
            else
                index = indexOfTopLevelItem(m_pIndicatorItem) + 1;
        }
        // The movable item has been filtered in startDrag
        QList<QTreeWidgetItem*> items = selectedItems();
        // Cannot move to an item that are moving and its descendants
        // parent and its ancestors cannot be included in items
        for (QTreeWidgetItem *pForeParent = pParent; pForeParent; pForeParent = pForeParent->parent())
        {
            if (items.contains(pForeParent))
            {
                return false;
            }
        }
        for (int i = 0; i < items.count(); i++)
        {
            QTreeWidgetItem *pItem = items.at(i);
            // remove item from tree
            int removeIndex = -1;
            QTreeWidgetItem *pRemoveItemParent = pItem->parent();
            QTreeWidgetItem *pIndicatorItemParent = m_pIndicatorItem->parent();
            if (pRemoveItemParent)
            {
                // the item is not in the top level
                removeIndex = pRemoveItemParent->indexOfChild(pItem);
                pRemoveItemParent->removeChild(pItem);
            }
            else
            {
                removeIndex = indexOfTopLevelItem(pItem);
                takeTopLevelItem(removeIndex);
            }
            // If the two are at the same level and the removed item is in front of the m_pIndicatorItem, index minus 1
            // pItem has been removed, so pItem->parent() cannot be used, so just use pRemoveItemParent
            if ((pRemoveItemParent == pIndicatorItemParent) && (removeIndex < index))
            {
                index--;
            }
            // add item in the drop position
            if (pParent)
            {
                pParent->insertChild(index, pItem);
            }
            else
            {
                // at the top level
                insertTopLevelItem(index, pItem);
            }
            // select a new item
            setItemSelected(pItem, true);
            // Insert the next item below
            // Otherwise 1 2 3 becomes 3 2 1 after inserting
            index++;
        }
        // expand parent to make new item visible
        if (pParent)
        {
            setItemExpanded(pParent, true);
        }
        return true;
    }
    return false;
}

bool BBTreeWidget::moveItemFromFileList(const QMimeData *pMimeData)
{
    Q_UNUSED(pMimeData);
    return true;
}

bool BBTreeWidget::moveItemFromOthers(const QMimeData *pMimeData)
{
    Q_UNUSED(pMimeData);
    return false;
}

void BBTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(getMimeType()))
    {
        // Receive internal drag
        event->accept();
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FILETREEWIDGET))
    {
        // Receive items dragged from the file list
        event->accept();
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_BASEOBJECT))
    {
        // Receive prefab
        event->accept();
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_LIGHTOBJECT))
    {
        // Receive light
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void BBTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
//    //当鼠标位于顶部或底部时向上或下滚动
//    QTreeWidget::dragMoveEvent(event);
//    QTreeWidgetItem *item = itemAt(event->pos());
//    //绘制将要被放置的位置
//    indicatorItem = item;
//    if (item)
//    {
//        if (item != mLastItem)
//        {
//            //移动的位置对应的item改变了
//            //重新记录新的item
//            mLastItem = item;
//            //重新记录当前的时间
//            mLastTime = QTime::currentTime();
//        }
//        else
//        {
//            //移动时所指的item没变
//            if (mLastTime.elapsed() > 1000)
//            {
//                //没变的时间超过一定量 展开该项
//                setItemExpanded(item, true);
//            }
//        }
//        int mouseY = event->pos().y();
//        int itemTop = visualItemRect(indicatorItem).top();
//        int itemBottom = visualItemRect(indicatorItem).bottom();
//        if (mouseY < itemTop + 2)
//        {
//            indicatorPos = IndicatorPos::TOP;
//        }
//        else if (mouseY <= itemBottom - 2)
//        {
//            //拖拽的鼠标落在该项的非边缘处 成为该项的孩子
//            indicatorPos = IndicatorPos::RECT;
//        }
//        else
//        {
//            indicatorPos = IndicatorPos::BOTTOM;
//        }
//    }
//    else
//    {
//        //没有所指的项 重置
//        mLastItem = NULL;
//    }
//    repaint();

    // do this in order to execute dropEvent
    event->accept();
}

void BBTreeWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(getMimeType()))
    {
        // Internal move item
        if (moveItem())
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FILETREEWIDGET))
    {
        // Receive items dropped from the file list
        if (moveItemFromFileList(event->mimeData()))
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        // Receive items dropped from others
        if (moveItemFromOthers(event->mimeData()))
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    // The indicator item is no longer needed after dragging
    m_pIndicatorItem = NULL;
    // Reset the currently suspended item after processing
    m_pLastItem = NULL;
    repaint();
}

void BBTreeWidget::filterSelectedItems()
{
    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count() > 1)
    {
        // Select multiple
        for (int i = 0; i < items.count(); i++)
        {
            for (QTreeWidgetItem *parent = items.at(i)->parent(); parent; parent = parent->parent())
            {
                // Traverse ancestors to see if they are also selected
                if (items.contains(parent))
                {
                    // Ancestors and descendants are selected at the same time, only ancestors are processed
                    setItemSelected(items.at(i), false);
                    break;
                }
            }
        }
    }
}

QString BBTreeWidget::getLevelPath(QTreeWidgetItem *pItem)
{
    // Hierarchical path of item
    QString location;
    for (QTreeWidgetItem *pParent = pItem; pParent; pParent = pParent->parent())
    {
        location = pParent->text(0) + "/" + location;
    }
    return location;
}



//void BaseTree::paintEvent(QPaintEvent *event)
//{
//    QTreeWidget::paintEvent(event);
//    //指着的item不是自己
//    if (indicatorItem /*&& indicatorItem != currentItem()*/)
//    {
//        //this参数 会报错
//        QPainter painter(viewport());
//        QPen pen(QColor("#d6dfeb"));
//        painter.setPen(pen);
//        QRect rect = visualItemRect(indicatorItem);
//        if (indicatorPos == IndicatorPos::RECT)
//        {
//            painter.drawRect(QRect(rect.topLeft() + QPoint(0, 1), rect.bottomRight() - QPoint(0, 1)));
//        }
//        else if (indicatorPos == IndicatorPos::TOP)
//        {
//            painter.drawLine(rect.topLeft() + QPoint(0, 1), rect.topRight() + QPoint(0, 1));
//        }
//        else
//        {
//            painter.drawLine(rect.bottomLeft(), rect.bottomRight());
//        }
//        painter.end();
//    }
//}

//void BaseTree::dragLeaveEvent(QDragLeaveEvent *event)
//{
//    Q_UNUSED(event);
//    //拖拽出去不再需要指示框
//    indicatorItem = NULL;
//    repaint();
//}

//void BaseTree::mousePressEvent(QMouseEvent *event)
//{
//    QTreeWidget::mousePressEvent(event);
//    if (event->buttons() & Qt::LeftButton)
//    {
//        //鼠标点击的位置没有item 去除选中项
//        QTreeWidgetItem *item = itemAt(event->pos());
//        if (!item)
//        {
//            setCurrentItem(NULL);
//        }
//    }
//}

//void BaseTree::contextMenuEvent(QContextMenuEvent *event)
//{
//    Q_UNUSED(event);
//    menu->show();
//    QPoint pos = cursor().pos();
//    //默认菜单显示在鼠标的右下方 如果水平超出了屏幕 菜单右侧和屏幕右侧对齐
//    if (pos.x() + menu->width() > QApplication::desktop()->width())
//    {
//        pos.setX(QApplication::desktop()->width() - menu->width());
//    }
//    //当垂直超出屏幕时 显示在右上 4为底边留白
//    if (pos.y() + menu->height() > QApplication::desktop()->height() - 4)
//    {
//        pos.setY(pos.y() - menu->height());
//    }
//    menu->move(pos);
//}

//void BaseTree::openEditor()
//{
//    QList<QTreeWidgetItem*> selected = selectedItems();
//    if (selected.count() == 1)
//    {
//        //多选或不选都不会进行重命名操作
//        editingItem = selected.first();
//        edit = new LineEdit;
//        edit->setStyleSheet("height: 16px; border: none; background: #d6dfeb; color: #191f28;"
//                            "selection-color: #d6dfeb; selection-background-color: #8193bc;"
//                            "font: 10pt \"Arial\"; padding: 0px 5px;");
//        //margin-left: 22px;
//        edit->setText(editingItem->text(0));
//        edit->selectAll();
//        QObject::connect(edit, SIGNAL(editingFinished()), this, SLOT(finishRename()));
//        setItemWidget(editingItem, 0, edit);
//        //set widget 使之显示之后 设置焦点才有作用
//        edit->setFocus();
//    }
//}

//void BaseTree::finishRename()
//{
//    removeItemWidget(editingItem, 0);
//    //delete edit;会出错
//    editingItem = NULL;
//    //否则列表没有焦点 不再相应按键事件
//    setFocus();
//}

//void BaseTree::deleteAction()
//{
//    //过滤选中项 父亲孩子同时选中 只处理父亲
//    filterSelectedItems();
//    //遍历所选项
//    QList<QTreeWidgetItem*> items = selectedItems();
//    if (items.count() == 0)
//        return;
//    //弹出对话框
//    ConfirmationDialog *dialog = new ConfirmationDialog;
//    dialog->setTitle("Delete selected?");
//    if (items.count() == 1)
//    {
//        dialog->setMessage("You cannot undo this action.\n\nAre you sure to delete this?");
//    }
//    else
//    {
//        dialog->setMessage("You cannot undo this action.\n\nAre you sure to delete these "
//                           + QString::number(items.count()) + " items?");
//    }
//    if (dialog->exec())
//    {
//        //真正开始删除
//        for (int i = 0; i < items.count(); i++)
//        {
//            QTreeWidgetItem* item = items.at(i);
//            deleteAction(item);
//        }
//        setCurrentItem(NULL);
//    }
//}

//void BaseTree::deleteAction(QTreeWidgetItem *item)
//{
//    //孩子删去childCount()会减少 用for循环出错
//    while (item->childCount() > 0)
//    {
//        QTreeWidgetItem* child = item->child(0);
//        deleteAction(child);
//    }
//    deleteOne(item);
//}

//void BaseTree::deleteOne(QTreeWidgetItem *item)
//{
//    //如果该项在剪贴板中 删去
//    if (clipBoardItems.contains(item))
//    {
//        clipBoardItems.removeOne(item);
//    }
//    //删去树节点
//    delete item;
//}

//void BaseTree::copyAction()
//{
//    //复制树节点
//    //清空上一次剪贴板存下的复制内容
//    clipBoardItems.clear();
//    //将过滤后的选中项存入剪贴板
//    filterSelectedItems();
//    clipBoardItems = selectedItems();
//}

//void BaseTree::pasteAction()
//{
//    int count = clipBoardItems.count();
//    if (count == 0)
//    {
//        //剪贴板没有内容
//        QApplication::beep();
//        return;
//    }
//    //复制到当前所指项下面 没有所指项 成为top节点
//    QTreeWidgetItem *destItem = currentItem();
//    QString destLevelPath = getLevelPath(destItem);
//    for (int i = 0; i < count; i++)
//    {
//        QString itemLevelPath = getLevelPath(clipBoardItems.at(i));
//        //不能复制到自己和自己的子孙下面
//        if (destLevelPath.mid(0, itemLevelPath.length()) == itemLevelPath)
//        {
//            //错误报警声
//            QApplication::beep();
//            return;
//        }
//    }
//    //粘贴合法
//    //去掉选中项的高亮
//    QList<QTreeWidgetItem*> selected = selectedItems();
//    count = selected.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(selected.at(i), false);
//    }
//    count = clipBoardItems.count();
//    for (int i = 0; i < count; i++)
//    {
//        QTreeWidgetItem *transcript = clipBoardItems.at(i)->clone();
//        QString name = transcript->text(0);
//        if (destItem)
//        {
//            int childCount = destItem->childCount();
//            //重命名 同一级不能有重名
//            int index = 2;
//            while (1)
//            {
//                int j;
//                for (j = 0; j < childCount; j++)
//                {
//                    if (destItem->child(j)->text(0) == name)
//                    {
//                        break;
//                    }
//                }
//                if (j < childCount)
//                {
//                    //有重名 加(2) (3) 继续检查是否有重名
//                    name = transcript->text(0) + "(" + QString::number(index) + ")";
//                    index++;
//                }
//                else
//                {
//                    //没有重名
//                    transcript->setText(0, name);
//                    break;
//                }
//            }
//            //接到目标结点下
//            destItem->addChild(transcript);
//        }
//        else
//        {
//            int childCount = topLevelItemCount();
//            int index = 2;
//            while (1)
//            {
//                int j;
//                for (j = 0; j < childCount; j++)
//                {
//                    if (topLevelItem(j)->text(0) == name)
//                    {
//                        break;
//                    }
//                }
//                if (j < childCount)
//                {
//                    //有重名 加(2) (3) 继续检查是否有重名
//                    name = transcript->text(0) + "(" + QString::number(index) + ")";
//                    index++;
//                }
//                else
//                {
//                    //没有重名
//                    transcript->setText(0, name);
//                    break;
//                }
//            }
//            addTopLevelItem(transcript);
//        }
//        //粘贴项高亮
//        setItemSelected(transcript, true);
//        //子类执行粘贴一项的具体操作
//        pasteOne(clipBoardItems.at(i), transcript);
//    }
//    //展开目的结点
//    if (destItem)
//        setItemExpanded(destItem, true);

//    //粘贴操作结束时候调用
//    pasteEnd();
//}

//void BaseTree::pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript)
//{
//    Q_UNUSED(source);
//    Q_UNUSED(transcript);
//    //粘贴合法时才会调用
//    //复制source 粘贴为transcript
//}

//void BaseTree::pasteEnd()
//{
//    //粘贴操作结束之后调用 子类执行不同的操作
//    //当粘贴操作不合法时 不会调用
//}

//void BaseTree::keyPressEvent(QKeyEvent *event)
//{
//    //处理菜单快捷键事件
//    int key;
//#if defined(Q_OS_WIN32)
//    key = Qt::Key_F2;
//#elif defined(Q_OS_MAC)
//    key = Qt::Key_Return;
//#endif
//    if (event->key() == key)
//    {
//        openEditor();
//    }
//}

//void BaseTree::focusInEvent(QFocusEvent *event)
//{
//    //父类 当获得焦点时 第一项会有蓝框 很丑
//    Q_UNUSED(event);
//}
