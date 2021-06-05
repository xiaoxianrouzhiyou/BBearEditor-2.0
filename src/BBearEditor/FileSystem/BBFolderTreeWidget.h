#ifndef BBFOLDERTREEWIDGET_H
#define BBFOLDERTREEWIDGET_H

#include "BBTreeWidget.h"
#include "BBUtils.h"


using namespace BBFileSystem;


class QWidgetAction;


class BBFolderTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBFolderTreeWidget(QWidget *pParent = nullptr);

    QString getMimeType() { return BB_MIMETYPE_FOLDERTREEWIDGET; }

    void removeTopLevelItems();
    void loadTopLevelItems(const QList<QTreeWidgetItem*> &items);
    void expandCurrentViewedItem(QTreeWidgetItem *pItem);
    void setSelectedItems(const QList<QTreeWidgetItem*> &items);

private slots:
    void pressRootButton();
    void pressSettingButton();
    void clickItem(QTreeWidgetItem *pItem, int nColumn);
    void newFolder();
    void showInFolder();
    void copyAction() override;
    void pasteAction() override;
    void finishRename() override;
    void deleteAction() override;

signals:
    void accessFolder(const QString &filePath, QTreeWidgetItem *pItem);
    void newFolder(const QString &parentPath, const BBSignalSender &eSender);
    void showInFolder(const QString &filePath);
    void rename(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath);
    void deleteFolder(QTreeWidgetItem *pItem);
    void finishDeleteAction();
    void moveFolders(const QList<QTreeWidgetItem*> &items, QTreeWidgetItem *pNewParentItem, bool bCopy);

private:
    void setMenu() override;
    QWidgetAction* createWidgetAction(QMenu *pParent, const QString &iconPath, const QString &name);
    void deleteOne(QTreeWidgetItem *pItem) override;

    void dragMoveEvent(QDragMoveEvent *event) override;
    bool moveItem() override;
    bool moveItemFromFileList(const QMimeData *pMimeData) override;

    void updateCorrespondingWidget(QTreeWidgetItem *pItem);
    void recordItemExpansionState();
    void resumeItemExpansionState();

    const BBSignalSender m_eSenderTag;
    QList<QTreeWidgetItem*> m_ExpandedItems;
    QTreeWidgetItem *m_pCurrentViewedItem;
};

#endif // BBFOLDERTREEWIDGET_H







//signals:
//    void copyToFileList(QList<QString> folderPaths);
//    void pasteFile(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void cancelHierarchyTreeSelectedItems();
//    void clearPropertyWidget();

//private slots:
//    void copyByFileList(QList<QString> filePaths);
//    void pasteItemWithoutPasteFile(QList<QString> clipBoardTranscriptFolderNames);
//    void removeClipBoardRenameItem(QString path);

//private:
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    void pasteEnd() override;
//    void loadMaterial(QString filePath);
//    //用于存粘贴的副本文件夹名字 用于高亮显示
//    QList<QString> pastedFolderNames;
//    //有时候加载工程 却无需加载材质
//    bool isLoadMaterial;

