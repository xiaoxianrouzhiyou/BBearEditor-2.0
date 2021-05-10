#include "BBTreeWidget.h"
#include <QKeyEvent>


//#include <QPainter>
//#include <QPen>
//#include <QLine>
//#include <QDebug>
//#include <QApplication>
//#include <QDesktopWidget>
//#include <QMimeData>
//#include <QDrag>
//#include <QScrollBar>
//#include <limits.h>
//#include <QTextItem>
//#include <QDropEvent>
//#include <QDragMoveEvent>
//#include <QPaintEvent>
//#include <QMenu>
//#include <QTime>
//#include "filelist.h"
//#include "confirmationdialog.h"


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


//BaseTree::BaseTree(QWidget *parent)
//    : QTreeWidget(parent), editingItem(NULL), indicatorItem(NULL), mLastItem(NULL)
//{
//    //启用拖放
//    setDragEnabled(true);
//    //设置拖放
//    setAcceptDrops(true);
//    //移动 不是复制
//    setDefaultDropAction(Qt::MoveAction);
//    //内部拖拽 不能拖到别的树 也不能从别的树拖过来
//    //setDragDropMode(QAbstractItemView::InternalMove);
//    //可按shift cmd多选
//    setSelectionMode(QAbstractItemView::ExtendedSelection);
//    //不需要拖拽落下指示器
//    setDropIndicatorShown(false);
//}

//QString BaseTree::getMimeType()
//{
//    return "tree";
//}

//void BaseTree::dragEnterEvent(QDragEnterEvent *event)
//{
//    if (event->mimeData()->hasFormat(getMimeType()))
//    {
//        //接收内部拖拽
//        event->accept();
//    }
//    else if (event->mimeData()->hasFormat(FileList::getMimeType()))
//    {
//        //接收文件列表中拖来的文件
//        event->accept();
//    }
//    else if (event->mimeData()->hasFormat("base"))
//    {
//        //接收预制体
//        event->accept();
//    }
//    else if (event->mimeData()->hasFormat("light"))
//    {
//        //接收灯光
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}

//void BaseTree::dragMoveEvent(QDragMoveEvent *event)
//{
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
//    event->accept();
//}

//void BaseTree::dropEvent(QDropEvent *event)
//{
//    if (event->mimeData()->hasFormat(getMimeType()))
//    {
//        //内部拖拽树节点
//        if (dragDropItem())
//        {
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
//    }
//    else if (event->mimeData()->hasFormat(FileList::getMimeType()))
//    {
//        //接收文件列表中拖来的文件
//        if (moveItemFromFileList(event->mimeData()))
//        {
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
//    }
//    else
//    {
//        //处理其他控件移来的
//        if (moveItemFromOther(event->mimeData()))
//        {
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
//    }
//    //拖拽完毕后不再需要指示框
//    indicatorItem = NULL;
//    //处理完毕后重置当前悬浮的item
//    mLastItem = NULL;
//    repaint();
//}

//bool BaseTree::dragDropItem()
//{
//    //树节点移动位置
//    if (indicatorItem)
//    {
//        //落下的位置
//        QTreeWidgetItem *parent = NULL;
//        //落下的索引值
//        int index;
//        if (indicatorPos == IndicatorPos::RECT)
//        {
//            //成为indicatorItem的孩子
//            parent = indicatorItem;
//            //添加到最后 如果用parent->childCount() 如果结点拖到他的父亲中 删去自己parent->childCount()减少了 再在父节点最后添加自己 不对
//            //添加到首
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
//        //拖拽的结点 已经在startDrag中进行过过滤了
//        QList<QTreeWidgetItem*> items = selectedItems();
//        //不能移动到正在移动的某个结点及其子孙中
//        //parent不能包含于items中 parent的祖先不能包含于items
//        for (QTreeWidgetItem *foreParent = parent; foreParent; foreParent = foreParent->parent())
//        {
//            if (items.contains(foreParent))
//            {
//                return false;
//            }
//        }
//        for (int i = 0; i < items.count(); i++)
//        {
//            QTreeWidgetItem *item = items.at(i);
//            //结点从树中移除
//            int removeIndex;
//            QTreeWidgetItem *removeItemParent = item->parent();
//            QTreeWidgetItem *indicatorItemParent = indicatorItem->parent();
//            if (removeItemParent)
//            {
//                //item不是top结点
//                removeIndex = removeItemParent->indexOfChild(item);
//                removeItemParent->removeChild(item);
//            }
//            else
//            {
//                removeIndex = indexOfTopLevelItem(item);
//                takeTopLevelItem(removeIndex);
//            }
//            //如果两者同级 移除的结点在indicatorItem的前面 index减1
//            //item已被移除 item->parent()为空 只能用removeItemParent
//            if ((removeItemParent == indicatorItemParent) && (removeIndex < index))
//            {
//                index--;
//            }
//            //结点添加在新的位置
//            if (parent)
//            {
//                parent->insertChild(index, item);
//            }
//            else
//            {
//                //成为top结点
//                insertTopLevelItem(index, item);
//            }
//            //选中新结点
//            setItemSelected(item, true);
//            //下个结点在下方插入 否则1 2 3 插入后变成3 2 1
//            index++;
//        }
//        //展开parent 新的结点可见
//        if (parent)
//        {
//            setItemExpanded(parent, true);
//        }
//        return true;
//    }
//    return false;
//}

