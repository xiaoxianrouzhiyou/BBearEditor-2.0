#ifndef BBOBJECTLISTWIDGET_H
#define BBOBJECTLISTWIDGET_H

#include <QListWidget>

class BBObjectListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit BBObjectListWidget(QWidget *parent = 0, const int pieceSize = 35);
    ~BBObjectListWidget();
    bool loadListItems(const char *xmlFilePath);
    void setObjectType(QString strObjectType) { m_strObjectType = strObjectType; }

private:
    int m_iPieceSize;
    // {Base, Light, ...}
    QString m_strObjectType;
};

#endif // BBOBJECTLISTWIDGET_H
