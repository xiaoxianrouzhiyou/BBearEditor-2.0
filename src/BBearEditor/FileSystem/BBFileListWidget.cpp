#include "BBFileListWidget.h"
#include <QDir>
#include <QPainter>
#include <QWidgetAction>
#include <QMenu>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include "Window/BBConfirmationDialog.h"
#include <QMimeData>
#include <QScrollBar>
#include <QQueue>
#include <QDrag>


//---------------------------------------------------------------------------------------------------
//  BBPlainTextEdit
//---------------------------------------------------------------------------------------------------

BBPlainTextEdit::BBPlainTextEdit(QWidget *pParent)
    : QPlainTextEdit(pParent)
{

}

void BBPlainTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    editFinished();
}

void BBPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        editFinished();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}


//---------------------------------------------------------------------------------------------------
//  BBFileListWidget
//---------------------------------------------------------------------------------------------------

QList<QString> BBFileListWidget::m_MeshSuffixs = {"obj", "fbx"};
QList<QString> BBFileListWidget::m_TextureSuffixs = {"png", "jpg", "jpeg", "bmp", "ico", "dds"};
QList<QString> BBFileListWidget::m_AudioSuffixs = {"mp3", "wav"};
QList<QString> BBFileListWidget::m_ScriptSuffixs = {"lua"};
QList<QString> BBFileListWidget::m_MaterialSuffixs = {"mtl"};

QSize BBFileListWidget::m_StandardIconSize = QSize(43, 43);
QSize BBFileListWidget::m_StandardItemSize = QSize(45, 90);
//QSize BBFileListWidget::m_StandardIconSize = QSize(53, 53);
//QSize BBFileListWidget::m_StandardItemSize = QSize(55, 100);

QString BBFileListWidget::m_MeshFileLogoColor = "#e85655";
QString BBFileListWidget::m_TextureFileLogoColor = "#e49831";
QString BBFileListWidget::m_AudioFileLogoColor = "#64abe4";
QString BBFileListWidget::m_MaterialFileLogoColor = "#fab8b7";


BBFileListWidget::BBFileListWidget(QWidget *pParent)
    : QListWidget(pParent)
{
    m_ItemSize = m_StandardItemSize;
    m_pEditingItem = NULL;
    m_pRenameEditor = NULL;
    m_pIndicatorItem = NULL;

    // icon is at the top, and text is at the bottom
    setViewMode(QListView::IconMode);
    // Dynamically adjust the layout position according to the size of the list container
    // From left to right and from top to bottom
    setResizeMode(QListView::Adjust);
    setIconSize(m_StandardIconSize);
    setSpacing(10);
    // Text can be showed in multiple lines
    setWordWrap(true);
    setAcceptDrops(true);
    setDragEnabled(true);
    // You can press shift and cmd to select multiple
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setMenu();

    QObject::connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(doubleClickItem(QListWidgetItem*)));
//    QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
//                     this, SLOT(itemClickedSlot(QListWidgetItem*)));
}

BBFileListWidget::~BBFileListWidget()
{
    BB_SAFE_DELETE(m_pMenu);
}

void BBFileListWidget::showFolderContent(const QString &folderPath)
{
    // clear map, and delete BBFileInfo
    while (!m_Map.isEmpty())
    {
        BB_SAFE_DELETE(m_Map.first());
        m_Map.remove(m_Map.firstKey());
    }
    // show the contents of the newly selected folder, the original list is cleared
    clear();

    m_FolderPath = folderPath;
//    //向显示当前路径的控件发送路径
//    showFolderPath(mFolderPath);
//    //向文件夹树发送信号 修改当前正在显示的文件夹对应的树节点
//    updateCurrentShowFolderContentItem(mFolderPath);

    QDir dir(folderPath);
    if (dir.exists())
    {
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach (QFileInfo fileInfo, fileInfoList)
        {
            QListWidgetItem *pItem = new QListWidgetItem(this);
            pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
            pItem->setSizeHint(m_ItemSize);
            if (fileInfo.isDir())
            {
                // is folder
                pItem->setText(lineFeed(fileInfo.fileName()));
                pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));
                m_Map.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::dir));
            }
            else
            {
                // is file
                pItem->setText(lineFeed(fileInfo.baseName()));
                QString suffix = fileInfo.suffix();
                if (m_MeshSuffixs.contains(suffix))
                {
                    QString sourcePath = fileInfo.absoluteFilePath();
                    QIcon icon = getMeshOverviewMap(sourcePath);
                    pItem->setIcon(icon);
                    m_Map.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::mesh));
                }
                else if (m_TextureSuffixs.contains(suffix))
                {
                    // Picture files use themselves as icons
                    pItem->setIcon(getTextureIcon(fileInfo.absoluteFilePath()));
                    // save map
                    m_Map.insert(pItem, new BBFileInfo(fileInfo.fileName(), BBFileType::texture));
                }

//                else if (audioSuffixs.contains(suffix))
//                {
//                    item->setIcon(getIcon(":/icon/resources/pictures/audio.jpg"));
//                    mMap.insert(item, new FileInfo(fileInfo.fileName(), FileType::audio));
//                }
//                else if (suffix == "mtl")
//                {
//                    Material *material = Material::mtlMap.value(fileInfo.absoluteFilePath());
//                    item->setIcon(QIcon(material->getPreview()));
//                    mMap.insert(item, new FileInfo(fileInfo.fileName(), FileType::material));
//                }
//                else if (scriptSuffixs.contains(suffix))
//                {
//                    item->setIcon(getIcon(":/icon/resources/icons/lua.png"));
//                    mMap.insert(item, new FileInfo(fileInfo.fileName(), FileType::script));
//                }
//                else
//                {
//                    mMap.insert(item, new FileInfo(fileInfo.fileName(), FileType::other));
//                }
            }
        }
    }
    else
    {
        dir.mkpath(dir.absolutePath());
    }
    sortItems();
}

void BBFileListWidget::doubleClickItem(QListWidgetItem *pItem)
{
    // pItem->text() is text after line feed, is not real name
    BBFileInfo *pInfo = m_Map.value(pItem);
    QString filePath = m_FolderPath + "/" + pInfo->m_FileName;
    QFileInfo fileInfo(filePath);
    if (fileInfo.isDir())
    {
        clickFolderTreeItem(filePath);
    }
    else
    {
        if (pInfo->m_eFileType == BBFileType::script)
        {
            // open file
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    }
}

void BBFileListWidget::newFolder()
{
    // Otherwise, after creating, several items will be selected
    QList<QListWidgetItem*> items = selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        setItemSelected(items.at(i), false);
    }

    QString fileName = "new folder";
    QString filePath = BBUtils::getExclusiveFolderPath(m_FolderPath, fileName);

    QDir dir;
    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath));

    // add item at the beginning
    QListWidgetItem *pItem = new QListWidgetItem(this);
    pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    pItem->setSizeHint(m_ItemSize);
    pItem->setText(lineFeed(fileName));
    pItem->setIcon(getIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));

    m_Map.insert(pItem, new BBFileInfo(fileName, BBFileType::dir));

    sortItems();
    addItemInFolderTree(m_FolderPath, fileName);
    setCurrentItem(pItem);
    openRenameEditor();
}

