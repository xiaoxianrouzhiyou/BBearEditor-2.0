#ifndef BBCONFIRMATIONDIALOG_H
#define BBCONFIRMATIONDIALOG_H

#include <QDialog>


class BBEditViewDockWidget;

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

    inline bool isCanceled() { return m_bCanceled; }

private slots:
    void pressCloseButton();
    void pressNoButton();

private:
    Ui::BBConfirmationDialog *m_pUi;
    /* true means doing nothing, false means removing the performation before */
    bool m_bCanceled;
};

#endif // BBCONFIRMATIONDIALOG_H
