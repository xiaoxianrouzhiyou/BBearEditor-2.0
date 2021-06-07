#ifndef BBRENDERTHREAD_H
#define BBRENDERTHREAD_H


#include <QObject>


class QTimer;

class BBRenderThread : public QObject
{
    Q_OBJECT

public:
    explicit BBRenderThread(QObject *pParent = nullptr);
    ~BBRenderThread();

signals:
    void updateOpenGLWidget();

private slots:
    void render();

private:
    QTimer *m_pRenderTimer;
};

#endif // BBRENDERTHREAD_H
