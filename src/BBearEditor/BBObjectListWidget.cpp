#include "BBObjectListWidget.h"
#include "BBUtils.h"

BBObjectListWidget::BBObjectListWidget(QWidget *parent, int pieceSize)
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

void BBObjectListWidget::loadListItems(const char *xmlFilePath)
{
    // Load external resources through .xml
    FILE *pFile;
    do{
        pFile = fopen(xmlFilePath, "rb");
        BB_PROCESS_ERROR(pFile);
    }while(0);

    if(pFile)
    {
        fclose(pFile);
    }
}
