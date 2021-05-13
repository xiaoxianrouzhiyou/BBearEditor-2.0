#ifndef BBEDITVIEWDOCKWIDGET_H
#define BBEDITVIEWDOCKWIDGET_H

#include <QDockWidget>

class QKeyEvent;
class BBEditViewDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit BBEditViewDockWidget(QWidget *pParent = 0);

signals:
    void pressMoveKeySignal(char key);
    void releaseMoveKeySignal(char key);
    void pressTransformSignal(char key);
    void pressESCSignal();
    void cancelFileListSelectedItems();
    void keyPress(QKeyEvent *e);

private:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *event) override;
};

#endif // BBEDITVIEWDOCKWIDGET_H
