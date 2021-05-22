#ifndef BBGROUPMANAGER_H
#define BBGROUPMANAGER_H


#include <QWidget>


class QToolButton;
class QPushButton;
class QMenu;
class BBGameObject;
class BBVector3DFactory;

// Manage a group of property

class BBGroupManager : public QWidget
{
    Q_OBJECT
public:
    explicit BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent = nullptr);
    ~BBGroupManager();

    void addFactory(const QString &name, QWidget *pFactory, int nStretch = 1);
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

private slots:
    void changePosition(const QVector3D &value);
    void changeRotation(const QVector3D &value);
    void changeScale(const QVector3D &value);
    void popMenu();
    void showGlobalCoordinate();
    void showLocalCoordinate();

signals:
    void updateCoordinateSystem();

private:
    void switchPositionValue();
    void switchRotationValue();
    void switchScaleValue();

    BBVector3DFactory *m_pPositionFactory;
    BBVector3DFactory *m_pRotationFactory;
    BBVector3DFactory *m_pScaleFactory;
    BBGameObject *m_pGameObject;
    BBReferenceSystem m_eReferenceSystem;
};


#endif // BBGROUPMANAGER_H
