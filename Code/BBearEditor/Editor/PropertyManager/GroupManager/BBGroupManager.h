#ifndef BBGROUPMANAGER_H
#define BBGROUPMANAGER_H


#include <QWidget>


class BBLineEditFactory;
class QToolButton;
class QPushButton;
class QMenu;
class BBGameObject;
class BBVector3DFactory;
class BBScene;
class BBEnumFactory;
class BBRenderableObject;
class QCheckBox;
class BBDragAcceptedFactory;


// Manage a group of property
class BBGroupManager : public QWidget
{
    Q_OBJECT
public:
    explicit BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent = nullptr);
    ~BBGroupManager();

    QWidget* addFactory(const QString &name, QWidget *pFactory, int nStretch = 1, const Qt::Alignment &alignment = Qt::Alignment());
    QWidget* addFactory(const QString &name, QWidget *pFactory1, QWidget *pFactory2, int nStretch = 1);
    QWidget* addFactory(QWidget *pFactory);
    BBLineEditFactory* addFactory(const QString &name, float fValue);

    void addMargin(int nHeight);

public slots:
    void setContainerExpanded(bool bExpanded);

protected:
    QToolButton *m_pMainButton;
    QPushButton *m_pMenuButton;
    QMenu *m_pMenu;
    QWidget *m_pContainer;
};


enum BBReferenceSystem
{
    Global = 0,
    Local = 1
};


class BBTransformGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBTransformGroupManager(BBGameObject *pGameObject, QWidget *pParent = 0);
    ~BBTransformGroupManager();

    void updatePositionValue();
    void updateRotationValue();
    void updateScaleValue();

private slots:
    void changePosition(const QVector3D &value);
    void changeRotation(const QVector3D &value);
    void changeScale(const QVector3D &value);
    void popMenu();
    void showGlobalCoordinate();
    void showLocalCoordinate();

signals:
    void coordinateSystemUpdated();

private:
    BBVector3DFactory *m_pPositionFactory;
    BBVector3DFactory *m_pRotationFactory;
    BBVector3DFactory *m_pScaleFactory;
    BBGameObject *m_pCurrentGameObject;
    BBReferenceSystem m_eReferenceSystem;
};


class BBRenderManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBRenderManager(BBRenderableObject *pObject, QWidget *pParent = 0);
    ~BBRenderManager();

private slots:
    void changeMaterial(const QString &filePath);
    void popupResourceDialog();
    void triggerNormalIndicator(bool bEnable);

private:
    BBRenderableObject *m_pRenderableObject;
    BBDragAcceptedFactory *m_pMaterialFactory;
};

#endif // BBGROUPMANAGER_H
