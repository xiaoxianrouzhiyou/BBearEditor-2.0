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
    void pressMoveKey(char key);
    void releaseMoveKey(char key);
    void pressTransform(char key);
    void pressESC();
    void cancelFileListSelectedItems();
    void keyPress(QKeyEvent *e);

private:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *event) override;
};

#endif // BBEDITVIEWDOCKWIDGET_H
