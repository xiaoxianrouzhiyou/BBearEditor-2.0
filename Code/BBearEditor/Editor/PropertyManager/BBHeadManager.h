#ifndef BBHEADMANAGER_H
#define BBHEADMANAGER_H


#include <QWidget>

class QLineEdit;
class BBGameObject;
class QPushButton;
class BBEnumFactory;
class QLabel;
class BBMaterial;

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

    inline BBMaterial* getMaterial() { return m_pMaterial; }

private slots:
    void changeCurrentVShader(const QString &name);
    void changeCurrentFShader(const QString &name);

private:
    void setIcon();
    void setShaderEnumFactory(QWidget *pParent);

    BBMaterial* m_pMaterial;
    QString m_FilePath;
    QLabel *m_pIcon;
    BBEnumFactory *m_pVShaderEnumFactory;
    BBEnumFactory *m_pFShaderEnumFactory;
};

#endif // BBHEADMANAGER_H
