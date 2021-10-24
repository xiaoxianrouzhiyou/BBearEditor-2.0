#ifndef BBCAMERA_H
#define BBCAMERA_H


#include "BBBaseRenderComponent.h"

class BBGameObject;
class BBRay;
class BBFrustumCluster;
class BBAABBBoundingBox3D;

class BBCamera
{
public:
    BBCamera(QVector3D position = QVector3D(0, 1, 10), QVector3D viewCenter = QVector3D(0, 1, 0), QVector3D up = QVector3D(0, 1, 0));
    ~BBCamera();

    void resetMove();
    void update(float fDeltaTime);
    void update(QVector3D position, QVector3D viewCenter);
    void resetViewportSize();
    void setViewportSize(int nWidth, int nHeight);
    void switchTo3D();
    void switchTo2D();
    void move(char dir, bool bMove);
    void pitch(float fAngle);
    void yaw(float fAngle);
    void setMoveSpeed(int dir);
    void lookAt(BBGameObject *pGameObject);
    BBRay createRayFromScreen(float x, float y);
    void switchCoordinate(int &x, int &y);
    QVector3D projectPointToScreenSpace(const QVector3D &point);
    QVector4D projectPointToScreenSpace(const QVector4D &point);

    bool isFrustumContainPoint(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ, const QVector3D &point);
    bool isSphereContainFrustum(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ,
                                const QVector3D &center, float fRadius);
    bool isFrustumIntersectWithAABB(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ, BBAABBBoundingBox3D *pAABB);

    inline QVector3D getPosition() { return m_Position; }
    inline QVector3D getViewCenter() { return m_ViewCenter; }
    inline void setViewMatrix(QMatrix4x4 viewMatrix) { m_ViewMatrix = viewMatrix; }
    inline QMatrix4x4 getViewMatrix() { return m_ViewMatrix; }
    inline void setProjectionMatrix(QMatrix4x4 projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
    inline QMatrix4x4 getProjectionMatrix() { return m_ProjectionMatrix; }
    inline int getViewportWidth() { return m_nViewportWidth; }
    inline int getViewportHeight() { return m_nViewportHeight; }
    inline float getNearPlane() { return m_fNearPlane; }
    inline float getFarPlane() { return m_fFarPlane; }
    inline float getDepth() { return m_fDepth; }
    inline float* getCameraParameters0() { return m_CameraParameters0; }
    inline float* getCameraParameters1() { return m_CameraParameters1; }

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
    float m_fDisplacement;

    float m_fVerticalAngle;
    float m_fAspect;
    float m_fNearPlane;
    float m_fFarPlane;
    float m_fDepth;

    BBFrustumCluster *m_pFrustumCluster;

    float m_CameraParameters0[4];
    float m_CameraParameters1[4];
};

#endif // BBCAMERA_H
