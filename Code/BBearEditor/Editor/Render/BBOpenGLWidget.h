#ifndef BBOPENGLWIDGET_H
#define BBOPENGLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class BBScene;
class BBRenderThread;

class BBOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit BBOpenGLWidget(QWidget *parent = 0);
    virtual ~BBOpenGLWidget();

    inline BBScene* getScene() { return m_pScene; }

protected:
    void setRenderThread();

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    BBScene *m_pScene;
    QThread *m_pRenderThread;
    QTimer *m_pRenderTimer;
};

#endif // BBOPENGLWIDGET_H
