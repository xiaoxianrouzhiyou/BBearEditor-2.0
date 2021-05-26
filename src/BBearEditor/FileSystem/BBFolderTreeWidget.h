#ifndef BBFOLDERTREEWIDGET_H
#define BBFOLDERTREEWIDGET_H

#include "BBTreeWidget.h"
#include "BBUtils.h"
#include <QFileInfo>


class QWidgetAction;


class BBFolderTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBFolderTreeWidget(QWidget *pParent = nullptr);
    QString getMimeType() { return BB_MIMETYPE_FOLDERTREEWIDGET; }

    loadProject();

private slots:
    void pressRootButton();
    void clickItem(QTreeWidgetItem *pItem, int nColumn);
    void setCurrentItemByPath(const QString &folderPath);

signals:
    void showFolderContent(const QString &folderPath);

private:
    QString getFileSuffix(QFileInfo fileInfo);
    QString getAbsolutePath(const QString &relativePath);
    QString getAbsolutePath(QTreeWidgetItem *pItem);
    QTreeWidgetItem* getItemByPath(const QString &absolutePath);

    void setMenu() override;
    QWidgetAction* createWidgetAction(QMenu *pParent, const QString &iconPath, const QString &name);   

    QTreeWidgetItem *m_pCurrentShowFolderContentItem;
};

#endif // BBFOLDERTREEWIDGET_H





//class ProjectTree : public BaseTree
//{
//    Q_OBJECT

//public:
//    static bool copyDirectoryFiles(QString fromDir, QString toDir);

//signals:
//    void updateFolderName(QString prePath, QString newPath);
//    void deleteFileItemInList();
//    void copyToFileList(QList<QString> folderPaths);
//    void pasteFile(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void cancelHierarchyTreeSelectedItems();
//    void clearPropertyWidget();

//public slots:
//    void copyAction() override;
//    void pasteAction() override;

//private slots:
//    void pressSettingButton();
//    void finishRename() override;
//    void updateFolderNameInvert(QString preName, QString newName);
//    void newFolder();
//    void showInFolder();
//    void deleteAction() override;
//    void updateCurrentShowFolderContentItem(QString path);
//    void addFolderItemInTree(QString parentPath, QString name);
//    void deleteFolderItemInTree(QString path);
//    void moveFolderItemInTree(QString prePath, QString newPath);
//    void copyByFileList(QList<QString> filePaths);
//    void pasteItemWithoutPasteFile(QList<QString> clipBoardTranscriptFolderNames);
//    void removeClipBoardRenameItem(QString path);

//private:
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    bool dragDropItem() override;
//    bool moveItemFromFileList(const QMimeData *mimeData) override;
//    void deleteOne(QTreeWidgetItem *item) override;
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    void pasteEnd() override;
//    void loadMaterial(QString filePath);
//    QList<QString> clipBoardFilePaths;
//    //用于存粘贴的副本文件夹名字 用于高亮显示
//    QList<QString> pastedFolderNames;
//    //有时候加载工程 却无需加载材质
//    bool isLoadMaterial;
//};
