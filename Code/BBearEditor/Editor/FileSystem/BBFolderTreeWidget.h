#ifndef BBFOLDERTREEWIDGET_H
#define BBFOLDERTREEWIDGET_H

#include "BBTreeWidget.h"
#include "Utils/BBUtils.h"


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
    void newSceneAction();
    void newMaterialAction();
    void showInFolder();
    void copyAction() override;
    void pasteAction() override;
    void finishRename() override;
    void deleteAction() override;

signals:
    void accessFolder(const QString &filePath, QTreeWidgetItem *pItem);
    void newFolder(const QString &parentPath, const BBSignalSender &eSender);
    void newFile(const QString &parentPath, int nType);
    void showInFolder(const QString &filePath);
    void rename(QTreeWidgetItem *pParentFolderItem, const QString &oldPath, const QString &newPath);
    void deleteFolder(QTreeWidgetItem *pItem);
    void finishDeleteAction();
    void moveFolders(const QList<QTreeWidgetItem*> &items, QTreeWidgetItem *pNewParentItem, bool bCopy);
    void moveFiles(const QList<QString> &oldFilePaths, QTreeWidgetItem *pNewParentItem, bool bCopy);

private:
    void setMenu() override;
    QWidgetAction* createWidgetAction(QMenu *pParent, const QString &iconPath, const QString &name);
    void newFile(int nType);
    void deleteOne(QTreeWidgetItem *pItem) override;

    void dragMoveEvent(QDragMoveEvent *event) override;
    bool moveItem() override;
    bool moveItemFromFileList(const QMimeData *pMimeData) override;

    void updateCorrespondingWidget(QTreeWidgetItem *pItem);
    void recordItemExpansionState();
    void resumeItemExpansionState();

    const BBSignalSender m_eSenderTag;
    QList<QTreeWidgetItem*> m_ExpandedItems;
};

#endif // BBFOLDERTREEWIDGET_H

