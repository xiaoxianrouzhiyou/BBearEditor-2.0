#ifndef BBOBJECTLISTWIDGET_H
#define BBOBJECTLISTWIDGET_H

#include <QListWidget>

class BBObjectListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit BBObjectListWidget(QWidget *parent = 0, const int pieceSize = 35);
    virtual ~BBObjectListWidget();

    bool loadListItems(const char *xmlFilePath);
    void setMimeType(const QString &strMimeType) { m_strMimeType = strMimeType; }

signals:
    void removeCurrentItemInFileList();

private:
    void startDrag(Qt::DropActions supportedActions) override;
    void focusInEvent(QFocusEvent *e) override;

    int m_iPieceSize;
    // {base, light, ...}
    QString m_strMimeType;
};

#endif // BBOBJECTLISTWIDGET_H
