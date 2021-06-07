#ifndef BBUITITLEBAR_H
#define BBUITITLEBAR_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class BBTitleBar;
}

class BBTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit BBTitleBar(QWidget *parent = 0);
    virtual ~BBTitleBar();

signals:
    void run(bool trigger);

private slots:
    void closeWindow();
    void minimizeWindow();
    void maximizeWindow();
    void buttonRunClicked(bool trigger);
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::BBTitleBar *m_pUi;
    QWidget *m_pParent;
};

#endif // BBUITITLEBAR_H
