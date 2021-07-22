#include "BBResourceDialog.h"
#include "ui_BBResourceDialog.h"

BBResourceDialog::BBResourceDialog(QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::BBResourceDialog)
{
    m_pUi->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    if (pParent)
    {
        move(pParent->mapToGlobal(QPoint(0, 0)));
    }
}

BBResourceDialog::~BBResourceDialog()
{
    delete m_pUi;
}
