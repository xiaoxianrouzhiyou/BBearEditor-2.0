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
        pressMoveKeySignal('W');
        pressTransformSignal('W');
        break;
    case Qt::Key_A:
        pressMoveKeySignal('A');
        break;
    case Qt::Key_S:
        pressMoveKeySignal('S');
        break;
    case Qt::Key_D:
        pressMoveKeySignal('D');
        break;
    case Qt::Key_Q:
        pressMoveKeySignal('Q');
        break;
    case Qt::Key_E:
        pressMoveKeySignal('E');
        pressTransformSignal('E');
        break;
    case Qt::Key_R:
        pressTransformSignal('R');
        break;
    case Qt::Key_Escape:
        pressESCSignal();
        break;
    default:
        break;
    }
}

void BBEditViewDockWidget::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_W:
        releaseMoveKeySignal('W');
        break;
    case Qt::Key_A:
        releaseMoveKeySignal('A');
        break;
    case Qt::Key_S:
        releaseMoveKeySignal('S');
        break;
    case Qt::Key_D:
        releaseMoveKeySignal('D');
        break;
    case Qt::Key_Q:
        releaseMoveKeySignal('Q');
        break;
    case Qt::Key_E:
        releaseMoveKeySignal('E');
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
