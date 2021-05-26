#ifndef BBFILEPATHBAR_H
#define BBFILEPATHBAR_H


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


class BBFilePathBar : public QWidget
{
    Q_OBJECT

public:
    BBFilePathBar(QWidget *pParent = nullptr);

private slots:
    void showCurrentFolderPath(const QString &path);
    void accessFolder(int id);

signals:
    void showFolderContent(const QString &folderPath);

private:
    QStringList m_HierarchyDirs;
};

#endif // BBFILEPATHBAR_H



