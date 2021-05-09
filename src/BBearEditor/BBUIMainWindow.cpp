#include "BBUIMainWindow.h"
#include "ui_BBUIMainWindow.h"

BBUIMainWindow::BBUIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::BBUIMainWindow)
{
    m_pUi->setupUi(this);
}

BBUIMainWindow::~BBUIMainWindow()
{
    delete m_pUi;
}
