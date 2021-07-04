#ifndef BBCAMERA_H
#define BBCAMERA_H


#include "BBBaseRenderComponent.h"

class BBGameObject;
class BBRay;

class BBCamera
{
public:
    BBCamera();
    void resetMove();
    void update(float fDeltaTime);
    void setViewportSize(int nWidth, int nHeight);
    void switchTo3D();
    void switchTo2D();
    void move(char dir, bool bMove);
    void pitch(float fAngle);
    void yaw(float fAngle);
    void setMoveSpeed(int dir);
    void lookAt(BBGameObject *pGameObject);
    BBRay createRayFromScreen(int x, int y);
    QVector3D projectPointToScreenSpace(const QVector3D &point);
    QVector4D projectPointToScreenSpace(const QVector4D &point);

    inline void setPosition(const QVector3D &position) { m_Position = position; }
    inline QVector3D getPosition() { return m_Position; }
    inline void setViewCenter(const QVector3D &viewCenter) { m_ViewCenter = viewCenter; }
    inline QVector3D getViewCenter() { return m_ViewCenter; }
    inline QMatrix4x4 getViewMatrix() { return m_ViewMatrix; }
    inline QMatrix4x4 getProjectionMatrix() { return m_ProjectionMatrix; }
    inline int getViewportWidth() { return m_nViewportWidth; }
    inline int getViewportHeight() { return m_nViewportHeight; }

private:
    void rotateView(float fAngle, float x, float y, float z);

    QVector3D m_Position;
    // Which point are you looking at
    QVector3D m_ViewCenter;
    QVector3D m_Up;
    float m_fMoveSpeed;
    bool m_bMoveLeft;
    bool m_bMoveRight;
    bool m_bMoveForward;
    bool m_bMoveBack;
    bool m_bMoveUp;
    bool m_bMoveDown;
    GLdouble m_pModelView[16];
    GLdouble m_pProjection[16];
    int m_nViewportWidth;
    int m_nViewportHeight;
    GLint m_pViewport[4];
    QMatrix4x4 m_ViewMatrix;
    QMatrix4x4 m_ProjectionMatrix;
};

#endif // BBCAMERA_H
