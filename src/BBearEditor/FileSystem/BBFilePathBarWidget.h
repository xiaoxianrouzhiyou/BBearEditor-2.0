#ifndef BBFILEPATHBARWIDGET_H
#define BBFILEPATHBARWIDGET_H


#include <QWidget>
#include <QScrollArea>


class BBFilePathBarScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    BBFilePathBarScrollArea(QWidget *pParent = nullptr);

private slots:
    void moveToEnd();
    void moveToLeft();
    void moveToRight();

private:
    void scroll(int nStep);
};


class BBFilePathBarWidget : public QWidget
{
    Q_OBJECT

public:
    BBFilePathBarWidget(QWidget *pParent = nullptr);

private slots:
    void showFolderPath(const QString &path);
    void accessFolder(int id);

signals:
    void accessFolder(const QString &folderPath);

private:
    QString m_CurrentPath;
    QStringList m_HierarchyDirs;
};

#endif // BBFILEPATHBARWIDGET_H



