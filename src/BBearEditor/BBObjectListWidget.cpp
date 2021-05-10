#include "BBObjectListWidget.h"
#include "BBUtils.h"
#include "rapidxml/rapidxml.hpp"

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
    FILE *pFile = NULL;
    char *pData = NULL;
    rapidxml::xml_node<> *pRoot = NULL;
    rapidxml::xml_node<> *pModel = NULL;
    rapidxml::xml_attribute<> *pAttrIcon = NULL;
    rapidxml::xml_attribute<> *pAttrFile = NULL;
    rapidxml::xml_attribute<> *pAttrName = NULL;

    do{
        pFile = fopen(xmlFilePath, "rb");
        BB_PROCESS_ERROR(pFile);
        // Seek the pointer to the end of the file
        BB_PROCESS_ERROR(BB_SUCCEEDED(fseek(pFile, 0, SEEK_END)));
        // Get the size of the file
        size_t length = ftell(pFile);
        BB_PROCESS_ERROR(length);
        // Seek to the beginning of the file
        BB_PROCESS_ERROR(BB_SUCCEEDED(fseek(pFile, 0, SEEK_SET)));
        // +1 Terminator
        pData = new char[length + 1];
        BB_PROCESS_ERROR(pData);
        // 1*length is the size of the file to be read
        BB_PROCESS_ERROR(fread(pData, 1, length, pFile));
        // Terminator
        pData[length] = 0;
        // Parsing xml
        rapidxml::xml_document<> doc;
        doc.parse<0>(pData);
        pRoot = doc.first_node();
        BB_PROCESS_ERROR(pRoot);
        pModel = pRoot->first_node();
        for(; pModel != 0; pModel = pModel->next_sibling())
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
    }while(0);

    if(pFile)
        fclose(pFile);
    if(pData)
        BB_SAFE_DELETE_ARRAY(pData);
    if(pRoot)
        BB_SAFE_DELETE(pRoot);
    if(pModel)
        BB_SAFE_DELETE(pModel);
    if(pAttrIcon)
        BB_SAFE_DELETE(pAttrIcon);
    if(pAttrFile)
        BB_SAFE_DELETE(pAttrFile);
    if(pAttrName)
        BB_SAFE_DELETE(pAttrName);
    return bResult;
}
