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
    explicit BBConfirmationDialog(QWidget *parent = 0);
    virtual ~BBConfirmationDialog();

    void setTitle(QString title);
    void setMessage(QString message);

private:
    Ui::BBConfirmationDialog *m_pUi;
};

#endif // BBCONFIRMATIONDIALOG_H
