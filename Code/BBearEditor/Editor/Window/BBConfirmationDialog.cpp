#include "BBConfirmationDialog.h"
#include "ui_BBConfirmationDialog.h"
#include "Utils/BBUtils.h"
#include <QApplication>
#include <QDesktopWidget>
#include "Render/BBEditViewDockWidget.h"


BBConfirmationDialog::BBConfirmationDialog(QWidget *pParent)
    : QDialog(pParent),
      m_pUi(new Ui::BBConfirmationDialog)
{
    m_pUi->setupUi(this);

    m_bCanceled = true;

    // Frameles
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // Show it in the center
    move((QApplication::desktop()->width() - width()) / 2,
        (QApplication::desktop()->height() - height()) / 2);
    // Press Enter to invoke the click event of the yes button
    m_pUi->buttonYes->setDefault(true);

    QObject::connect(m_pUi->buttonClose, SIGNAL(clicked(bool)), this, SLOT(pressCloseButton()));
    QObject::connect(m_pUi->buttonNo, SIGNAL(clicked(bool)), this, SLOT(pressNoButton()));
}

BBConfirmationDialog::~BBConfirmationDialog()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBConfirmationDialog::setTitle(const QString &title)
{
    m_pUi->labelTitle->setText(title);
}

void BBConfirmationDialog::setMessage(const QString &message)
{
    m_pUi->labelMessage->setText(message);
}

void BBConfirmationDialog::pressCloseButton()
{
    m_bCanceled = true;
}

void BBConfirmationDialog::pressNoButton()
{
    m_bCanceled = false;
}
