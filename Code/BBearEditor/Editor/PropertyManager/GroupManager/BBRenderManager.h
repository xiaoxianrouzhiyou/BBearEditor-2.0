#ifndef BBRENDERMANAGER_H
#define BBRENDERMANAGER_H


#include "BBGroupManager.h"

class BBRenderableObject;
class BBColorFactory;
class BBDragAcceptedFactory;

class BBRenderManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBRenderManager(BBRenderableObject *pObject, QWidget *pParent = 0);
    ~BBRenderManager();

private slots:
    void changeAttributeColor(float r, float g, float b, float a);
    void changeMaterial(const QString &filePath);
    void changeMaterial2(const QString &filePath);
    void popupResourceDialog();
    void triggerNormalIndicator(bool bEnable);
    void triggerLinePolygonMode(bool bEnable);

private:
    BBRenderableObject *m_pRenderableObject;

    BBColorFactory *m_pAttributeColorFactory;
    BBDragAcceptedFactory *m_pMaterialFactory;
    BBDragAcceptedFactory *m_pMaterialFactory2;
};

#endif // BBRENDERMANAGER_H
