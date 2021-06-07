#ifndef BBCONFIRMATIONDIALOG_H
#define BBCONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class BBConfirmationDialog;
}

class BBConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BBConfirmationDialog(QWidget *pParent = 0);
    virtual ~BBConfirmationDialog();

    void setTitle(const QString &title);
    void setMessage(const QString &message);

private:
    Ui::BBConfirmationDialog *m_pUi;
};

#endif // BBCONFIRMATIONDIALOG_H
