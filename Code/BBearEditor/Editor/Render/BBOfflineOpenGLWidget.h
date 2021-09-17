#ifndef BBOFFLINEOPENGLWIDGET_H
#define BBOFFLINEOPENGLWIDGET_H


#include "BBOpenGLWidget.h"


class BBOfflineRenderer;

class BBOfflineOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    BBOfflineOpenGLWidget(QWidget *pParent = 0);
    ~BBOfflineOpenGLWidget();

private:
    void initializeGL() override;
    void resizeGL(int nWidth, int nHeight) override;

    BBOfflineRenderer *m_pOfflineRenderer;
};

#endif // BBOFFLINEOPENGLWIDGET_H
