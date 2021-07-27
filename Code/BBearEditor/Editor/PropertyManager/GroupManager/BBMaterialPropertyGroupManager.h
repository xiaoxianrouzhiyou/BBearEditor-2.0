#ifndef BBMATERIALPROPERTYGROUPMANAGER_H
#define BBMATERIALPROPERTYGROUPMANAGER_H


#include "BBGroupManager.h"

class BBPreviewOpenGLWidget;
class BBMaterial;

class BBMaterialPropertyGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBMaterialPropertyGroupManager(BBMaterial *pMaterial, BBPreviewOpenGLWidget *pPreviewOpenGLWidget, QWidget *pParent = 0);
    ~BBMaterialPropertyGroupManager();

private slots:
    void setSampler2D(const QString &uniformName, const QString &texturePath);
    void setFloat(const QString &uniformName, float fValue);
    void setColor(float r, float g, float b, float a, const std::string &uniformName);

private:
    void addBlendStateItem();
    void addBlendFuncItem();
    void addPropertyItems();

private slots:
    void enableBlendState(bool bEnable);
    void switchSRCBlendFunc(int nIndex);
    void switchDSTBlendFunc(int nIndex);

private:
    unsigned int getBlendFunc(int nIndex);

private:
    BBMaterial *m_pMaterial;
    BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
};


#endif // BBMATERIALPROPERTYGROUPMANAGER_H
