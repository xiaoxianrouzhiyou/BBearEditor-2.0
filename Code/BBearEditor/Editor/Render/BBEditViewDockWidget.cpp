#include "BBEditViewDockWidget.h"
#include <QKeyEvent>

BBEditViewDockWidget::BBEditViewDockWidget(QWidget *pParent)
    : QDockWidget(pParent)
{

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
