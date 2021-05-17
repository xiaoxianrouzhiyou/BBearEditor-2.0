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
    BBGameObject(const QVector3D position, const QVector3D rotation, const QVector3D scale);
    BBGameObject(const float px, const float py, const float pz,
                 const float rx, const float ry, const float rz,
                 const float sx, const float sy, const float sz);
    virtual ~BBGameObject() {}

    inline QMatrix4x4 getModelMatrix() { return m_ModelMatrix; }

    virtual void setPosition(const QVector3D position, const bool bUpdateLocalTransform = true);
    inline QVector3D getPosition() { return m_Position; }
    inline QVector3D getLocalPosition() { return m_LocalPosition; }

    virtual void setRotation(const int nAngle, const QVector3D axis, const bool bUpdateLocalTransform = true);
    virtual void setRotation(const QVector3D rotation, const bool bUpdateLocalTransform = true);
    QVector3D getRotation() { return m_Rotation; }
    QVector3D getLocalRotation() { return m_LocalRotation; }
    QQuaternion getQuaternion() { return m_Quaternion; }
    QQuaternion getLocalQuaternion() { return m_LocalQuaternion; }

    virtual void setScale(const QVector3D scale, const bool bUpdateLocalTransform = true);
    QVector3D getScale() { return m_Scale; }
    QVector3D getLocalScale() { return m_LocalScale; }

    virtual void setActivity(const bool bActive) { m_bActive = bActive; }
    inline bool getActivity() { return m_bActive; }
    virtual void setVisibility(const bool bVisible) { m_bVisible = bVisible; }

    void setName(const QString name) { m_strName = name; }
    inline QString getName() { return m_strName; }
    void setClassName(const QString className) { m_strClassName = className; }
    inline QString getClassName()  { return m_strClassName; }
    void setIconName(const QString iconName) { m_strIconName = iconName; }
    inline QString getIconName()  { return m_strIconName; }
    void setBaseAttributes(QString name, QString className, QString iconName, bool bActive = true);

    virtual void init();
    virtual void init(const QString path);
    virtual void render(BBCamera *pCamera);
    virtual void render(QMatrix4x4 modelMatrix, BBCamera *pCamera);
    virtual void resize(float fWidth, float fHeight);

    virtual bool hit(BBRay ray, float &fDistance);


//    void setLocalTransform();

//    virtual void lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor = 2.4);


//    virtual bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                         QVector3D top1, QVector3D top2, QVector3D top3,
//                                         QVector3D right1, QVector3D right2, QVector3D right3,
//                                         QVector3D bottom1, QVector3D bottom2, QVector3D bottom3);
//    QString getFilePath();


protected:
    virtual void setModelMatrix(const float px, const float py, const float pz,
                                const QQuaternion r,
                                const float sx, const float sy, const float sz);

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


//    QString mFilePath;

};

#endif // BBGAMEOBJECT_H