void BBFileListWidget::copyAction()
{

}

void BBFileListWidget::pasteAction()
{

}

void BBFileListWidget::openRenameEditor()
{
    QList<QListWidgetItem*> items = selectedItems();
    if (items.count() == 1)
    {
        m_pEditingItem = items.first();
        QWidget *pWidget = new QWidget(this);
        QVBoxLayout *pLayout = new QVBoxLayout(pWidget);
        pLayout->setContentsMargins(1, 2, 1, 2);
        pLayout->setSpacing(3);
        m_pRenameEditor = new BBPlainTextEdit(pWidget);
        m_pRenameEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // margin: 47px 1px 2px 1px;
        m_pRenameEditor->setStyleSheet("border: none; background: #d6dfeb; color: #191f28;"
                                       "selection-color: #d6dfeb; selection-background-color: #8193bc;"
                                       "font: 9pt \"Arial\"; padding: -2px;");
        BBFileInfo *pFileInfo = m_Map.value(m_pEditingItem);
        QString fileName = pFileInfo->m_FileName;
        if (pFileInfo->m_eFileType != BBFileType::dir)
        {
            // remove suffix
            fileName = fileName.mid(0, fileName.lastIndexOf('.'));
        }
        m_pRenameEditor->setPlainText(fileName);
        m_pRenameEditor->selectAll();
        QObject::connect(m_pRenameEditor, SIGNAL(editFinished()), this, SLOT(finishRename()));
        pLayout->addWidget(m_pRenameEditor, 1);
        setItemWidget(m_pEditingItem, pWidget);
        m_pRenameEditor->setFocus();
    }
}

void BBFileListWidget::finishRename()
{
    BB_PROCESS_ERROR_RETURN(m_pEditingItem);

    // without suffix
    QString newBaseName = m_pRenameEditor->toPlainText();
    if (!newBaseName.isEmpty())
    {
        QString newName = newBaseName;
        BBFileInfo *pFileInfo = m_Map.value(m_pEditingItem);
        QString oldName = pFileInfo->m_FileName;
        QString oldBaseName = oldName;
        if (pFileInfo->m_eFileType != BBFileType::dir)
        {
            // add suffix
            QString suffix = BBUtils::getFileSuffix(oldName);
            newName = newBaseName + "." + suffix;
            oldBaseName = BBUtils::getBaseName(oldName);
        }
        if (oldName != newName)
        {
            QString oldPath = m_FolderPath + "/" + oldName;
            QString newPath;
            if (pFileInfo->m_eFileType == BBFileType::dir)
            {
                newPath = BBUtils::getExclusiveFolderPath(m_FolderPath, newName);
                if (QFile::rename(oldPath, newPath))
                {
                    newBaseName = BBUtils::getBaseName(newName);
                    pFileInfo->m_FileName = newName;
                    m_pEditingItem->setText(lineFeed(newBaseName));
//                    //修改对应meta文件夹
//                    QString oldMetaFolderPath = getMetaFilePath(oldPath);
//                    QString newMetaFolderPath = getMetaFilePath(newPath);
//                    QFile::rename(oldMetaFolderPath, newMetaFolderPath);
                    // update corresponding item in the folder tree
                    updateItemInFolderTree(oldName, newName);
                }
//                //如果该文件夹在剪贴板中 则也在文件夹树的剪贴板中 自己和树的剪贴板都要移除
//                if (clipBoardPaths.contains(oldPath))
//                {
//                    clipBoardPaths.removeOne(oldPath);
//                    //树结点的名字已经改了 需要使用新路径找对应结点
//                    removeRenameItemInTreeClipBoard(newPath);
//                }
            }
            else
            {
                // file
                newPath = BBUtils::getExclusiveFilePath(m_FolderPath, newName);
                if (QFile::rename(oldPath, newPath))
                {
                    newBaseName = BBUtils::getBaseName(newName);
                    pFileInfo->m_FileName = newName;
                    m_pEditingItem->setText(lineFeed(newBaseName));

                    if (pFileInfo->m_eFileType == BBFileType::mesh)
                    {
//                        //有些文件需要对相应meta修改名字
//                        QString oldMetaFilePath = getMetaFilePath(oldPath);
//                        QString newMetaFilePath = getMetaFilePath(newPath);
//                        //修改后缀为jpg
//                        int index = oldMetaFilePath.lastIndexOf('.');
//                        oldMetaFilePath = oldMetaFilePath.mid(0, index) + ".jpg";
//                        index = newMetaFilePath.lastIndexOf('.');
//                        newMetaFilePath = newMetaFilePath.mid(0, index) + ".jpg";
//                        QFile::rename(oldMetaFilePath, newMetaFilePath);
                    }
                    else if (pFileInfo->m_eFileType == BBFileType::material)
                    {
//                        //材质文件 需要修改文件路径与材质对象的映射
//                        Material::rename(oldPath, newPath);
                    }
                }
//                //重命名后 如果在剪贴板中 移除掉
//                if (clipBoardPaths.contains(oldPath))
//                {
//                    clipBoardPaths.removeOne(oldPath);
//                }
            }
            sortItems();
        }
    }

    removeItemWidget(m_pEditingItem);
    m_pEditingItem = NULL;
    // Otherwise, the list has no focus and no longer triggers key events
    setFocus();
//    //重新显示属性栏的属性 名字更新
//    itemClicked(editingItem);
}

