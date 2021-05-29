#ifndef BBFILELISTWIDGET_H
#define BBFILELISTWIDGET_H


#include <QListWidget>
#include <QPlainTextEdit>
#include <BBUtils.h>


class QWidgetAction;


enum BBFileType
{
    dir = 0,
    mesh = 1,
    audio = 2,
    texture = 3,
    material = 4,
    script = 5,
    other = 6
};


struct BBFileInfo
{
    QString m_FileName;
    BBFileType m_eFileType;

    BBFileInfo(const QString &fileName, const BBFileType &fileType)
    {
        m_FileName = fileName;
        m_eFileType = fileType;
    }
};


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

    static QList<QString> m_MeshSuffixs;
    static QList<QString> m_TextureSuffixs;
    static QList<QString> m_AudioSuffixs;
    static QList<QString> m_ScriptSuffixs;
    static QList<QString> m_MaterialSuffixs;

private slots:
    void showFolderContent(const QString &folderPath);
    void doubleClickItem(QListWidgetItem *pItem);
    void newFolder();
    void copyAction();
    void pasteAction();
    void openRenameEditor();
    void finishRename();
    void deleteAction();

signals:
    void clickFolderTreeItem(const QString &folderPath);
    void addItemInFolderTree(const QString &parentPath, const QString &name);
    void updateItemInFolderTree(const QString &oldName, const QString &newName);
    void deleteItemInFolderTree(const QString &folderPath);
    void updateFolderTree();
    void createMeshOverviewMap(const QString &sourcePath, const QString &overviewMapPath);

private:
    void setMenu();
    QWidgetAction* createWidgetAction(const QString &iconPath, const QString &name);
    void deleteOne(QListWidgetItem *pItem);

    QString lineFeed(QString originalText);
    QIcon getIcon(const QString &path);
    QIcon getTextureIcon(const QString &path);
    QColor getFileLogoColor(const BBFileType &eFileType);

    void startDrag(Qt::DropActions supportedActions) override;
    bool moveItem();
    bool moveItemFromFolderTree(const QMimeData *pMimeData);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void importAsset(const QList<QUrl> &urls);
    void importAsset(const QFileInfo &fileInfo, const QString &newPath);
    QIcon getMeshOverviewMap(const QString &sourcePath);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

    static QSize m_StandardIconSize;
    static QSize m_StandardItemSize;

    static QString m_MeshFileLogoColor;
    static QString m_TextureFileLogoColor;
    static QString m_AudioFileLogoColor;
    static QString m_MaterialFileLogoColor;

    QMenu *m_pMenu;
    QMap<QListWidgetItem*, BBFileInfo*> m_Map;
    QSize m_ItemSize;
    QString m_FolderPath;
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
//    void updateFolderName(QString prePath, QString newPath);
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
