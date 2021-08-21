#include "BBConsoleDockWidget.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include "FileSystem/BBFileSystemDataManager.h"
#include "Python/BBPythonVM.h"


BBConsoleDockWidget::BBConsoleDockWidget(QWidget *pParent)
    : QDockWidget(pParent)
{
    setAcceptDrops(true);
}

BBConsoleDockWidget::~BBConsoleDockWidget()
{

}

void BBConsoleDockWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QByteArray data;
    if ((data = event->mimeData()->data(BB_MIMETYPE_FILELISTWIDGET)) != nullptr)
    {
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        m_CurrentDragFilePath.clear();
        dataStream >> m_CurrentDragFilePath;
        QString suffix = QFileInfo(m_CurrentDragFilePath).suffix();
        if (BBFileSystemDataManager::m_ScriptSuffixs.contains(suffix))
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
}

void BBConsoleDockWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void BBConsoleDockWidget::dragLeaveEvent(QDragLeaveEvent *event)
{

}

void BBConsoleDockWidget::dropEvent(QDropEvent *event)
{
    BBPythonVM::runScript(m_CurrentDragFilePath);
}