void BBFileListWidget::deleteAction()
{
    QList<QListWidgetItem*> items = selectedItems();
    BB_PROCESS_ERROR_RETURN(items.count() > 0);

    BBConfirmationDialog dialog;
    dialog.setTitle("Delete selected?");
    if (items.count() == 1)
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete this?");
    }
    else
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete these "
                          + QString::number(items.count()) + " items?");
    }
    if (dialog.exec())
    {
        for (int i = 0; i < items.count(); i++)
        {
            QListWidgetItem* pItem = items.at(i);
            BBFileInfo *pFileInfo = m_Map.value(pItem);
            QString path = m_FolderPath + "/" + pFileInfo->m_FileName;
            if (pFileInfo->m_eFileType == BBFileType::dir)
            {
                QDir dir(path);
                BB_PROCESS_ERROR_RETURN(dir.removeRecursively());
//                //删除对应的meta文件夹
//                dir = new QDir(getMetaFilePath(path));
//                dir->removeRecursively();
                // delete the corresponding item in the folder tree
                deleteItemInFolderTree(path);
//                //刷新材质文件的映射 被删除的材质文件的映射不再占用内存
//                Material::updateMap();
            }
            else
            {
                BB_PROCESS_ERROR_RETURN(QFile::remove(path));
//                //有些文件需要删除对应meta
//                if (fileInfo->mFileType == FileType::mesh)
//                {
//                    QString metaFilePath = getMetaFilePath(path);
//                    //修改后缀为jpg
//                    int index = metaFilePath.lastIndexOf('.');
//                    metaFilePath = metaFilePath.mid(0, index) + ".jpg";
//                    QFile::remove(metaFilePath);
//                }
//                //材质文件 需要删除材质对象
//                else if (fileInfo->mFileType == FileType::material)
//                {
//                    Material::deleteOne(path);
//                }
            }
            // remove from clipboard
//            if (clipBoardPaths.contains(path))
//            {
//                clipBoardPaths.removeOne(path);
//            }
            m_Map.remove(pItem);
            BB_SAFE_DELETE(pFileInfo);
            BB_SAFE_DELETE(pItem);
        }
        setCurrentItem(NULL);
//        //清空属性栏 包括场景 层级视图选中
//        cancelHierarchyTreeSelectedItems();
//        itemClickedSlot(NULL);
    }
}

void BBFileListWidget::setMenu()
{
    // first level menu
    m_pMenu = new QMenu(this);
    QAction *pActionNewFolder = new QAction(tr("New Folder"));
    QAction *pLabelNewAsset = new QAction(tr("New Asset ..."));
    // As the title of a column, not clickable
    pLabelNewAsset->setEnabled(false);

    QWidgetAction *pActionNewMaterial = createWidgetAction(QString(BB_PATH_RESOURCE_ICON) + "material.png", tr("Material"));
    QWidgetAction *pActionNewScript = createWidgetAction(QString(BB_PATH_RESOURCE_ICON) + "script.png", tr("Script"));

    QAction *pActionShowInFolder = new QAction(tr("Show In Folder"));
    QAction *pActionCopy = new QAction(tr("Copy"));
    pActionCopy->setShortcut(QKeySequence(tr("Ctrl+C")));
    QAction *pActionPaste = new QAction(tr("Paste"));
    pActionPaste->setShortcut(QKeySequence(tr("Ctrl+V")));
    QAction *pActionRename = new QAction(tr("Rename"));
#if defined(Q_OS_WIN32)
    pActionRename->setShortcut(Qt::Key_F2);
#elif defined(Q_OS_MAC)
    pActionRename->setShortcut(Qt::Key_Return);
#endif
    QAction *pActionDelete = new QAction(tr("Delete"));
    // second level menu
    QMenu *pMenuSort = new QMenu(tr("Sort By"), m_pMenu);
    QAction *pActionNameSort = new QAction(tr("Name"));
    QAction *pActionTypeSort = new QAction(tr("Type"));
    QAction *pActionCreateDateSort = new QAction(tr("Create Date"));
    QAction *pActionModifyDateSort = new QAction(tr("Modify Date"));
    pMenuSort->addAction(pActionNameSort);
    pMenuSort->addAction(pActionTypeSort);
    pMenuSort->addAction(pActionCreateDateSort);
    pMenuSort->addAction(pActionModifyDateSort);
    // slider
    QWidgetAction *pActionSlider = new QWidgetAction(m_pMenu);
    QWidget *pWidget = new QWidget(this);
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setContentsMargins(6, 1, 6, 1);
    QLabel *pLabel = new QLabel("Size", pWidget);
    pLabel->setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\";");
    pLayout->addWidget(pLabel);
    QSlider *pSlider = new QSlider(Qt::Horizontal, pWidget);
    pSlider->setRange(-2, 2);
    pSlider->setValue(0);
    QObject::connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(changeItemSize(int)));
    pLayout->addWidget(pSlider);
    pActionSlider->setDefaultWidget(pWidget);

    m_pMenu->addAction(pActionNewFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pLabelNewAsset);
    m_pMenu->addAction(createWidgetAction(QString(BB_PATH_RESOURCE_ICON) + "scene.png", tr("Scene")));
    m_pMenu->addAction(pActionNewMaterial);
    m_pMenu->addAction(createWidgetAction(QString(BB_PATH_RESOURCE_ICON) + "animation.png", tr("Animation")));
    m_pMenu->addAction(createWidgetAction(QString(BB_PATH_RESOURCE_ICON) + "particle.png", tr("Particle")));
    m_pMenu->addAction(pActionNewScript);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionShowInFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionCopy);
    m_pMenu->addAction(pActionPaste);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionRename);
    m_pMenu->addAction(pActionDelete);
    m_pMenu->addSeparator();
    m_pMenu->addMenu(pMenuSort);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionSlider);

    QObject::connect(pActionNewFolder, SIGNAL(triggered()), this, SLOT(newFolder()));
    QObject::connect(pActionNewMaterial, SIGNAL(triggered()), this, SLOT(newMaterial()));
    QObject::connect(pActionNewScript, SIGNAL(triggered()), this, SLOT(newScript()));
    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openRenameEditor()));
    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

QWidgetAction* BBFileListWidget::createWidgetAction(const QString &iconPath, const QString &name)
{
    QWidgetAction *pAction = new QWidgetAction(m_pMenu);
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName("widgetAction");
    pWidget->setStyleSheet("#widgetAction:hover {background: #0ebf9c;}");
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setContentsMargins(6, 2, 6, 2);
    QLabel *pIcon = new QLabel(pWidget);
    QPixmap pix(iconPath);
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(13 * devicePixelRatio(), 13 * devicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pIcon->setPixmap(pix);
    pLayout->addWidget(pIcon);
    QLabel *pText = new QLabel(pWidget);
    pText->setText(name);
    pText->setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\";");
    pLayout->addWidget(pText, Qt::AlignLeft);
    pAction->setDefaultWidget(pWidget);
    return pAction;
}

QString BBFileListWidget::lineFeed(QString originalText)
{
    // 40 corresponds to QSize(45, 90)
    QFontMetrics fm = fontMetrics();
    QString lineFeedText;
    int nHeadIndex = 0;
    for (int i = 0; i < originalText.length(); i++)
    {
        if (fm.width(originalText.mid(nHeadIndex, i + 1 - nHeadIndex)) < m_ItemSize.width() - 5)
        {
            // The width does not exceed 40, no need to line feed
            lineFeedText += originalText.at(i);
        }
        else
        {
            // Line feed
            // The index of the first character is set to the index of the first character of the next line
            lineFeedText += "\n" + originalText[i];
            nHeadIndex = i;
        }
    }
    return lineFeedText;
}

