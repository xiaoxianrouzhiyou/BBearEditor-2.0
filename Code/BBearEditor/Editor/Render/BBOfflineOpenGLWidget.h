#ifndef BBOFFLINEOPENGLWIDGET_H
#define BBOFFLINEOPENGLWIDGET_H


#include "BBOpenGLWidget.h"


class BBOfflineOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    BBOfflineOpenGLWidget(QWidget *pParent = 0);
    ~BBOfflineOpenGLWidget();
};

#endif // BBOFFLINEOPENGLWIDGET_H
