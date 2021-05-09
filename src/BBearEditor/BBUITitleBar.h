#ifndef BBUITITLEBAR_H
#define BBUITITLEBAR_H

#include <QWidget>

namespace Ui {
class BBUITitleBar;
}

class BBUITitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit BBUITitleBar(QWidget *parent = 0);
    ~BBUITitleBar();

private:
    Ui::BBUITitleBar *m_pUi;
};

#endif // BBUITITLEBAR_H