QIcon BBFileListWidget::getIcon(const QString &path)
{
    // Cut into a square
    QPixmap pix(path);
    if (pix.isNull())
    {
        return QIcon(QString(BB_PATH_RESOURCE_ICON) + "empty2");
    }
    else
    {
        int h = pix.height();
        int w = pix.width();
        int size = h < w ? h : w;
        pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);
        return QIcon(pix);
    }
}

QIcon BBFileListWidget::getTextureIcon(const QString &path)
{
    QPixmap pix(path);
    int h = pix.height();
    int w = pix.width();
    int size = h < w ? h : w;
    pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);
    // Transparent pictures need to add background
    // When the image is smaller than the icon size, use the icon size. The image is showed in the center
    int nBackgroundSize = size > iconSize().width() ? size : iconSize().width();
    QPixmap background(nBackgroundSize, nBackgroundSize);
    background.fill(QColor("#d6dfeb"));
    QPainter painter(&background);
    painter.drawPixmap((nBackgroundSize - size) / 2, (nBackgroundSize - size) / 2, pix);
    painter.end();
    return QIcon(background);
}

QColor BBFileListWidget::getFileLogoColor(const BBFileType &eFileType)
{
    if (eFileType == BBFileType::mesh)
    {
        return QColor(m_MeshFileLogoColor);
    }
    else if (eFileType == BBFileType::texture)
    {
        return QColor(m_TextureFileLogoColor);
    }
    else if (eFileType == BBFileType::audio)
    {
        return QColor(m_AudioFileLogoColor);
    }
    else if (eFileType == BBFileType::material)
    {
        return QColor(m_MaterialFileLogoColor);
    }
    else
    {
        return nullptr;
    }
}

void BBFileListWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);
    QListWidgetItem *pCenterItem = currentItem();
    QRect centerItemRect = visualItemRect(pCenterItem);
    // Used to calculate the absolute position of each item that needs to be dragged
    int hPos = horizontalScrollBar()->sliderPosition();
    int vPos = verticalScrollBar()->sliderPosition();
    // step of each item
    int hStep = centerItemRect.width() + spacing();
    int vStep = centerItemRect.height() + spacing();
    // The size of a single icon
    static int nSize = iconSize().width() * devicePixelRatio();
    // Record the row and column value and icon of the selected item
    struct Info
    {
        int nRow;
        int nColumn;
        QPixmap icon;
    };
    QList<Info> infos;
    // Used for the size of final icon
    int nMaxRow = 0;
    int nMaxColumn = 0;
    int nMinRow = INT_MAX;
    int nMinColumn = INT_MAX;
    // drag all selected items
    QList<QListWidgetItem*> items = selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        Info info;
        QListWidgetItem *pItem = items.at(i);
        QRect rect = visualItemRect(pItem);
        // rect is relative to the position of the scroll bar
        // translate and make it relative to 0
        rect.translate(hPos - spacing(), vPos - spacing());
        // Calculate the row and column value of the item based on the step
        info.nColumn = rect.left() / hStep;
        info.nRow = rect.top() / vStep;
        if (info.nColumn > nMaxColumn)
        {
            nMaxColumn = info.nColumn;
        }
        if (info.nColumn < nMinColumn)
        {
            nMinColumn = info.nColumn;
        }
        if (info.nRow > nMaxRow)
        {
            nMaxRow = info.nRow;
        }
        if (info.nRow < nMinRow)
        {
            nMinRow = info.nRow;
        }
        info.icon = pItem->icon().pixmap(nSize);
        info.icon.setDevicePixelRatio(devicePixelRatio());
        info.icon = info.icon.scaled(nSize, nSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        infos.append(info);
    }
    QPoint hotSpot;
    // final icon
    QPixmap pixmap((nSize + 6) * (nMaxColumn - nMinColumn + 1), (nSize + 6) * (nMaxRow - nMinRow + 1));
    pixmap.setDevicePixelRatio(devicePixelRatio());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    // paint the icon of each item to the final icon according to the row and column value of each item
    for (int i = 0; i < infos.count(); i++)
    {
        int x = (infos.at(i).nColumn - nMinColumn) * (nSize + 6) / devicePixelRatio();
        int y = (infos.at(i).nRow - nMinRow) * (nSize + 6) / devicePixelRatio();
        painter.drawPixmap(x, y, infos.at(i).icon);
        if (items.at(i) == pCenterItem)
        {
            // Take the center of pCenterItem as the mouse position
            hotSpot.setX(x + nSize / devicePixelRatio() / 2);
            hotSpot.setY(y + nSize / devicePixelRatio() / 2);
        }
    }
    painter.end();
    // Pack relevant data
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    // The pCenterItem is written in the first one, used for the situation that you just can read one item
    QString filePath = m_FolderPath + "/" + m_Map.value(pCenterItem)->m_FileName;
    dataStream << filePath;
    // all selected items is written, used for the situation that you can read all selected items
    for (int i = 0; i < items.count(); i++)
    {
        filePath = m_FolderPath + "/" + m_Map.value(items.at(i))->m_FileName;
        dataStream << filePath;
    }
    QMimeData *pMimeData = new QMimeData;
    // Give this data a unique identifying name
    pMimeData->setData(getMimeType(), itemData);
    QDrag drag(this);
    drag.setMimeData(pMimeData);
    drag.setHotSpot(hotSpot);
    drag.setPixmap(pixmap);
    drag.exec(Qt::MoveAction);
}

bool BBFileListWidget::moveItem()
{

}

bool BBFileListWidget::moveItemFromFolderTree(const QMimeData *pMimeData)
{

}

void BBFileListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->urls().isEmpty())
    {
        // the outside of the editor
        event->accept();
    }
    else if (event->mimeData()->hasFormat(getMimeType()))
    {
        // internal drag
        event->accept();
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void BBFileListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    // Scroll up and down when the mouse is at the top or bottom
    // QListWidget::dragMoveEvent(event); invalidates dropEvent
    int y = event->pos().y();
    if (y < 10)
    {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - 10);
    }
    else if (y >= height() - 10)
    {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() + 10);
    }
    // Drop position indicator
    QListWidgetItem *pItem = itemAt(event->pos());
    // Clear the last indicator position painted
    m_pIndicatorItem = NULL;
    if (pItem)
    {
        // only folder can show drop indicator
        BBFileInfo *pFileInfo = m_Map.value(pItem);
        if (pFileInfo->m_eFileType == BBFileType::dir)
        {
            // When dragging internally, the drop position cannot be the folder being dragged
            // It doesn't matter if it is dragged from the folder tree, since the item has been judged
            if ((event->mimeData()->hasFormat(getMimeType()) && !selectedItems().contains(pItem))
                    || event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
            {
                m_pIndicatorItem = pItem;
            }
        }
    }
    repaint();
}

void BBFileListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    // No need for indicator when dragLeave
    m_pIndicatorItem = NULL;
    repaint();
}

void BBFileListWidget::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->urls().isEmpty())
    {
        // from outside of the editor
        event->accept();
        importAsset(event->mimeData()->urls());
    }
    else if (event->mimeData()->hasFormat(getMimeType()))
    {
        if (m_pIndicatorItem)
        {
            // there is a drop spot
            event->accept();
            moveItem();
        }
        else
        {
            event->ignore();
        }
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
    {
        if (moveItemFromFolderTree(event->mimeData()))
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
    // The indicator is no longer needed after drop
    m_pIndicatorItem = NULL;
    repaint();
}

void BBFileListWidget::importAsset(const QList<QUrl> &urls)
{
    QString folderPath = m_FolderPath;
    if (m_pIndicatorItem)
    {
        // Drag to the folder in the file list and import the asset into this folder
        folderPath += "/" + m_Map.value(m_pIndicatorItem)->m_FileName;
    }
    for (int i = 0; i < urls.length(); i++)
    {
        QString importedAssetPath = urls.at(i).toLocalFile();
        // if it is the folder, you need to traverse the sub-files
        // otherwise, directly operate
        QFileInfo fileInfo(importedAssetPath);
        if (fileInfo.isDir())
        {
            // create the folder
            QString rootFolderName = BBUtils::getFileNameByPath(importedAssetPath);
            QString rootFolderPath = BBUtils::getExclusiveFolderPath(folderPath, rootFolderName);

            QDir dir;
            BB_PROCESS_ERROR_RETURN(dir.mkpath(rootFolderPath));
            // breadth-first traverse
            QQueue<QString> queue;
            queue.enqueue(importedAssetPath);
            while (!queue.isEmpty())
            {
                QDir dir(queue.dequeue());
                dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
                // the folder exists, traverse sub-files
                QFileInfoList fileInfoList = dir.entryInfoList();
                foreach (QFileInfo fileInfo, fileInfoList)
                {
                    // Keep the path of the sub file relative to the root folder
                    // and replace with the project file path in front
                    // There is no duplicate name problem
                    QString childPath = rootFolderPath + fileInfo.absoluteFilePath().mid(importedAssetPath.length());
                    if (fileInfo.isDir())
                    {
                        // folder enqueue, waiting to traverse its sub-files
                        queue.enqueue(fileInfo.absoluteFilePath());
                        BB_PROCESS_ERROR_RETURN(dir.mkpath(childPath));
                    }
                    else
                    {
                        // handle the file
                        importAsset(fileInfo, childPath);
                    }
                }
            }
        }
        else
        {
            // handle the file
            QFileInfo fileInfo(importedAssetPath);
            importAsset(fileInfo, folderPath + "/" + fileInfo.fileName());
        }
    }
    // Open the asset import management dialog
    // BBAssetManager assetManager;
    // assetManager.exec();
    // there are new items, update file system
    showFolderContent(m_FolderPath);
    updateFolderTree();
}

void BBFileListWidget::importAsset(const QFileInfo &fileInfo, const QString &newPath)
{
    QString suffix = fileInfo.suffix();
    if (m_TextureSuffixs.contains(suffix) || m_ScriptSuffixs.contains(suffix))
    {
        // import directly
        QFile::copy(fileInfo.absoluteFilePath(), BBUtils::getExclusiveFilePath(newPath));
    }
    else if (m_MeshSuffixs.contains(suffix))
    {
        QString targetPath = BBUtils::getExclusiveFilePath(newPath);
        QFile::copy(fileInfo.absoluteFilePath(), targetPath);
        createMeshOverviewMap(targetPath, BBUtils::getOverviewMapPath(targetPath));
    }
}

QIcon BBFileListWidget::getMeshOverviewMap(const QString &sourcePath)
{
    // read icon from engine folder if it is created before
    // if it does not exist, create
    QString overviewMapPath = BBUtils::getOverviewMapPath(sourcePath);
    QFile file(overviewMapPath);
    if (!file.exists())
    {
        createMeshOverviewMap(sourcePath, overviewMapPath);
    }
    return getIcon(overviewMapPath);
}

void BBFileListWidget::paintEvent(QPaintEvent *event)
{
    QListWidget::paintEvent(event);

    // using "this" is wrong
    QPainter painter(viewport());
    // The file type logo color is painted in the top left corner
    for (int i = 0; i < count(); i++)
    {
        // Get the logo color of the corresponding file type of the item
        QColor color = getFileLogoColor(m_Map.value(item(i))->m_eFileType);
        if (color == nullptr)
            continue;
        QPen pen(color);
        painter.setPen(pen);
        // item position
        QRect rect = visualItemRect(item(i));
        for (int j = 8; j < 20; j++)
        {
            QPoint p1 = rect.topLeft() + QPoint(1, 2 + j);
            QPoint p2 = rect.topLeft() + QPoint(1 + j, 2);
            painter.drawLine(p1, p2);
        }
    }
    // paint drop indicator
    if (m_pIndicatorItem)
    {
        painter.setPen(QColor("#d6dfeb"));
        QRect rect = visualItemRect(m_pIndicatorItem);
        painter.drawRect(rect);
    }
    painter.end();
}

void BBFileListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        // There is no item at the mouse click position, remove the selection
        QListWidgetItem *pItem = itemAt(event->pos());
        if (!pItem)
        {
            setCurrentItem(NULL);
        }
    }
}

void BBFileListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    m_pMenu->show();
    QPoint pos = cursor().pos();
    // default: showed in bottom-right of cursor
    // when exceed screen in horizon, the right side of the menu and the right side of the screen are aligned
    if (pos.x() + m_pMenu->width() > QApplication::desktop()->width())
    {
        pos.setX(QApplication::desktop()->width() - m_pMenu->width());
    }
    // when exceed screen in vertical, showed in top-right, 4 is margin
    if (pos.y() + m_pMenu->height() > QApplication::desktop()->height() - 4)
    {
        pos.setY(pos.y() - m_pMenu->height());
    }
    m_pMenu->move(pos);
}

void BBFileListWidget::keyPressEvent(QKeyEvent *event)
{
    // Handling menu shortcut events
    int key;
#if defined(Q_OS_WIN32)
    key = Qt::Key_F2;
#elif defined(Q_OS_MAC)
    key = Qt::Key_Return;
#endif
    if (event->key() == key)
    {
        openRenameEditor();
    }
}

void BBFileListWidget::focusInEvent(QFocusEvent *event)
{
    // parent class, when the focus is obtained, the first item will show a blue box, which is ugly
    Q_UNUSED(event);
}

