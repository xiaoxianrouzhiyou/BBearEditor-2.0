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
    void setMimeType(QString strMimeType) { m_strMimeType = strMimeType; }

private:
    void startDrag(Qt::DropActions supportedActions) override;

    int m_iPieceSize;
    // {base, light, ...}
    QString m_strMimeType;
};

#endif // BBOBJECTLISTWIDGET_H
