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

private:
    void addBlendStateItem();
    void addBlendFuncItem();
    void addCullStateItem();
    void addCullFaceItem();

private slots:
    void enableBlendState(bool bEnable);
    void switchSRCBlendFunc(int nIndex);
    void switchDSTBlendFunc(int nIndex);
    void enableCullState(bool bEnable);
    void switchCullFace(int nIndex);

private:
    void addPropertyItems();

private slots:
    void setSampler2D(const QString &uniformName, const QString &texturePath);
    void setTilingAndOffset(const QString &uniformName, float fTilingX, float fTilingY, float fOffsetX, float fOffsetY);
    void setSamplerCube(const QString &uniformName, QString *pResourcePaths);
    void setFloat(const QString &uniformName, float fValue);
    void setColor(float r, float g, float b, float a, const std::string &uniformName);

private:
    BBMaterial *m_pMaterial;
    BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
};


#endif // BBMATERIALPROPERTYGROUPMANAGER_H