//bool BaseTree::moveItemFromFileList(const QMimeData *mimeData)
//{
//    return true;
//}

//bool BaseTree::moveItemFromOther(const QMimeData *mimeData)
//{
//    return false;
//}

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

//void BaseTree::filterSelectedItems()
//{
//    QList<QTreeWidgetItem*> items = selectedItems();
//    if (items.count() > 1)
//    {
//        //选中多项
//        for (int i = 0; i < items.count(); i++)
//        {
//            for (QTreeWidgetItem *parent = items.at(i)->parent(); parent; parent = parent->parent())
//            {
//                //遍历祖先 看是否也被选中
//                if (items.contains(parent))
//                {
//                    //祖先和子孙同时被选中 只处理祖先
//                    setItemSelected(items.at(i), false);
//                    break;
//                }
//            }
//        }
//    }
//}

//QString BaseTree::getLevelPath(QTreeWidgetItem *item)
//{
//    //求树item的层级路径
//    QString location;
//    for (QTreeWidgetItem *parent = item; parent; parent = parent->parent())
//    {
//        location = parent->text(0) + "/" + location;
//    }
//    return location;
//}

//void BaseTree::startDrag(Qt::DropActions supportedActions)
//{
//    Q_UNUSED(supportedActions);
//    //祖先和子孙同时选中 过滤掉子孙
//    filterSelectedItems();
//    QList<QTreeWidgetItem*> items = selectedItems();
//    //打包每项的路径为mimeData
//    QByteArray itemData;
//    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//    //每项 图标 文本
//    struct Info
//    {
//        int y;
//        QPixmap icon;
//        QString text;
//    };
//    QList<Info> infos;
//    //用于计算最终拖拽图标的大小
//    QFontMetrics fm = fontMetrics();
//    int pixmapWidth = 0;
//    int pixmapHeight = 0;
//    for (int i = 0; i < items.count(); i++)
//    {
//        //每项的相对路径
//        QString levelPath = getLevelPath(items.at(i));
//        dataStream << levelPath;
//        Info info;
//        QRect rect = visualItemRect(items.at(i));
//        info.y = rect.top();
//        //最终图标高度 叠加
//        pixmapHeight += rect.height();
//        info.text = items.at(i)->text(0);
//        int textWidth = fm.width(info.text);
//        if (textWidth > pixmapWidth)
//            pixmapWidth = textWidth;
//        //该项的图标
//        info.icon = items.at(i)->icon(getDragIconColumn()).pixmap(rect.height() * devicePixelRatio());
//        info.icon.setDevicePixelRatio(devicePixelRatio());
//        info.icon = info.icon.scaled(rect.height() * devicePixelRatio(), rect.height() * devicePixelRatio(),
//                                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        //infos根据上下位置关系插入排序
//        int j;
//        for (j = 0; j < infos.count(); j++)
//        {
//            if (infos.at(j).y > info.y)
//            {
//                break;
//            }
//        }
//        infos.insert(j, info);
//    }
//    QMimeData *mimeData = new QMimeData;
//    //给这个数据取一个唯一的标识名称
//    mimeData->setData(getMimeType(), itemData);
//    //拖拽的图标
//    int itemHeight = pixmapHeight / infos.count();
//    //宽度要加上小图标和留白的像素
//    QPixmap pixmap((pixmapWidth + itemHeight + 6) * devicePixelRatio(), pixmapHeight * devicePixelRatio());
//    pixmap.setDevicePixelRatio(devicePixelRatio());
//    pixmap.fill(Qt::transparent);
//    QPainter painter(&pixmap);
//    painter.setPen(QColor("#d6dfeb"));
//    painter.setFont(QFont("Arial", 10));
//    //绘制每项的图标和文本到最终图标上
//    for (int i = 0; i < infos.count(); i++)
//    {
//        int y = itemHeight * i;
//        painter.drawPixmap(0, y, infos.at(i).icon);
//        painter.drawText(QRect(itemHeight + 6, y, pixmapWidth, itemHeight), Qt::AlignLeft | Qt::AlignVCenter, infos.at(i).text);
//    }
//    //结束绘制
//    painter.end();

//    //生成拖拽
//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    drag->setPixmap(pixmap);
//    drag->setHotSpot(QPoint(0, 0));
//    //执行移动操作
//    drag->exec(Qt::MoveAction);
//}

//int BaseTree::getDragIconColumn()
//{
//    return 0;
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
