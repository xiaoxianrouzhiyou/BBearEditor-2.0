#include "BBFBX2BBear.h"
#include <QListWidgetItem>
#include "Utils/BBUtils.h"


void BBFBX2BBear::loadFBXFile(QListWidgetItem *pItem)
{
    BB_PROCESS_ERROR_RETURN(pItem);
    qDebug() << pItem->text();
}
