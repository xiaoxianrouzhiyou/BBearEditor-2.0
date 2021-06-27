#ifndef BBGROUPMANAGER_H
#define BBGROUPMANAGER_H


#include <QWidget>


class QLineEdit;
class QToolButton;
class QPushButton;
class QMenu;
class BBGameObject;
class BBVector3DFactory;
class BBScene;
class BBEnumFactory;
class QLabel;


// manage name, class name, visibility, and so on
// showed in the top of property manager
class BBBaseInformationManager : public QWidget
{
    Q_OBJECT

public:
    BBBaseInformationManager(BBGameObject *pGameObject, QWidget *pParent = 0);
    ~BBBaseInformationManager();
//    void rename(QString newName);

private slots:
    virtual void changeVisibility();
//    void finishRename();

signals:
    void visibilityChanged(BBGameObject *pGameObject, bool bVisible);
//    void nameChanged(GameObject *gameObject);

protected:
    void setVisibilityButtonChecked(bool bChecked);

    QPushButton *m_pVisibilityButton;
    QLineEdit *m_pNameEdit;
    BBGameObject *m_pCurrentGameObject;
};


// manage base information of BBGameObjectSet
class BBSetBaseInformationManager : public BBBaseInformationManager
{
    Q_OBJECT

public:
    BBSetBaseInformationManager(BBGameObject *pCenterGameObject,
                                const QList<BBGameObject*> &gameObjectSet,
                                QWidget *pParent = 0);

private slots:
    void changeVisibility() override;

signals:
    void visibilityChanged(const QList<BBGameObject*> &gameObjectSet, bool bVisible);

private:
    QList<BBGameObject*> m_CurrentGameObjectSet;
};


class BBMaterialManager : public QWidget
{
    Q_OBJECT

public:
    BBMaterialManager(const QString &filePath, QWidget *pParent = nullptr);
    ~BBMaterialManager();

private:
    void setIcon();
    void setShaderEnumFactory(QWidget *pParent);

    QString m_FilePath;
    QLabel *m_pIcon;
    BBEnumFactory *m_pVShaderEnumFactory;
    BBEnumFactory *m_pFShaderEnumFactory;
};


// Manage a group of property
class BBGroupManager : public QWidget
{
    Q_OBJECT
public:
    explicit BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent = nullptr);
    ~BBGroupManager();

    void addFactory(const QString &name, QWidget *pFactory, int nStretch = 1);
    void addFactory(QWidget *pFactory);
    //    void addProperty(QWidget *factory);
    //    LineEditFactory *addProperty(QString name, float value);
    //    void removeProperty(int index);

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


class BBGlobalSettingsGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent = 0);
    ~BBGlobalSettingsGroupManager();

private slots:
    void changeCurrentRenderingAlgorithm(int nIndex);

private:
    void initRenderingAlgorithmEnumFactory();

    static int m_nCurrentRenderingAlgorithmIndex;
    BBScene *m_pScene;
    BBEnumFactory *m_pRenderingAlgorithmEnumFactory;
};


#endif // BBGROUPMANAGER_H
