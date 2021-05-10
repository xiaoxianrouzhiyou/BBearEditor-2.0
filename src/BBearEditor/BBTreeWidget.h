#ifndef BBTREEWIDGET_H
#define BBTREEWIDGET_H

#include <QLineEdit>
#include <QTreeWidget>

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


////拖拽目标位置指示器的位置
//enum IndicatorPos
//{
//    RECT = 0x01,
//    TOP = 0x02,
//    BOTTOM = 0x04
//};

//class BaseTree : public QTreeWidget
//{
//    Q_OBJECT

//public:
//    virtual QString getMimeType();

//protected:
//    BaseTree(QWidget *parent = 0);
//    void dropEvent(QDropEvent *event) override;
//    virtual void deleteOne(QTreeWidgetItem *item);
//    void keyPressEvent(QKeyEvent *event) override;
//    QString getLevelPath(QTreeWidgetItem *item);
//    void startDrag(Qt::DropActions supportedActions) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    virtual bool dragDropItem();
//    //drag取第几列的图标
//    virtual int getDragIconColumn();
//    virtual bool moveItemFromFileList(const QMimeData *mimeData);
//    virtual bool moveItemFromOther(const QMimeData *mimeData);
//    virtual void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript);
//    virtual void pasteEnd();
//    void filterSelectedItems();
//    void deleteAction(QTreeWidgetItem *item);
//    QTreeWidgetItem *editingItem;
//    QLineEdit *edit;
//    QMenu *menu;
//    QTreeWidgetItem *indicatorItem;
//    IndicatorPos indicatorPos;
//    QList<QTreeWidgetItem*> clipBoardItems;

//private:
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void paintEvent(QPaintEvent *event) override;
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void contextMenuEvent(QContextMenuEvent *event) override;
//    void focusInEvent(QFocusEvent *event) override;
//    QTime mLastTime;
//    QTreeWidgetItem *mLastItem;

//public slots:
//    virtual void copyAction();
//    virtual void pasteAction();
//    virtual void deleteAction();

//protected slots:
//    void openEditor();
//    virtual void finishRename();

//};

#endif // BBTREEWIDGET_H
