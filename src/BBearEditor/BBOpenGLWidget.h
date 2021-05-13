#ifndef BBOPENGLWIDGET_H
#define BBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QTimer;
class BBScene;
class BBOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit BBOpenGLWidget(QWidget *parent = 0);
    ~BBOpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    BBScene *m_pScene;
    QTimer *m_pRenderTimer;
};

#endif // BBOPENGLWIDGET_H
