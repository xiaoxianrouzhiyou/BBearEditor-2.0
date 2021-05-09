#include "BBUITitleBar.h"
#include "ui_BBUITitleBar.h"

BBUITitleBar::BBUITitleBar(QWidget *parent) :
    QWidget(parent),
    m_pUi(new Ui::BBUITitleBar)
{
    m_pUi->setupUi(this);
}

BBUITitleBar::~BBUITitleBar()
{
    delete m_pUi;
}
