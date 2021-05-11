#ifndef BBTREEWIDGET_H
#define BBTREEWIDGET_H

#include <QLineEdit>
#include <QTreeWidget>
#include "BBUtils.h"
#include <QTime>

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
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    QTreeWidgetItem *m_pIndicatorItem;
    BBIndicatorPos m_eIndicatorPos;
    QMenu *m_pMenu;

//    virtual void deleteOne(QTreeWidgetItem *item);
//    virtual void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript);
//    virtual void pasteEnd();
//    void deleteAction(QTreeWidgetItem *item);
//    QList<QTreeWidgetItem*> clipBoardItems;

private:
    QTreeWidgetItem *m_pLastItem;
    QTime m_LastTime;
    QTreeWidgetItem *m_pEditingItem;
    BBLineEdit *m_pRenameEditor;


protected:
    void filterSelectedItems();
    // drag, Which column of icons to use
    virtual int getDragIconColumnIndex() { return 0; }
    QString getLevelPath(QTreeWidgetItem *pItem);

protected slots:
    void openRenameEditor();
    virtual void finishRename();


//    void focusInEvent(QFocusEvent *event) override;


//public slots:
//    virtual void copyAction();
//    virtual void pasteAction();
//    virtual void deleteAction();


};

#endif // BBTREEWIDGET_H
