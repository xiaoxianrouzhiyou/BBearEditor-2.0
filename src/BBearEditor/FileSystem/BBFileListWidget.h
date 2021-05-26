#ifndef BBFILELISTWIDGET_H
#define BBFILELISTWIDGET_H


#include <QListWidget>


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


class BBFileListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit BBFileListWidget(QWidget *pParent = nullptr);
    ~BBFileListWidget();

private slots:
    void showFolderContent(const QString &folderPath);

signals:

private:
    void setMenu();
    QWidgetAction* createWidgetAction(const QString &iconPath, const QString &name);

    QString lineFeed(QString originalText);
    QIcon getIcon(const QString &path);
    QIcon getTextureIcon(const QString &path);
    QColor getFileLogoColor(const BBFileType &eFileType);

    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    static QSize m_StandardIconSize;
    static QSize m_StandardItemSize;

    static QList<QString> m_MeshSuffixs;
    static QList<QString> m_TextureSuffixs;
    static QList<QString> m_AudioSuffixs;
    static QList<QString> m_ScriptSuffixs;

    static QString m_MeshFileLogoColor;
    static QString m_TextureFileLogoColor;
    static QString m_AudioFileLogoColor;
    static QString m_MaterialFileLogoColor;

    QMenu *m_pMenu;
    QMap<QListWidgetItem*, BBFileInfo*> m_Map;
    QSize m_ItemSize;
};

#endif // BBFILELISTWIDGET_H




////重写多行文本编辑框 捕获焦点移除、输入回车
//class PlainTextEdit : public QPlainTextEdit
//{
//    Q_OBJECT

//public:
//    PlainTextEdit(QWidget *parent = 0);
//signals:
//    void editingFinished();
//private:
//    void focusOutEvent(QFocusEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;
//};






//class FileList : public QListWidget
//{
//    Q_OBJECT

//public:
//    FileList(QWidget *parent = 0);
//    static QString getMimeType();
//    static QString getMetaFilePath(QString sourcePath);
//    static QString getMetaJpgFilePath(QString sourcePath);
//    static QString checkFolderDuplicateName(QString path);
//    static QString checkFileDuplicateName(QString path);


//public slots:
//    void popupMenu();
//    void newMaterial();
//    void newScript();
//    void copyAction();
//    void pasteAction();
//    void cancelSelectedItems();

//private slots:
//    void updateFolderName(QString prePath, QString newPath);
//    void itemDoubleClickedSlot(QListWidgetItem *item);
//    void openEditor();
//    void finishRename();
//    void newFolder();
//    void deleteFile();
//    void deleteItem();
//    void copyByProjectTree(QList<QString> folderPaths);
//    void pasteFileFromProjectTree(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames);
//    void changeItemSize(int factor);
//    void itemClickedSlot(QListWidgetItem* item);
//    void updateMaterialFileIcon(QString filePath);

//signals:
//    void updateProjectTree();
//    void showFolderPath(QString path);
//    void updateFolderNameInvert(QString preName, QString newName);
//    void updateCurrentShowFolderContentItem(QString path);
//    void addItemInProjectTree(QString parentPath, QString name);
//    void deleteItemInProjectTree(QString path);
//    void moveItemInProjectTree(QString prePath, QString newPath);
//    void copyToProjectTree(QList<QString> filePaths);
//    void pasteItemInProjectTree(QList<QString> clipBoardTranscriptFolderNames);
//    void removeRenameItemInTreeClipBoard(QString path);
//    void cancelHierarchyTreeSelectedItems();
//    void showMaterialProperty(QString filePath);
//    void showFbxProperty(QString filePath);
//    void clearPropertyWidget();

//private:
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
//    void dropEvent(QDropEvent *event) override;
//    static QString getProjectTreeMimeType();
//    void dropExternalAsset(QList<QUrl> urls);
//    void moveItems();
//    bool moveItemFromProjectTree(const QMimeData *mimeData);
//    void importAsset(QFileInfo fileInfo, QString newPath);
//    void createMeshMetaFile(QString sourcePath);
//    void startDrag(Qt::DropActions supportedActions) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    QWidgetAction *createWidgetAction(QString iconPath, QString name);
//    void keyPressEvent(QKeyEvent *event) override;
//    void focusInEvent(QFocusEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void selectPasteItem(QList<QString> itemNames);

//    QString mFolderPath;
//    QMenu *menu;
//    QListWidgetItem *editingItem;
//    PlainTextEdit *edit;
//    QListWidgetItem *indicatorItem;
//    QList<QString> clipBoardPaths;

//};