//void FileList::updateFolderName(QString prePath, QString newPath)
//{
//    //修改meta文件夹内对应文件夹的名字
//    QString newMetaFolderPath = newPath.mid((projectPath + contentsFolderName).length());
//    newMetaFolderPath = projectEngineFolderPath + contentMetaFolderName + newMetaFolderPath;
//    QString preMetaFolderPath = newMetaFolderPath.mid(0, newMetaFolderPath.lastIndexOf('/'));
//    int index = prePath.lastIndexOf('/');
//    QString preName = prePath.mid(index);
//    QString preOnlyPath = prePath.mid(0, index);
//    preMetaFolderPath += preName;
//    QFile::rename(preMetaFolderPath, newMetaFolderPath);

//    //如果修改名字的文件夹就是当前显示的文件夹
//    if (prePath == mFolderPath)
//    {
//        //用于list item的事件
//        mFolderPath = newPath;
//        //向显示当前路径的控件发送路径
//        showFolderPath(mFolderPath);
//    }
//    //如果当前显示的是修改名字文件夹的上一级
//    else if (preOnlyPath == mFolderPath)
//    {
//        //列表对应文件夹需要改变名字
//        QMap<QListWidgetItem*, FileInfo*>::Iterator itr;
//        for (itr = mMap.begin(); itr != mMap.end(); itr++)
//        {
//            FileInfo *fileInfo = itr.value();
//            if (fileInfo->mFileName == preName.mid(1))
//            {
//                QListWidgetItem *item = itr.key();
//                QString name = newPath.mid(newPath.lastIndexOf('/') + 1);
//                item->setText(lineFeed(name));
//                //映射里的文件名也要改
//                fileInfo->mFileName = name;
//                break;
//            }
//        }
//        sortItems();
//    }

//}

//void FileList::newMaterial()
//{
//    //否则创建完毕后 会有多项被选中
//    QList<QListWidgetItem*> items = selectedItems();
//    int count = items.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(items.at(i), false);
//    }
//    //创建基础材质文件 将基础材质文件从资源中拷贝到工程文件
//    static QString baseMtlFilePath = engineResourcesMaterialsPath + "base.mtl";
//    QString newMaterialFilePath = mFolderPath + "/new material.mtl";
//    newMaterialFilePath = checkFileDuplicateName(newMaterialFilePath);
//    QFile::copy(baseMtlFilePath, newMaterialFilePath);
//    //创建材质对象
//    Material *material = new Material(newMaterialFilePath);
//    //列表中显示新建的材质
//    //不带路径的名字
//    QString fileName = newMaterialFilePath.mid(newMaterialFilePath.lastIndexOf('/') + 1);
//    //在列表第一项插入item 表示材质文件
//    QListWidgetItem *item = new QListWidgetItem(this);
//    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
//    item->setSizeHint(itemSize);
//    //去掉后缀
//    item->setText(lineFeed(fileName.mid(0, fileName.lastIndexOf('.'))));
//    item->setIcon(QIcon(material->getPreview()));
//    //插入映射
//    mMap.insert(item, new FileInfo(fileName, FileType::material));
//    //列表重新排序
//    sortItems();
//    //选中新建项
//    setCurrentItem(item);
//    //属性栏显示该材质的属性 必须在setCurrentItem之后 否则选中项不一定一致
//    itemClickedSlot(item);
//    //打开重命名编辑框
//    openEditor();
//}

//void FileList::newScript()
//{
//    //否则创建完毕后 会有多项被选中
//    QList<QListWidgetItem*> items = selectedItems();
//    int count = items.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(items.at(i), false);
//    }
//    //创建基础脚本文件 将基础脚本文件从资源中拷贝到工程文件
//    static QString baseScriptPath = engineResourcesScriptsPath + "new script.lua";
//    QString newScriptPath = mFolderPath + "/new script.lua";
//    newScriptPath = checkFileDuplicateName(newScriptPath);
//    QFile::copy(baseScriptPath, newScriptPath);
//    //列表中显示新建的脚本
//    //不带路径的名字
//    QString fileName = newScriptPath.mid(newScriptPath.lastIndexOf('/') + 1);
//    //在列表第一项插入item 表示脚本文件
//    QListWidgetItem *item = new QListWidgetItem(this);
//    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
//    item->setSizeHint(itemSize);
//    //去掉后缀
//    item->setText(lineFeed(fileName.mid(0, fileName.lastIndexOf('.'))));
//    item->setIcon(getIcon(":/icon/resources/icons/lua.png"));
//    //插入映射
//    mMap.insert(item, new FileInfo(fileName, FileType::script));
//    //列表重新排序
//    sortItems();
//    //选中新建项
//    setCurrentItem(item);
//    //清空属性栏 （暂时不显示信息）
//    clearPropertyWidget();
//    //打开重命名编辑框
//    openEditor();
//}

//void FileList::copyAction()
//{
//    //复制节点 不能存item 因为切换列表显示文件夹 item将更新 要存文件路径
//    //清空上一次剪贴板存下的复制内容
//    clipBoardPaths.clear();
//    //将选中项对应的路径存入剪贴板
//    QList<QListWidgetItem*> items = selectedItems();
//    int count = items.count();
//    for (int i = 0; i < count; i++)
//    {
//        clipBoardPaths.append(mFolderPath + "/" + mMap.value(items.at(i))->mFileName);
//    }
//    //发送信号给文件夹树 重置文件夹树的剪贴板
//    copyToProjectTree(clipBoardPaths);
//}

//void FileList::copyByProjectTree(QList<QString> folderPaths)
//{
//    //清空上一次剪贴板存下的复制内容
//    clipBoardPaths.clear();
//    int count = folderPaths.count();
//    for (int i = 0; i < count; i++)
//    {
//        clipBoardPaths.append(folderPaths.at(i));
//    }
//}

