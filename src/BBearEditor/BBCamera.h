#ifndef BBCAMERA_H
#define BBCAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <GL/glu.h>

class BBGameObject;
class BBCamera
{
public:
    BBCamera();
    void resetMove();
    void update(float fDeltaTime);
//    void move(char dir, bool isMove);
//    void pitch(float angle);
//    void yaw(float angle);
//    void rotateView(float angle, float x, float y, float z);
//    void switchTo3D();
//    void switchTo2D();
//    void setViewportSize(int width, int height);
//    void lookAt(GameObject *object);
//    Ray createRayFromScreen(int x, int y);
//    GLint viewport[4];
//    int viewportWidth;
//    int viewportHeight;
//    void setMoveSpeed(int dir);


private:
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
    GLdouble m_ModelView[16];
    GLdouble m_Projection[16];
    QMatrix4x4 m_ViewMatrix;
};

#endif // BBCAMERA_H
