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

private:
    int m_iPieceSize;
};

#endif // BBOBJECTLISTWIDGET_H
