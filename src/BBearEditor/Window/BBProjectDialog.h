#ifndef BBPROJECTDIALOG_H
#define BBPROJECTDIALOG_H

#include <QDialog>


class QAbstractButton;
class QListWidgetItem;


namespace Ui {
class BBProjectDialog;
}

class BBProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BBProjectDialog(QWidget *pMainWidget, QWidget *pParent = 0);
    ~BBProjectDialog();

private slots:
    void switchDiskAndCloud(QAbstractButton *pButton);
    void switchFolderAndNew(QAbstractButton *pButton);
    void selectFolder();
    void createNewProject();
    void closeDialog();
    void openSelectedProject(QListWidgetItem *pItem);

private:
    void setButtonTabBar();
    void loadExistingProject();
    void setListWidget();
    void setLineEdit();

    Ui::BBProjectDialog *m_pUi;
    QWidget *m_pMainWidget;
    static QString m_ProjectDirArrayKey;
    static QString m_ProjectDirKey;
    int m_nProjectCount;
    QList<QString> m_ProjectDirs;
};

#endif // BBPROJECTDIALOG_H
