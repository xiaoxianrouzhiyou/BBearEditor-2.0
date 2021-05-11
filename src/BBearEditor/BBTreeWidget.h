#ifndef BBTREEWIDGET_H
#define BBTREEWIDGET_H

#include <QLineEdit>
#include <QTreeWidget>
#include "BBUtils.h"

class BBLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit BBLineEdit(QWidget *parent = 0);

signals:
    void finishEdit();

private:
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};


// drop target
enum BBIndicatorPos
{
    // drop in the item rect
    RECT = 0x01,
    // drop between two items
    TOP = 0x02,
    BOTTOM = 0x04
};


class BBTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    virtual QString getMimeType() { return BB_MIMETYPE_TREEWIDGET; }

protected:
    explicit BBTreeWidget(QWidget *parent = 0);
    ~BBTreeWidget();

    void startDrag(Qt::DropActions supportedActions) override;
    virtual bool moveItem();
    virtual bool moveItemFromFileList(const QMimeData *pMimeData);
    virtual bool moveItemFromOthers(const QMimeData *pMimeData);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    QTreeWidgetItem *m_pIndicatorItem;
    BBIndicatorPos m_eIndicatorPos;
    QMenu *m_pMenu;

//    virtual void deleteOne(QTreeWidgetItem *item);
//    void keyPressEvent(QKeyEvent *event) override;
//    virtual void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript);
//    virtual void pasteEnd();
//    void deleteAction(QTreeWidgetItem *item);
//    QTreeWidgetItem *editingItem;
//    QLineEdit *edit;
//    QList<QTreeWidgetItem*> clipBoardItems;

private:
    QTreeWidgetItem *m_pLastItem;


protected:
    void filterSelectedItems();
    // drag, Which column of icons to use
    virtual int getDragIconColumnIndex() { return 0; }
    QString getLevelPath(QTreeWidgetItem *pItem);


//    void paintEvent(QPaintEvent *event) override;
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void contextMenuEvent(QContextMenuEvent *event) override;
//    void focusInEvent(QFocusEvent *event) override;
//    QTime mLastTime;


//public slots:
//    virtual void copyAction();
//    virtual void pasteAction();
//    virtual void deleteAction();

//protected slots:
//    void openEditor();
//    virtual void finishRename();

};

#endif // BBTREEWIDGET_H
