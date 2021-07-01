#ifndef BBPREVIEWOPENGLWIDGET_H
#define BBPREVIEWOPENGLWIDGET_H


#include "BBOpenGLWidget.h"

class BBModel;
class BBMaterial;

class BBPreviewOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    BBPreviewOpenGLWidget(QWidget *pParent = 0);

public:
    void updateMaterialSphere(BBMaterial *pMaterial);

private slots:
    void showMaterialPreview(const QString &filePath);
    void removeMaterialPreview();

private:
    void initializeGL() override;
    void createSphere();

    BBModel *m_pSphere;
};

#endif // BBPREVIEWOPENGLWIDGET_H
