#ifndef BBUITITLEBAR_H
#define BBUITITLEBAR_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class BBUITitleBar;
}

class BBUITitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit BBUITitleBar(QWidget *parent = 0);
    virtual ~BBUITitleBar();

signals:
    void run(bool trigger);

private slots:
    void closeWindow();
    void minimizeWindow();
    void maximizeWindow();
    void buttonRunClicked(bool trigger);
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::BBUITitleBar *m_pUi;
    QWidget *m_pParent;
};

#endif // BBUITITLEBAR_H
