#include "BBUIMainWindow.h"
#include "ui_BBUIMainWindow.h"
#include "BBUtils.h"

BBUIMainWindow::BBUIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::BBUIMainWindow)
{
    m_pUi->setupUi(this);
    setGameObjectDockWidget();
}

BBUIMainWindow::~BBUIMainWindow()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBUIMainWindow::setGameObjectDockWidget()
{
    // Load contents for list
    m_pUi->listBaseGameObject->loadListItems("../BBearEditor/xmlfiles/baselist.xml");
}
