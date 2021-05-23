#include "BBTitleBar.h"
#include "ui_BBTitleBar.h"
#include "BBUtils.h"

BBUITitleBar::BBUITitleBar(QWidget *parent) :
    QWidget(parent),
    m_pUi(new Ui::BBUITitleBar)
{
    m_pUi->setupUi(this);
    this->m_pParent = parent;
    m_pUi->buttonMaximize->setCheckable(true);
    m_pUi->buttonMaximize->setChecked(false);
    m_pUi->buttonRun->setCheckable(true);
    m_pUi->buttonRun->setChecked(false);
    QObject::connect(m_pUi->buttonRun, SIGNAL(clicked(bool)), this, SLOT(buttonRunClicked(bool)));
}

BBUITitleBar::~BBUITitleBar()
{
    BB_SAFE_DELETE(m_pUi);
    BB_SAFE_DELETE(m_pParent);
}

void BBUITitleBar::closeWindow()
{
    m_pParent->close();
}

void BBUITitleBar::minimizeWindow()
{
    m_pParent->showMinimized();
}

void BBUITitleBar::maximizeWindow()
{
    if (m_pUi->buttonMaximize->isChecked())
        m_pParent->showFullScreen();
    else
        m_pParent->showMaximized();
}

void BBUITitleBar::buttonRunClicked(bool trigger)
{
    run(trigger);
}

void BBUITitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    m_pParent->move(0, 0);
}
