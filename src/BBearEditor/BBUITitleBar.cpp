#include "BBUITitleBar.h"
#include "ui_BBUITitleBar.h"

BBUITitleBar::BBUITitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BBUITitleBar)
{
    ui->setupUi(this);
}

BBUITitleBar::~BBUITitleBar()
{
    delete ui;
}
