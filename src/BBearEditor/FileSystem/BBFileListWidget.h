#ifndef BBFILELISTWIDGET_H
#define BBFILELISTWIDGET_H


#include <QListWidget>
#include <QPlainTextEdit>
#include <BBUtils.h>
#include "BBFileSystemData.h"


class QTreeWidgetItem;
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

    QString getMimeType() { return BB_MIMETYPE_FILELISTWIDGET; }

    void loadItems(const QList<QListWidgetItem*> &items);

public:
    static QSize m_ItemSize;

private slots:
//    void doubleClickItem(QListWidgetItem *pItem);
//    void newFolder();
//    void showInFolder();
//    void copyAction();
//    void pasteAction();
//    void openRenameEditor();
//    void finishRename();
//    void deleteAction();

signals:
//    void clickFolderTreeItem(const QString &folderPath);
//    void addItemInFolderTree(const QString &parentPath, const QString &name);
//    void renameItemInFolderTree(const QString &oldName, const QString &newName);
//    void moveItemInFolderTree(const QString &oldPath, const QString &newPath);
//    void deleteItemInFolderTree(const QString &folderPath);
//    void updateFolderTree();

private:
    void setMenu();
    QWidgetAction* createWidgetAction(const QString &iconPath, const QString &name);


//    void deleteOne(QListWidgetItem *pItem);

//    bool moveFile(const QString &oldPath, QString &newPath, BBFileType eFileType, bool bCopy);

//    void startDrag(Qt::DropActions supportedActions) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
//    void dropEvent(QDropEvent *event) override;
//    void importAsset(const QList<QUrl> &urls);
//    void importAsset(const QFileInfo &fileInfo, const QString &newPath);
//    bool moveItem();
//    bool moveItemFromFolderTree(const QMimeData *pMimeData);
//    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

    static QSize m_StandardIconSize;
    static QSize m_StandardItemSize;

    QMenu *m_pMenu;
    QString m_FolderPath;
    BBFILE m_FileData;
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
//    void copyByProjectTree(QList<QString> folderPaths);
//    void pasteFileFromProjectTree(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void changeItemSize(int factor);
//    void itemClickedSlot(QListWidgetItem* item);
//    void updateMaterialFileIcon(QString filePath);

//signals:
//    void copyToProjectTree(QList<QString> filePaths);
//    void pasteItemInProjectTree(QList<QString> clipBoardTranscriptFolderNames);
//    void removeRenameItemInTreeClipBoard(QString path);
//    void cancelHierarchyTreeSelectedItems();
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void clearPropertyWidget();

//private:
//    void selectPasteItem(QList<QString> itemNames);

//    QListWidgetItem *indicatorItem;
//    QList<QString> clipBoardPaths;

//};
