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
    CENTER = 0x01,
    // drop between two items
    TOP = 0x02,
    BOTTOM = 0x04
};


class BBTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    virtual QString getMimeType() { return BB_MIMETYPE_TREEWIDGET; }

    explicit BBTreeWidget(QWidget *parent = 0);
    virtual ~BBTreeWidget();

public:
    static QString getLevelPath(QTreeWidgetItem *pItem);

protected:
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
    void focusInEvent(QFocusEvent *event) override;

    QTreeWidgetItem *m_pIndicatorItem;
    BBIndicatorPos m_eIndicatorPos;
    QMenu *m_pMenu;
    QList<QTreeWidgetItem*> m_ClipBoardItems;
    QTreeWidgetItem *m_pLastItem;
    QTime m_LastTime;
    QTreeWidgetItem *m_pEditingItem;
    BBLineEdit *m_pRenameEditor;

protected:
    virtual void setMenu() = 0;
    void filterSelectedItems();
    QTreeWidgetItem* getParentOfMovingItem(int &nIndex);
    // drag, Which column of icons to use
    virtual int getDragIconColumnIndex() { return 0; }
    virtual void pasteOne(QTreeWidgetItem *pSource, QTreeWidgetItem* pTranscript);
    virtual void pasteEnd();
    void deleteAction(QTreeWidgetItem *pItem);
    virtual void deleteOne(QTreeWidgetItem *pItem);

protected slots:
    virtual void copyAction();
    virtual void pasteAction();
    void openRenameEditor();
    virtual void finishRename();
    virtual void deleteAction();
};

#endif // BBTREEWIDGET_H
