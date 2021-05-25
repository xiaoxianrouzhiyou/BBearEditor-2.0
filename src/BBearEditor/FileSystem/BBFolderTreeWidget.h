#ifndef BBFOLDERTREEWIDGET_H
#define BBFOLDERTREEWIDGET_H

#include "BBTreeWidget.h"
#include <QFileInfo>


class BBFolderTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBFolderTreeWidget(QWidget *pParent = nullptr);

    loadProject();

signals:

private slots:

private:
    QString getFileSuffix(QFileInfo fileInfo);

};

#endif // BBFOLDERTREEWIDGET_H





//class ProjectTree : public BaseTree
//{
//    Q_OBJECT

//public:
//    ProjectTree(QWidget *parent = 0);
//    QString getMimeType() override;
//    static bool copyDirectoryFiles(QString fromDir, QString toDir);

//signals:
//    void showFolderContent(QString folderPath);
//    void updateFolderName(QString prePath, QString newPath);
//    void deleteFileItemInList();
//    void copyToFileList(QList<QString> folderPaths);
//    void pasteFile(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void cancelHierarchyTreeSelectedItems();
//    void clearPropertyWidget();

//public slots:
//    void loadProject();
//    void copyAction() override;
//    void pasteAction() override;

//private slots:
//    void pressRootButton();
//    void pressSettingButton();
//    void itemClickedSlot(QTreeWidgetItem *item, int column);
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
//    QString getFilePath(QString relativeLocation);
//    void initMenu();
//    QWidgetAction *createWidgetAction(QMenu *parent, QString iconPath, QString name);
//    void deleteOne(QTreeWidgetItem *item) override;
//    QTreeWidgetItem *getItemByPath(QString path);
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    void pasteEnd() override;
//    void loadMaterial(QString filePath);
//    QTreeWidgetItem *currentShowFolderContentItem;
//    QList<QString> clipBoardFilePaths;
//    //用于存粘贴的副本文件夹名字 用于高亮显示
//    QList<QString> pastedFolderNames;
//    //有时候加载工程 却无需加载材质
//    bool isLoadMaterial;
//};
