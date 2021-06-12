#include "BBEditViewDockWidget.h"
#include <QKeyEvent>
#include "Window/BBConfirmationDialog.h"
#include "Scene/BBSceneManager.h"
#include <QFileDialog>
#include "FileSystem/BBFileSystemDataManager.h"


BBEditViewDockWidget::BBEditViewDockWidget(QWidget *pParent)
    : QDockWidget(pParent)
{

}

bool BBEditViewDockWidget::saveScene(const QString &defaultParentPath)
{
    // pop-up dialog
    BBConfirmationDialog dialog;
    dialog.setTitle("Unsaved changes!");

    QString sceneFilePath = BBSceneManager::getCurrentSceneFilePath();
    if (sceneFilePath.isEmpty())
    {
        dialog.setMessage("Do you want to save these changes?");
        if (dialog.exec())
        {
            QString defaultFilePath = BBFileSystemDataManager::getExclusiveFilePath(defaultParentPath,
                                                                                    BBConstant::BB_NAME_DEFAULT_SCENE);
            // pop-up file dialog and select path for new file
            QString filePath = QFileDialog::getSaveFileName(NULL, tr("Save Scene"), defaultFilePath, tr("Scene (*.bbscene)"));
            if (!filePath.isEmpty())
            {
                BBSceneManager::saveScene(filePath);
                return true;
            }
        }
    }
    else
    {
        dialog.setMessage("Do you want to save these changes into " + sceneFilePath + " ?");
        if (dialog.exec())
        {
            BBSceneManager::saveScene();
            return true;
        }
    }

    return false;
}

void BBEditViewDockWidget::keyPressEvent(QKeyEvent *e)
{
    keyPress(e);

    if (e->isAutoRepeat())
        return;

    switch (e->key()) {
    case Qt::Key_W:
        pressMoveKey('W');
        pressTransform('W');
        break;
    case Qt::Key_A:
        pressMoveKey('A');
        break;
    case Qt::Key_S:
        pressMoveKey('S');
        break;
    case Qt::Key_D:
        pressMoveKey('D');
        break;
    case Qt::Key_Q:
        pressMoveKey('Q');
        break;
    case Qt::Key_E:
        pressMoveKey('E');
        pressTransform('E');
        break;
    case Qt::Key_R:
        pressTransform('R');
        break;
    case Qt::Key_Escape:
        pressESC();
        break;
    default:
        break;
    }

    if ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_S))
    {

    }
}

void BBEditViewDockWidget::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_W:
        releaseMoveKey('W');
        break;
    case Qt::Key_A:
        releaseMoveKey('A');
        break;
    case Qt::Key_S:
        releaseMoveKey('S');
        break;
    case Qt::Key_D:
        releaseMoveKey('D');
        break;
    case Qt::Key_Q:
        releaseMoveKey('Q');
        break;
    case Qt::Key_E:
        releaseMoveKey('E');
        break;
    default:
        break;
    }
}

void BBEditViewDockWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    cancelFileListSelectedItems();
}
