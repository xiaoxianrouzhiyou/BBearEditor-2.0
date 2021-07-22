#include "BBResourceDialog.h"
#include "ui_BBResourceDialog.h"
#include "Utils/BBUtils.h"
#include <QDir>


QSize BBResourceDialog::m_ItemSize = QSize(32, 32);

BBResourceDialog::BBResourceDialog(const QString &folderPath, QWidget *pParent)
    :
    QDialog(pParent),
    m_pUi(new Ui::BBResourceDialog)
{
    m_pUi->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    if (pParent)
    {
        move(pParent->mapToGlobal(QPoint(0, 0)));
    }

    loadListItems(folderPath);
}

BBResourceDialog::~BBResourceDialog()
{
    delete m_pUi;
}

QString BBResourceDialog::getCurrentItemFilePath()
{
    int nIndex = m_pUi->resourceList->currentIndex().row();
    if (nIndex < 0)
    {
        return "";
    }
    else
    {
        return m_ItemFilePaths.at(nIndex);
    }
}

bool BBResourceDialog::loadListItems(const QString &folderPath)
{
    QDir dir(folderPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        pItem->setSizeHint(m_ItemSize);
        pItem->setIcon(QIcon(BB_PATH_RESOURCE_ICON(material5)));
        pItem->setText(fileInfo.baseName());
        m_pUi->resourceList->addItem(pItem);
        m_ItemFilePaths.append(fileInfo.filePath());
    }
    return true;
}
