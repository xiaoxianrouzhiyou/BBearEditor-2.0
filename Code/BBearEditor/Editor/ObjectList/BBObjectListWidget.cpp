#include "BBObjectListWidget.h"
#include "Utils/BBUtils.h"
#include "rapidxml/rapidxml.hpp"
#include <QMimeData>
#include <QDrag>

BBObjectListWidget::BBObjectListWidget(QWidget *parent, const int pieceSize)
    : QListWidget(parent), m_iPieceSize(pieceSize)
{
    setDragEnabled(true);
    setIconSize(QSize(m_iPieceSize, m_iPieceSize));
    setSpacing(2);
    setAcceptDrops(false);
    // Show the indicator being dragged
    setDropIndicatorShown(true);
}

BBObjectListWidget::~BBObjectListWidget()
{

}

bool BBObjectListWidget::loadListItems(const char *xmlFilePath)
{
    // Load external resources through .xml
    bool bResult = false;
    int nFileSize = 0;
    char *pData = NULL;
    rapidxml::xml_node<> *pRoot = NULL;
    rapidxml::xml_node<> *pModel = NULL;
    rapidxml::xml_attribute<> *pAttrIcon = NULL;
    rapidxml::xml_attribute<> *pAttrFile = NULL;
    rapidxml::xml_attribute<> *pAttrName = NULL;

    do {
        pData = BBUtils::loadFileContent(xmlFilePath, nFileSize);
        BB_PROCESS_ERROR(pData);
        // Parsing xml
        rapidxml::xml_document<> doc;
        doc.parse<0>(pData);
        pRoot = doc.first_node();
        BB_PROCESS_ERROR(pRoot);
        pModel = pRoot->first_node();
        for (; pModel != 0; pModel = pModel->next_sibling())
        {
            pAttrIcon = pModel->first_attribute("icon");
            BB_PROCESS_ERROR(pAttrIcon);
            pAttrFile = pModel->first_attribute("file");
            BB_PROCESS_ERROR(pAttrFile);
            pAttrName = pModel->first_attribute("name");
            BB_PROCESS_ERROR(pAttrName);

            QPixmap pic;
            pic.load(pAttrIcon->value());

            QListWidgetItem *pItem = new QListWidgetItem(this);
            pItem->setSizeHint(QSize(m_iPieceSize, m_iPieceSize));
            pItem->setIcon(QIcon(pic));
            pItem->setData(Qt::UserRole, QVariant(pic));
            pItem->setData(Qt::UserRole + 1, pAttrFile->value());
            pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
            pItem->setText(pAttrName->value());
        }
        bResult = true;
    } while(0);

    if (pData)
        BB_SAFE_DELETE_ARRAY(pData);
//    if (pRoot)
//        BB_SAFE_DELETE(pRoot);
//    if (pModel)
//        BB_SAFE_DELETE(pModel);
//    if (pAttrIcon)
//        BB_SAFE_DELETE(pAttrIcon);
//    if (pAttrFile)
//        BB_SAFE_DELETE(pAttrFile);
//    if (pAttrName)
//        BB_SAFE_DELETE(pAttrName);
    return bResult;
}

void BBObjectListWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    QListWidgetItem *pItem = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QString fileName = qvariant_cast<QString>(pItem->data(Qt::UserRole + 1));
    dataStream << fileName;

    QMimeData *pMimeData = new QMimeData;
    // Give this data a unique identifying tag
    pMimeData->setData(m_strMimeType, itemData);

    QPixmap pixmap = qvariant_cast<QPixmap>(pItem->data(Qt::UserRole));
    pixmap.setDevicePixelRatio(devicePixelRatio());
    pixmap = pixmap.scaled(QSize(m_iPieceSize * devicePixelRatio(), m_iPieceSize * devicePixelRatio()),
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QDrag drag(this);
    drag.setMimeData(pMimeData);
    drag.setHotSpot(QPoint(pixmap.width() / 2 / devicePixelRatio(), pixmap.height() / 2 / devicePixelRatio()));
    drag.setPixmap(pixmap);
    drag.exec(Qt::MoveAction);
}

void BBObjectListWidget::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    emit removeCurrentItemInFileList();
}
