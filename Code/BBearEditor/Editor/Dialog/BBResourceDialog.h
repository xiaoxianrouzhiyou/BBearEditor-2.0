#ifndef BBRESOURCEDIALOG_H
#define BBRESOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class BBResourceDialog;
}

class BBResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BBResourceDialog(QWidget *pParent = 0);
    ~BBResourceDialog();

private:
    Ui::BBResourceDialog *m_pUi;
};

#endif // BBRESOURCEDIALOG_H