//void FileList::pasteAction()
//{
//    //复制到当前显示的文件夹下 mFolderPath
//    int count = clipBoardPaths.count();
//    if (count == 0)
//    {
//        //剪贴板没有内容
//        QApplication::beep();
//        return;
//    }
//    for (int i = 0; i < count; i++)
//    {
//        //不能复制到自己和自己的子孙下面
//        //folder和folder(2)会被认为是自己 尾部接个/号 folder/和folder(2)/不会认为是自己
//        QString sourcePath = clipBoardPaths.at(i) + "/";
//        QString destPath = mFolderPath + "/";
//        if (destPath.mid(0, sourcePath.length()) == sourcePath)
//        {
//            QApplication::beep();
//            return;
//        }
//    }
//    //粘贴合法
//    //取消选中项的高亮
//    QList<QListWidgetItem*> selected = selectedItems();
//    count = selected.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(selected.at(i), false);
//    }
//    //成功粘贴的 是文件夹的项 计数 大于0 向文件夹树发送信号 修改树
//    int pastedFolderCount = 0;
//    //存是文件夹的粘贴项的名字 有些副本名字相同被重命名了
//    QList<QString> clipBoardTranscriptFolderNames;
//    //存入所有成功粘贴的文件名字 用于在更新后的列表中找到对应项 显示高亮
//    QList<QString> clipBoardTranscriptFileNames;
//    //成功粘贴的文件数 如果没有成功粘贴的文件 发出警告
//    int transcriptCount = 0;
//    count = clipBoardPaths.count();
//    for (int i = 0; i < count; i++)
//    {
//        //文件是否还存在 可能复制之后粘贴之前被删除了
//        QFileInfo *fileInfo = new QFileInfo(clipBoardPaths.at(i));
//        if (fileInfo->exists())
//        {
//            //成功创建了一个副本
//            transcriptCount++;
//            QString transcriptPath = mFolderPath + "/" + fileInfo->fileName();
//            if (fileInfo->isDir())
//            {
//                pastedFolderCount++;
//                transcriptPath = checkFolderDuplicateName(transcriptPath);
//                //复制文件
//                ProjectTree::copyDirectoryFiles(fileInfo->absoluteFilePath(), transcriptPath);
//                //复制对应meta
//                ProjectTree::copyDirectoryFiles(getMetaFilePath(fileInfo->absoluteFilePath()),
//                                                getMetaFilePath(transcriptPath));
//                QString name = transcriptPath.mid(transcriptPath.lastIndexOf('/') + 1);
//                clipBoardTranscriptFolderNames.append(name);
//                clipBoardTranscriptFileNames.append(name);
//            }
//            else
//            {
//                transcriptPath = checkFileDuplicateName(transcriptPath);
//                QFile::copy(fileInfo->absoluteFilePath(), transcriptPath);
//                QString suffix = fileInfo->fileName().mid(fileInfo->fileName().lastIndexOf('.') + 1);
//                //有些文件有meta
//                if (meshSuffixs.contains(suffix))
//                {
//                    QString sourceMetaFilePath = getMetaJpgFilePath(fileInfo->absoluteFilePath());
//                    QString transcriptMetaFilePath = getMetaJpgFilePath(transcriptPath);
//                    QFile::copy(sourceMetaFilePath, transcriptMetaFilePath);
//                }
//                //材质文件需要新建材质对象
//                else if (suffix == "mtl")
//                {
//                    new Material(transcriptPath);
//                }
//                clipBoardTranscriptFileNames.append(transcriptPath.mid(transcriptPath.lastIndexOf('/') + 1));
//            }
//        }
//    }
//    if (transcriptCount == 0)
//    {
//        //剪贴板上的文件都被删除了
//        QApplication::beep();
//        return;
//    }
//    //更新列表
//    showFolderContent(mFolderPath);
//    //粘贴项被选中 高亮
//    selectPasteItem(clipBoardTranscriptFileNames);
//    //有新增的文件夹 更新文件夹树
//    if (pastedFolderCount > 0)
//        pasteItemInProjectTree(clipBoardTranscriptFolderNames);
//    //清空属性栏
//    clearPropertyWidget();
//}

//void FileList::pasteFileFromProjectTree(QList<QString> filePaths, QString destPath, QList<QString> pastedFolderNames)
//{
//    //能执行到此处 粘贴是合法的 虽然非文件夹的文件无需考虑粘贴到子文件夹里 但是文件和文件夹同时被选中复制时 文件夹不能粘贴 文件一起不粘贴
//    //存入文件名 用于高亮显示
//    //将文件夹副本名写入
//    QList<QString> fileNames = pastedFolderNames;
//    int count = filePaths.count();
//    for (int i = 0; i < count; i++)
//    {
//        QFileInfo *fileInfo = new QFileInfo(filePaths.at(i));
//        if (!fileInfo->exists())
//        {
//            //粘贴之前被删除了
//            continue;
//        }
//        //文件夹粘贴操作在文件夹树中进行了 只处理非文件夹文件
//        QString fileName = fileInfo->fileName();
//        QString transcriptPath = destPath + "/" + fileName;
//        //文件复制
//        transcriptPath = checkFileDuplicateName(transcriptPath);
//        QFile::copy(fileInfo->absoluteFilePath(), transcriptPath);
//        //有些文件有meta
//        QString suffix = fileName.mid(fileName.lastIndexOf('.') + 1);
//        if (meshSuffixs.contains(suffix))
//        {
//            QString sourceMetaFilePath = getMetaFilePath(fileInfo->absoluteFilePath());
//            QString transcriptMetaFilePath = getMetaFilePath(transcriptPath);
//            //修改后缀为jpg
//            sourceMetaFilePath = sourceMetaFilePath.mid(0, sourceMetaFilePath.lastIndexOf('.')) + ".jpg";
//            transcriptMetaFilePath = transcriptMetaFilePath.mid(0, transcriptMetaFilePath.lastIndexOf('.')) + ".jpg";
//            QFile::copy(sourceMetaFilePath, transcriptMetaFilePath);
//        }
//        //存入副本文件名
//        fileNames.append(transcriptPath.mid(transcriptPath.lastIndexOf('/') + 1));
//    }
//    //如果是复制到当前显示的文件夹下
//    if (destPath == mFolderPath)
//    {
//        //更新列表
//        showFolderContent(mFolderPath);
//        //去掉原来选中项的高亮 列表被更新了 没有选中项
//        //粘贴项高亮
//        selectPasteItem(fileNames);
//    }
//}

//void FileList::selectPasteItem(QList<QString> itemNames)
//{
//    //遍历列表每项的名字 如果在粘贴项名字里
//    QMap<QListWidgetItem*, FileInfo*>::Iterator itr;
//    for (itr = mMap.begin(); itr != mMap.end(); itr++)
//    {
//        FileInfo *fileInfo = itr.value();
//        if (itemNames.contains(fileInfo->mFileName))
//        {
//            setItemSelected(itr.key(), true);
//        }
//    }
//}


