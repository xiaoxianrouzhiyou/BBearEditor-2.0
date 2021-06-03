#ifndef BBFILELISTWIDGET_H
#define BBFILELISTWIDGET_H


#include <QListWidget>
#include <QPlainTextEdit>
#include <BBUtils.h>


using namespace BBFileSystem;


class QWidgetAction;


class BBPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    BBPlainTextEdit(QWidget *pParent = 0);

signals:
    void editFinished();

private:
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};


class BBFileListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit BBFileListWidget(QWidget *pParent = nullptr);
    ~BBFileListWidget();

    inline QString getMimeType() { return BB_MIMETYPE_FILELISTWIDGET; }
    inline QString getCurrentParentPath() { return m_ParentPath; }
    inline QTreeWidgetItem* getCurrentParentItem() { return m_pParentItem; }

    void loadItems(const QString &parentPath, QTreeWidgetItem *pParentItem,
                   BBFILE *pFileData, QListWidgetItem *pCurrentItem);
    void updateCurrentInfos(QTreeWidgetItem *pDeletedItem);

public:
    static QSize m_ItemSize;

private slots:
    void doubleClickItem(QListWidgetItem *pItem);
    void newFolder();
    void showInFolder();
    void copyAction();
    void pasteAction();
    void openRenameEditor();
    void finishRename();
    void deleteAction();

signals:
    void openFile(const QString &filePath);
    void newFolder(const QString &parentPath, const BBSignalSender &eSender);
    void showInFolder(const QString &filePath);
    void rename(QListWidgetItem *pFileItem, const QString &oldPath, const QString &newPath);
    void deleteFiles(QTreeWidgetItem *pParentItem, const QString &parentPath, const QList<QListWidgetItem*> &items);
    void importAsset(const QList<QUrl> &urls);
    bool moveFile(const QString &oldPath, const QString &newPath, const BBFileType &eFileType, bool bCopy);

private:
    void setMenu();
    QWidgetAction* createWidgetAction(const QString &iconPath, const QString &name);

    void startDrag(Qt::DropActions supportedActions) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool moveItem();
    bool moveItemFromFolderTree(const QMimeData *pMimeData);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

    QString getPathByItem(QListWidgetItem *pItem);

    const BBSignalSender m_eSenderTag;

    static QSize m_StandardIconSize;
    static QSize m_StandardItemSize;

    QMenu *m_pMenu;
    QString m_ParentPath;
    QTreeWidgetItem *m_pParentItem;
    BBFILE *m_pFileData;
    QListWidgetItem *m_pEditingItem;
    BBPlainTextEdit *m_pRenameEditor;
    QListWidgetItem *m_pIndicatorItem;
};

#endif // BBFILELISTWIDGET_H











//class FileList : public QListWidget
//{

//public slots:
//    void newMaterial();
//    void newScript();
//    void cancelSelectedItems();

//private slots:
//    void pasteFileFromProjectTree(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void changeItemSize(int factor);
//    void itemClickedSlot(QListWidgetItem* item);
//    void updateMaterialFileIcon(QString filePath);

//signals:
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void clearPropertyWidget();

//private:
//    void selectPasteItem(QList<QString> itemNames);

//};
