#ifndef BBCAMERA_H
#define BBCAMERA_H

#include <QVector3D>
#include <GL/glu.h>

//class BBGameObject;
class BBCamera
{
public:
    BBCamera();
    void resetMove();
    void update(float fDeltaTime);
    void setViewportSize(int width, int height);
    void switchTo3D();
    void switchTo2D();
//    void move(char dir, bool isMove);
//    void pitch(float angle);
//    void yaw(float angle);
//    void rotateView(float angle, float x, float y, float z);


//    void lookAt(GameObject *object);
//    Ray createRayFromScreen(int x, int y);

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
    GLdouble m_pModelView[16];
    GLdouble m_pProjection[16];
    int m_iViewportWidth;
    int m_iViewportHeight;
    GLint m_pViewport[4];
};

#endif // BBCAMERA_H