//void FileList::moveItems()
//{
//    QString newOnlyPath = mFolderPath + "/" + mMap.value(indicatorItem)->mFileName;
//    //选中项移入indicatorItem中
//    QList<QListWidgetItem*> items = selectedItems();
//    for (int i = 0; i < items.count(); i++)
//    {
//        QListWidgetItem *item = items.at(i);
//        FileInfo *fileInfo = mMap.value(item);
//        QString oldPath = mFolderPath + "/" + fileInfo->mFileName;
//        QString newPath = newOnlyPath + "/" + fileInfo->mFileName;
//        if (fileInfo->mFileType == FileType::dir)
//        {
//            //copy文件夹到indicatorItem里
//            newPath = checkFolderDuplicateName(newPath);
//            ProjectTree::copyDirectoryFiles(oldPath, newPath);
//            //删除原来位置的文件夹
//            QDir *dir = new QDir(oldPath);
//            dir->removeRecursively();
//            //对应的meta文件夹也要移动
//            QString oldMetaFolderPath = getMetaFilePath(oldPath);
//            ProjectTree::copyDirectoryFiles(oldMetaFolderPath, getMetaFilePath(newPath));
//            dir = new QDir(oldMetaFolderPath);
//            dir->removeRecursively();
//            //文件夹树对应文件夹结点移动
//            //newPath名字可能被重命名了
//            moveItemInProjectTree(oldPath, newPath);
//        }
//        else
//        {
//            //copy文件到indicatorItem里
//            newPath = checkFileDuplicateName(newPath);
//            QFile::copy(oldPath, newPath);
//            //删除原来位置的文件
//            //删除文件
//            if (QFile::remove(oldPath))
//            {
//                //有些文件对应的meta文件夹也要移动
//                if (fileInfo->mFileType == FileType::mesh)
//                {
//                    QString oldMetaFilePath = getMetaFilePath(oldPath);
//                    QString newMetaFilePath = getMetaFilePath(newPath);
//                    //修改后缀为jpg
//                    int index = oldMetaFilePath.lastIndexOf('.');
//                    oldMetaFilePath = oldMetaFilePath.mid(0, index) + ".jpg";
//                    index = newMetaFilePath.lastIndexOf('.');
//                    newMetaFilePath = newMetaFilePath.mid(0, index) + ".jpg";
//                    QFile::copy(oldMetaFilePath, newMetaFilePath);
//                    QFile::remove(oldMetaFilePath);
//                }
//                //材质文件需要新建材质对象
//                else if (fileInfo->mFileType == FileType::material)
//                {
//                    new Material(newPath);
//                }
//            }
//        }
//        //删去项对应的映射
//        mMap.remove(item);
//        delete fileInfo;
//        //删去列表移走项
//        delete item;
//    }
//}

//bool FileList::moveItemFromProjectTree(const QMimeData *mimeData)
//{
//    //拖到的目标路径 成为destPath的子文件夹
//    QString destPath = mFolderPath;
//    if (indicatorItem)
//    {
//        destPath += "/" + mMap.value(indicatorItem)->mFileName;
//    }
//    //拖来的数据
//    QList<QString> sourceFilePaths;
//    QByteArray data = mimeData->data(getProjectTreeMimeType());
//    QDataStream dataStream(&data, QIODevice::ReadOnly);
//    QString levelPath;
//    dataStream >> levelPath;
//    while (!levelPath.isEmpty())
//    {
//        QString sourceFilePath = projectPath + "contents/" + levelPath.mid(0, levelPath.length() - 1);
//        sourceFilePaths.append(sourceFilePath);
//        //文件夹不能移到自己里面 也不能移到自己的子文件夹里面
//        if (destPath.mid(0, sourceFilePath.length()) == sourceFilePath)
//        {
//            return false;
//        }
//        //也不能自己成为自己的兄弟 移到自己的父文件夹里
//        if (sourceFilePath.mid(0, sourceFilePath.lastIndexOf('/')) == destPath)
//        {
//            return false;
//        }
//        dataStream >> levelPath;
//    }
//    //移动合法 将所有文件夹移到destPath里面
//    for (int i = 0; i < sourceFilePaths.count(); i++)
//    {
//        QString prePath = sourceFilePaths.at(i);
//        QString fileName = prePath.mid(prePath.lastIndexOf('/') + 1);
//        QString newPath = checkFolderDuplicateName(destPath + "/" + fileName);
//        //移动文件夹
//        ProjectTree::copyDirectoryFiles(prePath, newPath);
//        //删除原来位置的文件夹
//        QDir *dir = new QDir(prePath);
//        dir->removeRecursively();
//        //对应的meta文件夹也要移动
//        QString oldMetaFolderPath = getMetaFilePath(prePath);
//        ProjectTree::copyDirectoryFiles(oldMetaFolderPath, getMetaFilePath(newPath));
//        dir = new QDir(oldMetaFolderPath);
//        dir->removeRecursively();
//        //文件夹树的结点也要移动
//        moveItemInProjectTree(prePath, newPath);
//    }
//    //刷新列表
//    showFolderContent(mFolderPath);
//    return true;
//}

//void FileList::changeItemSize(int factor)
//{
//    /*//修改图标大小item大小
//    setIconSize(standardIconSize + factor * 5 * QSize(1, 1));
//    itemSize = standardItemSize + factor * 5 * QSize(1, 1);
//    //刷新列表
//    showFolderContent(mFolderPath);*/
//}

//void FileList::cancelSelectedItems()
//{
//    //取消选中
//    setCurrentItem(NULL);
//}

//void FileList::itemClickedSlot(QListWidgetItem* item)
//{
//    Q_UNUSED(item);

//    //属性栏需要显示文件的属性 层级视图及场景选中的对象去掉
//    cancelHierarchyTreeSelectedItems();

//    QList<QListWidgetItem*> items = selectedItems();
//    int count = items.count();
//    if (count == 1)
//    {
//        //属性栏显示该文件对应属性
//        FileInfo *fileInfo = mMap.value(items.first());
//        if (fileInfo->mFileType == FileType::material)
//        {
//            showMaterialProperty(mFolderPath + "/" + fileInfo->mFileName);
//        }
//        else if (fileInfo->mFileType == FileType::mesh)
//        {
//            QString suffix = fileInfo->mFileName.mid(fileInfo->mFileName.lastIndexOf('.') + 1);
//            if (suffix == "fbx")
//            {
//                showFbxProperty(mFolderPath + "/" + fileInfo->mFileName);
//            }
//            else
//            {
//                clearPropertyWidget();
//            }
//        }
//        else
//        {
//            clearPropertyWidget();
//        }
//    }
//    else
//    {
//        clearPropertyWidget();
//    }
//}

//void FileList::updateMaterialFileIcon(QString filePath)
//{
//    QString onlyPath = filePath.mid(0, filePath.lastIndexOf('/'));
//    if (mFolderPath == onlyPath)
//    {
//        //被修改的材质文件当前正在文件列表中被显示
//        QString fileName = filePath.mid(filePath.lastIndexOf('/') + 1);
//        //findItems(fileName, Qt::MatchExactly);不行 因为text被换行了
//        //找该文件对应的item
//        QMap<QListWidgetItem*, FileInfo*>::Iterator itr;
//        for (itr = mMap.begin(); itr != mMap.end(); itr++)
//        {
//            FileInfo *fileInfo = itr.value();
//            if (fileInfo->mFileName == fileName)
//            {
//                //找该文件对应的item 修改图标
//                QListWidgetItem *item = itr.key();
//                Material *material = Material::mtlMap.value(filePath);
//                item->setIcon(QIcon(material->getPreview()));
//                break;
//            }
//        }
//    }
//}
