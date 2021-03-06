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
    explicit BBResourceDialog(const QString &folderPath, QWidget *pParent = 0);
    ~BBResourceDialog();

    QString getCurrentItemFilePath();

private:
    bool loadListItems(const QString &folderPath);

    Ui::BBResourceDialog *m_pUi;
    static QSize m_ItemSize;
    QList<QString> m_ItemFilePaths;
};

#endif // BBRESOURCEDIALOG_H
