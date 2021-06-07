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

    void loadItems(const QString &parentPath, BBFILE *pFileData, QListWidgetItem *pCurrentItem);
    void setSelectedItems(const QList<QListWidgetItem*> &items);

public:
    static QSize m_ItemSize;

private slots:
    void clickItem(QListWidgetItem *pItem);
    void doubleClickItem(QListWidgetItem *pItem);
    void changeItemSize(int factor);
    void newFolder();
    void newMaterial();
    void newScript();
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
    void deleteFiles(const QList<QListWidgetItem*> &items);
    void importAsset(const QString &parentPath, const QList<QUrl> &urls);
    void moveFolders(const QList<QString> &oldFilePaths, const QString &newParentPath, bool bCopy);
    void moveFiles(const QList<QListWidgetItem*> &items, const QString &oldParentPath,
                   const QString &newParentPath, bool bCopy);

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
    BBFILE *m_pFileData;
    QListWidgetItem *m_pEditingItem;
    BBPlainTextEdit *m_pRenameEditor;
    QListWidgetItem *m_pIndicatorItem;
};

#endif // BBFILELISTWIDGET_H






//private slots:
//    void pasteFileFromProjectTree(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void updateMaterialFileIcon(QString filePath);

//signals:
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void clearPropertyWidget();

