#ifndef BBGAMEOBJECT_H
#define BBGAMEOBJECT_H


#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include "Geometry/BBFrustum.h"
#include "Geometry/BBRay.h"


class BBCamera;
class BBCanvas;
class BBMaterial;
class BBRenderQueue;

class BBGameObject
{
public:
    BBGameObject();
    BBGameObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    BBGameObject(float px, float py, float pz,
                 float rx, float ry, float rz,
                 float sx, float sy, float sz);
    BBGameObject(int x, int y, int nWidth, int nHeight);
    virtual ~BBGameObject() {}

    inline QMatrix4x4 getModelMatrix() { return m_ModelMatrix; }

    virtual void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true);
    inline QVector3D getPosition() { return m_Position; }
    inline QVector3D getLocalPosition() { return m_LocalPosition; }

    virtual void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true);
    virtual void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true);
    virtual void setRotation(const QQuaternion &quaternion, bool bUpdateLocalTransform = true);
    inline QVector3D getRotation() { return m_Rotation; }
    inline QVector3D getLocalRotation() { return m_LocalRotation; }
    inline QQuaternion getQuaternion() { return m_Quaternion; }
    inline QQuaternion getLocalQuaternion() { return m_LocalQuaternion; }

    virtual void setScale(float scale, bool bUpdateLocalTransform = true);
    virtual void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true);
    inline QVector3D getScale() { return m_Scale; }
    inline QVector3D getLocalScale() { return m_LocalScale; }

    void setLocalTransform(BBGameObject* pParent);

    virtual void setActivity(bool bActive) { m_bActive = bActive; }
    inline bool getActivity() { return m_bActive; }
    virtual void setVisibility(bool bVisible);

    inline void setName(const QString &name) { m_strName = name; }
    inline QString getName() { return m_strName; }
    inline void setClassName(const QString &className) { m_strClassName = className; }
    inline QString getClassName()  { return m_strClassName; }
    inline void setIconName(const QString &iconName) { m_strIconName = iconName; }
    inline QString getIconName()  { return m_strIconName; }
    void setBaseAttributes(const QString &name, const QString &className, const QString &iconName, bool bActive = true);
    inline QString getFilePath()  { return m_strFilePath; }

    virtual void init();
    virtual void init(const QString &path);
    virtual void render();
    virtual void render(BBCamera *pCamera);
    virtual void render(BBCanvas *pCanvas);
    virtual void render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera);
    virtual void resize(float fWidth, float fHeight);
    virtual void insertInRenderQueue(BBRenderQueue *pQueue);
    virtual void removeFromRenderQueue(BBRenderQueue *pQueue);

    virtual void setCurrentMaterial(int nIndex);
    virtual void setCurrentMaterial(BBMaterial *pMaterial);
    virtual void restoreMaterial();

    virtual void setTexture(const std::string &uniformName, GLuint textureName);

    virtual void openLight();
    virtual void closeLight();

    virtual bool hit(const BBRay &ray, float &fDistance);
    virtual bool hit(int x, int y);
    virtual bool belongToSelectionRegion(const BBFrustum &frustum);
    virtual void showCloseUp(QVector3D &outPosition, QVector3D &outViewCenter, float fDistFactor = 2.4);

    void setScreenCoordinateWithSwitchingOriginalPoint(int x, int y);
    virtual void setScreenCoordinate(int x, int y);
    void setSize(int nWidth, int nHeight);
    virtual void translate(int nDeltaX, int nDeltaY);
    inline QPoint getScreenCoordinate() { return QPoint(m_Position.x(), m_Position.y()); }
    inline int getScreenX() { return m_Position.x(); }
    inline int getScreenY() { return m_Position.y(); }

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

    // screen space
    int m_nWidth;
    int m_nHeight;

    bool m_bActive;
    bool m_bVisible;

    QString m_strName;
    QString m_strClassName;
    QString m_strIconName;
    QString m_strFilePath;
};

#endif // BBGAMEOBJECT_H
