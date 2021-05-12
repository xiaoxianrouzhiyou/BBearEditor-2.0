#ifndef BBOPENGLWIDGET_H
#define BBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QTimer;
class BBOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    BBOpenGLWidget(QWidget *parent = 0);
    ~BBOpenGLWidget();
//    Scene scene;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    QTimer *m_pRenderTimer;
};

#endif // BBOPENGLWIDGET_H
