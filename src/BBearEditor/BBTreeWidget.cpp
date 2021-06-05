#include "BBTreeWidget.h"
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QDrag>
#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>
#include "Window/BBConfirmationDialog.h"


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
    m_pIndicatorItem = NULL;
    m_pLastItem = NULL;
    m_eIndicatorPos = BBIndicatorPos::CENTER;
    m_pEditingItem = NULL;
    m_pRenameEditor = NULL;

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

QString BBTreeWidget::getLevelPath(QTreeWidgetItem *pItem)
{
    if (pItem == NULL)
    {
        return "";
    }
    else
    {
        // Hierarchical path of item
        QString location;
        for (QTreeWidgetItem *pParent = pItem; pParent; pParent = pParent->parent())
        {
            location = pParent->text(0) + "/" + location;
        }
        // remove "/" at the end
        location = location.mid(0, location.length() - 1);
        return location;
    }
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

    // Used to calculate the size of the final drag icon
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
        int index = -1;
        QTreeWidgetItem *pParent = getParentOfMovingItem(index);
        // The movable item has been filtered in startDrag
        QList<QTreeWidgetItem*> items = selectedItems();
        // Cannot move to an item that are moving and its descendants
        // parent and its ancestors cannot be included in items
        for (QTreeWidgetItem *pForeParent = pParent; pForeParent; pForeParent = pForeParent->parent())
        {
            BB_PROCESS_ERROR_RETURN_FALSE(!items.contains(pForeParent));
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
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FILELISTWIDGET))
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
    // Scroll up or down when the mouse is at the top or bottom
    QTreeWidget::dragMoveEvent(event);

    QTreeWidgetItem *pItem = itemAt(event->pos());
    m_pIndicatorItem = pItem;
    if (pItem)
    {
        if (pItem != m_pLastItem)
        {
            // The item corresponding to the position pointed has changed
            // Re-record the new item
            m_pLastItem = pItem;
            // Re-record the current time
            m_LastTime = QTime::currentTime();
        }
        else
        {
            // When moving, the pointed item is not changed
            if (m_LastTime.elapsed() > 1000)
            {
                // Time exceeds a certain amount, expand the item
                setItemExpanded(pItem, true);
            }
        }

        int mouseY = event->pos().y();
        int itemTop = visualItemRect(m_pIndicatorItem).top();
        int itemBottom = visualItemRect(m_pIndicatorItem).bottom();
        if (mouseY < itemTop + 2)
        {
            m_eIndicatorPos = BBIndicatorPos::TOP;
        }
        else if (mouseY <= itemBottom - 2)
        {
            // The mouse drops on the non-edge of the item
            // become the child of the item
            m_eIndicatorPos = BBIndicatorPos::CENTER;
        }
        else
        {
            m_eIndicatorPos = BBIndicatorPos::BOTTOM;
        }
    }
    else
    {
        // No item pointed, reset
        m_pLastItem = NULL;
    }

     // Draw the place where it will be placed
    repaint();

    // do this in order to execute dropEvent
    event->accept();
}

void BBTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    // No need for indicator box when dragLeave
    m_pIndicatorItem = NULL;
    repaint();
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
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
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

void BBTreeWidget::paintEvent(QPaintEvent *event)
{
    QTreeWidget::paintEvent(event);
    // The pointed item is not itself
    if (m_pIndicatorItem /*&& m_pIndicatorItem != currentItem()*/)
    {
        //using "this" is wrong
        QPainter painter(viewport());
        QPen pen(QColor("#d6dfeb"));
        painter.setPen(pen);
        QRect rect = visualItemRect(m_pIndicatorItem);
        if (m_eIndicatorPos == BBIndicatorPos::CENTER)
        {
            painter.drawRect(QRect(rect.topLeft(), rect.bottomRight() - QPoint(1, 1)));
        }
        else if (m_eIndicatorPos == BBIndicatorPos::TOP)
        {
            painter.drawLine(rect.topLeft(), rect.topRight());
        }
        else
        {
            painter.drawLine(rect.bottomLeft(), rect.bottomRight());
        }
        painter.end();
    }
}

void BBTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        // There is no item at the mouse click position, remove the selection
        QTreeWidgetItem *pItem = itemAt(event->pos());
        if (!pItem)
        {
            setCurrentItem(NULL);
        }
    }
}

void BBTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    m_pMenu->exec(cursor().pos());
}

void BBTreeWidget::keyPressEvent(QKeyEvent *event)
{
    // Handling menu shortcut events
    int key;
#if defined(Q_OS_WIN32)
    key = Qt::Key_F2;
#elif defined(Q_OS_MAC)
    key = Qt::Key_Return;
#endif
    if (event->key() == key)
    {
        openRenameEditor();
    }
}

void BBTreeWidget::focusInEvent(QFocusEvent *event)
{
    // parent class, when the focus is obtained, the first item will show a blue box, which is ugly
    Q_UNUSED(event);
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

QTreeWidgetItem* BBTreeWidget::getParentOfMovingItem(int &nIndex)
{
    QTreeWidgetItem *pParent = NULL;
    //nIndex is the position inserted
    if (m_eIndicatorPos == BBIndicatorPos::CENTER)
    {
        // become the child of m_pIndicatorItem
        pParent = m_pIndicatorItem;
        // If add to the end, use parent->childCount()
        // But when item is dragged to its parent, deleting itself makes parent->childCount() decrease
        // add itself at the end is wrong
        // Add to the first
        nIndex = 0;
    }
    else if (m_eIndicatorPos == BBIndicatorPos::TOP)
    {
        // become the front sibling of m_pIndicatorItem
        pParent = m_pIndicatorItem->parent();
        if (pParent)
            nIndex = pParent->indexOfChild(m_pIndicatorItem);
        else
            nIndex = indexOfTopLevelItem(m_pIndicatorItem);
    }
    else//m_eIndicatorPos == BBIndicatorPos::BOTTOM
    {
        // become the back sibling of m_pIndicatorItem
        pParent = m_pIndicatorItem->parent();
        if (pParent)
            nIndex = pParent->indexOfChild(m_pIndicatorItem) + 1;
        else
            nIndex = indexOfTopLevelItem(m_pIndicatorItem) + 1;
    }
    return pParent;
}

void BBTreeWidget::pasteOne(QTreeWidgetItem *pSource, QTreeWidgetItem* pTranscript)
{
    Q_UNUSED(pSource);
    Q_UNUSED(pTranscript);
    // Invoked only when the paste is legal
    // copy source, paste transcript
}

void BBTreeWidget::pasteEnd()
{
    // After the paste operation is over, invoke, the subclass to perform different operations
    // Invoked only when the paste is legal
}

void BBTreeWidget::deleteAction(QTreeWidgetItem *pItem)
{
    // After the child is deleted, childCount() will decrease. Error with for loop
    while (pItem->childCount() > 0)
    {
        QTreeWidgetItem* pChild = pItem->child(0);
        deleteAction(pChild);
    }
    deleteOne(pItem);
}

void BBTreeWidget::deleteOne(QTreeWidgetItem *pItem)
{
    // If the item is in the clipboard, delete it
    if (m_ClipBoardItems.contains(pItem))
    {
        m_ClipBoardItems.removeOne(pItem);
    }

    BB_SAFE_DELETE(pItem);
}

void BBTreeWidget::copyAction()
{
    // copy items
    // Clear the last copied content saved in the clipboard
    m_ClipBoardItems.clear();
    // Save the filtered selection to the clipboard
    filterSelectedItems();
    m_ClipBoardItems = selectedItems();
}

void BBTreeWidget::pasteAction()
{
    int count = m_ClipBoardItems.count();
    if (count == 0)
    {
        // Clipboard has no content
        QApplication::beep();
        return;
    }

    // Copy into the pointed item, when there is no pointed item, add into the top level
    QTreeWidgetItem *pDestItem = currentItem();
    QString destLevelPath = getLevelPath(pDestItem);
    for (int i = 0; i < count; i++)
    {
        QString itemLevelPath = getLevelPath(m_ClipBoardItems.at(i));
        // Can’t be copied into oneself and one's descendants
        if (destLevelPath.mid(0, itemLevelPath.length()) == itemLevelPath)
        {
            QApplication::beep();
            return;
        }
    }

    // legal paste
    // Remove the highlight of the selected item
    QList<QTreeWidgetItem*> selected = selectedItems();
    for (int i = 0; i < selected.count(); i++)
    {
        setItemSelected(selected.at(i), false);
    }

    for (int i = 0; i < m_ClipBoardItems.count(); i++)
    {
        QTreeWidgetItem *pTranscript = m_ClipBoardItems.at(i)->clone();
        QString name = pTranscript->text(0);
        if (pDestItem)
        {
            int childCount = pDestItem->childCount();
            // Rename, the same level cannot include the same name
            int index = 2;
            while (1)
            {
                int j;
                for (j = 0; j < childCount; j++)
                {
                    if (pDestItem->child(j)->text(0) == name)
                    {
                        break;
                    }
                }
                if (j < childCount)
                {
                    // If the same name, add (2) (3), continue to check whether there is the same name
                    name = pTranscript->text(0) + "(" + QString::number(index) + ")";
                    index++;
                }
                else
                {
                    // No the same name
                    pTranscript->setText(0, name);
                    break;
                }
            }
            // add into pDestItem
            pDestItem->addChild(pTranscript);
        }
        else
        {
            int childCount = topLevelItemCount();
            int index = 2;
            while (1)
            {
                int j;
                for (j = 0; j < childCount; j++)
                {
                    if (topLevelItem(j)->text(0) == name)
                    {
                        break;
                    }
                }
                if (j < childCount)
                {
                    // If the same name, add (2) (3), continue to check whether there is the same name
                    name = pTranscript->text(0) + "(" + QString::number(index) + ")";
                    index++;
                }
                else
                {
                    // No the same name
                    pTranscript->setText(0, name);
                    break;
                }
            }
            addTopLevelItem(pTranscript);
        }
        // highlight pasted items
        setItemSelected(pTranscript, true);
        // Subclass to perform specific operations of paste
        pasteOne(m_ClipBoardItems.at(i), pTranscript);
    }

    if (pDestItem)
        setItemExpanded(pDestItem, true);

    // Invoked at the end of the paste operation
    pasteEnd();
}

void BBTreeWidget::openRenameEditor()
{
    QList<QTreeWidgetItem*> selected = selectedItems();
    if (selected.count() == 1)
    {
        // Multi-select or unselect will not perform the rename operation
        m_pEditingItem = selected.first();

        m_pRenameEditor = new BBLineEdit(this);
        m_pRenameEditor->setStyleSheet("height: 16px; border: none; background: #d6dfeb; color: #191f28;"
                                       "selection-color: #d6dfeb; selection-background-color: #8193bc;"
                                       "font: 9pt \"Arial\"; padding: 0px 5px;");
        // margin-left: 22px;
        m_pRenameEditor->setText(m_pEditingItem->text(0));
        m_pRenameEditor->selectAll();

        QObject::connect(m_pRenameEditor, SIGNAL(finishEdit()), this, SLOT(finishRename()));

        setItemWidget(m_pEditingItem, 0, m_pRenameEditor);
        // make effective
        m_pRenameEditor->setFocus();
    }
}

void BBTreeWidget::finishRename()
{
    removeItemWidget(m_pEditingItem, 0);
    BB_SAFE_DELETE(m_pRenameEditor);
    m_pEditingItem = NULL;
    // Otherwise, the list has no focus and no longer responds to key events
    setFocus();
}

void BBTreeWidget::deleteAction()
{
    // When selecting ancestors and descendants at the same time, filter out descendants
    filterSelectedItems();

    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count() == 0)
        return;

    // pop dialog
    BBConfirmationDialog dialog;
    dialog.setTitle("Delete selected?");
    if (items.count() == 1)
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete this?");
    }
    else
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete these "
                           + QString::number(items.count()) + " items?");
    }

    if (dialog.exec())
    {
        // start delete operation
        for (int i = 0; i < items.count(); i++)
        {
            deleteAction(items.at(i));
        }
        setCurrentItem(NULL);
    }
}
