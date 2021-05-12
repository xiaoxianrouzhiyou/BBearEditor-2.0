#include "BBConfirmationDialog.h"
#include "ui_BBConfirmationDialog.h"
#include "BBUtils.h"
#include <QApplication>
#include <QDesktopWidget>

BBConfirmationDialog::BBConfirmationDialog(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::BBConfirmationDialog)
{
    m_pUi->setupUi(this);

    // Frameles
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // Show it in the center
    move((QApplication::desktop()->width() - width()) / 2,
        (QApplication::desktop()->height() - height()) / 2);
    // Press Enter to invoke the click event of the yes button
    m_pUi->buttonYes->setDefault(true);
}

BBConfirmationDialog::~BBConfirmationDialog()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBConfirmationDialog::setTitle(QString title)
{
    m_pUi->labelTitle->setText(title);
}

void BBConfirmationDialog::setMessage(QString message)
{
    m_pUi->labelMessage->setText(message);
}
