#ifndef BBGAMEOBJECT_H
#define BBGAMEOBJECT_H


#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include "BBRay.h"


class BBCamera;
class BBGameObject
{
public:
    BBGameObject();
    BBGameObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    BBGameObject(float px, float py, float pz,
                 float rx, float ry, float rz,
                 float sx, float sy, float sz);
    virtual ~BBGameObject() {}

    inline QMatrix4x4 getModelMatrix() { return m_ModelMatrix; }

    virtual void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true);
    inline QVector3D getPosition() { return m_Position; }
    inline QVector3D getLocalPosition() { return m_LocalPosition; }

    virtual void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true);
    virtual void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true);
    QVector3D getRotation() { return m_Rotation; }
    QVector3D getLocalRotation() { return m_LocalRotation; }
    QQuaternion getQuaternion() { return m_Quaternion; }
    QQuaternion getLocalQuaternion() { return m_LocalQuaternion; }

    virtual void setScale(float scale, bool bUpdateLocalTransform = true);
    virtual void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true);
    QVector3D getScale() { return m_Scale; }
    QVector3D getLocalScale() { return m_LocalScale; }

    void setLocalTransform(BBGameObject* pParent);

    virtual void setActivity(bool bActive) { m_bActive = bActive; }
    inline bool getActivity() { return m_bActive; }
    virtual void setVisibility(bool bVisible) { m_bVisible = bVisible; }

    void setName(const QString &name) { m_strName = name; }
    inline QString getName() { return m_strName; }
    void setClassName(const QString &className) { m_strClassName = className; }
    inline QString getClassName()  { return m_strClassName; }
    void setIconName(const QString &iconName) { m_strIconName = iconName; }
    inline QString getIconName()  { return m_strIconName; }
    void setBaseAttributes(const QString &name, const QString &className, const QString &iconName, bool bActive = true);

    virtual void init();
    virtual void init(const QString &path);
    virtual void render(BBCamera *pCamera);
    virtual void render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera);
    virtual void resize(float fWidth, float fHeight);

    virtual bool hit(const BBRay &ray, float &fDistance);
    virtual bool belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                         const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                         const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                         const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3);

//    virtual void lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor = 2.4);

protected:
    virtual void setModelMatrix(float px, float py, float pz,
                                const QQuaternion &r,
                                float sx, float sy, float sz);

    QVector3D m_Position;
    QVector3D m_LocalPosition;
    QVector3D m_Rotation;
    QVector3D m_LocalRotation;
    QQuaternion m_Quaternion;
    QQuaternion m_LocalQuaternion;
    QVector3D m_Scale;
    QVector3D m_LocalScale;
    QMatrix4x4 m_ModelMatrix;

    bool m_bActive;
    bool m_bVisible;

    QString m_strName;
    QString m_strClassName;
    QString m_strIconName;

};

#endif // BBGAMEOBJECT_H
