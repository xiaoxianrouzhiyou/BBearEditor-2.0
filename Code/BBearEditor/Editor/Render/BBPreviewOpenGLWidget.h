#ifndef BBPREVIEWOPENGLWIDGET_H
#define BBPREVIEWOPENGLWIDGET_H


#include "BBOpenGLWidget.h"

class BBModel;
class BBPreviewOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    BBPreviewOpenGLWidget(QWidget *pParent = 0);

private slots:
    void showMaterialPreview(const QString &filePath);
    void removeMaterialPreview();

private:
    void createSphere();
    BBModel *m_pSphere;
};

#endif // BBPREVIEWOPENGLWIDGET_H
