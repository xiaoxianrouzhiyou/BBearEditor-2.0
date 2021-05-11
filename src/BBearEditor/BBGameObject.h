#ifndef BBGAMEOBJECT_H
#define BBGAMEOBJECT_H


#include <QString>


class BBGameObject
{
public:
    BBGameObject();
    inline QString getName() { return m_strName; }
    void setName(const QString name) { m_strName = name; }
    inline QString getClassName()  { return m_strClassName; }
    void setClassName(const QString className) { m_strClassName = className; }
    inline QString getIconName()  { return m_strIconName; }
    void setIconName(const QString iconName) { m_strIconName = iconName; }
    inline bool getActive() { return m_bActive; }

//    GameObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    GameObject(QVector3D position, QVector3D rotation, QVector3D scale);
//    void setModelMatrix(float px, float py, float pz, QQuaternion r, float sx, float sy, float sz);
//    void setModelMatrix(QMatrix4x4 modelMatrix);
//    QMatrix4x4 getModelMatrix();
//    virtual void setPosition(QVector3D position, bool isUpdateLocalTransform = true);
//    QVector3D getPosition();
//    QVector3D getLocalPosition();
//    virtual void setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform = true);
//    virtual void setRotation(QVector3D rotation, bool isUpdateLocalTransform = true);
//    QVector3D getRotation();
//    QVector3D getLocalRotation();
//    QQuaternion getQuaternion();
//    QQuaternion getLocalQuaternion();
//    virtual void setScale(QVector3D scale, bool isUpdateLocalTransform = true);
//    QVector3D getScale();
//    QVector3D getLocalScale();
//    void setLocalTransform();
//    virtual void setActive(bool isActive);
//    virtual void setVisible(bool isVisible);
//    void setBaseAttributes(QString name, QString className, QString iconName, bool isActive = true);
//    virtual void lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor = 2.4);
//    virtual void init();
//    virtual void render(Camera camera);
//    virtual void render(QMatrix4x4 modelMatrix, Camera camera);
//    virtual void resize(float width, float height);
//    virtual bool hit(Ray ray, float &distance);
//    virtual bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                         QVector3D top1, QVector3D top2, QVector3D top3,
//                                         QVector3D right1, QVector3D right2, QVector3D right3,
//                                         QVector3D bottom1, QVector3D bottom2, QVector3D bottom3);
//    QString getFilePath();


protected:
    QString m_strName;
    QString m_strClassName;
    QString m_strIconName;
    bool m_bActive;
//    QString mFilePath;
//    QVector3D mPosition;
//    QVector3D mLocalPosition;
//    QVector3D mRotation;
//    QVector3D mLocalRotation;
//    QQuaternion mQuaternion;
//    QQuaternion mLocalQuaternion;
//    QVector3D mScale;
//    QVector3D mLocalScale;
//    QMatrix4x4 mModelMatrix;
};

#endif // BBGAMEOBJECT_H
