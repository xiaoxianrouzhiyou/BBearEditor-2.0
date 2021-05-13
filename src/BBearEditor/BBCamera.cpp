#include "BBCamera.h"
#include "math.h"

BBCamera::BBCamera()
    : m_Position(0, 1, 10), m_ViewCenter(0, 1, 0), m_Up(0, 1, 0)
{
    m_fMoveSpeed = 10.0f;
    resetMove();
}

void BBCamera::resetMove()
{
    m_bMoveLeft = false;
    m_bMoveRight = false;
    m_bMoveForward = false;
    m_bMoveBack = false;
    m_bMoveUp = false;
    m_bMoveDown = false;
}

void BBCamera::update(float fDeltaTime)
{
    QVector3D forwardDirection = m_ViewCenter - m_Position;
    forwardDirection.normalize();
    QVector3D rightDirection = QVector3D::crossProduct(forwardDirection, m_Up);
    rightDirection.normalize();

//    if (m_bMoveLeft)
//    {
//        m_Position = m_Position - m_fMoveSpeed * fDeltaTime * rightDirection;
//        m_ViewCenter = m_ViewCenter - m_fMoveSpeed * fDeltaTime * rightDirection;
//    }
//    else if (m_bMoveRight)
//    {
//        m_Position = m_Position + m_fMoveSpeed * fDeltaTime * rightDirection;
//        m_ViewCenter = m_ViewCenter + m_fMoveSpeed * fDeltaTime * rightDirection;
//    }
//    if (m_bMoveForward)
//    {
//        m_Position = m_Position + m_fMoveSpeed * fDeltaTime * forwardDirection;
//        m_ViewCenter = m_ViewCenter + m_fMoveSpeed * fDeltaTime * forwardDirection;
//    }
//    else if (m_bMoveBack)
//    {
//        m_Position = m_Position - m_fMoveSpeed * fDeltaTime * forwardDirection;
//        m_ViewCenter = m_ViewCenter - m_fMoveSpeed * fDeltaTime * forwardDirection;
//    }
//    if (m_bMoveUp)
//    {
//        m_Position = m_Position + m_fMoveSpeed * fDeltaTime * m_Up;
//        m_ViewCenter = m_ViewCenter + m_fMoveSpeed * fDeltaTime * m_Up;
//    }
//    else if (m_bMoveDown)
//    {
//        m_Position = m_Position - m_fMoveSpeed * fDeltaTime * m_Up;
//        m_ViewCenter = m_ViewCenter - m_fMoveSpeed * fDeltaTime * m_Up;
//    }

    // Reset, eliminate the influence of the previous frame on the current matrix
//    glLoadIdentity();
//    gluLookAt(m_Position.x(), m_Position.y(), m_Position.z(),
//              m_ViewCenter.x(), m_ViewCenter.y(), m_ViewCenter.z(),
//              m_Up.x(), m_Up.y(), m_Up.z());
//    glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelView);
//    glGetDoublev(GL_PROJECTION_MATRIX, m_Projection);

//    m_ViewMatrix = QMatrix4x4(m_ModelView[0], m_ModelView[4], m_ModelView[8], m_ModelView[12],
//                              m_ModelView[1], m_ModelView[5], m_ModelView[9], m_ModelView[13],
//                              m_ModelView[2], m_ModelView[6], m_ModelView[10], m_ModelView[14],
//                              m_ModelView[3], m_ModelView[7], m_ModelView[11], m_ModelView[15]);
}

//void Camera::lookAt(GameObject *object)
//{
//    object->lookAtSelf(pos, viewCenter);
//}



//void Camera::move(char dir, bool isMove)
//{
//    switch (dir) {
//    case 'A':
//        isMoveLeft = isMove;
//        break;
//    case 'D':
//        isMoveRight = isMove;
//        break;
//    case 'W':
//        isMoveForward = isMove;
//        break;
//    case 'S':
//        isMoveBack = isMove;
//        break;
//    case 'Q':
//        isMoveDown = isMove;
//        break;
//    case 'E':
//        isMoveUp = isMove;
//        break;
//    default:
//        break;
//    }
//}

////上下
//void Camera::pitch(float angle)
//{
//    QVector3D forwardDirection = viewCenter - pos;
//    forwardDirection.normalize();
//    QVector3D rightDirection = QVector3D::crossProduct(forwardDirection, up);
//    rightDirection.normalize();
//    rotateView(angle, rightDirection.x(), rightDirection.y(), rightDirection.z());
//}

////左右  绕世界坐标y轴
//void Camera::yaw(float angle)
//{
//    rotateView(angle, up.x(), up.y(), up.z());
//}

////绕哪个轴旋转多少角度 视点的变化
//void Camera::rotateView(float angle, float x, float y, float z)
//{
//    QVector3D viewDirection = viewCenter - pos;
//    QVector3D newDirection;
//    float c = cosf(angle);
//    float s = sinf(angle);
//    QVector3D tempX(c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s);
//    newDirection.setX(QVector3D::dotProduct(tempX, viewDirection));
//    QVector3D tempY(x * y * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s);
//    newDirection.setY(QVector3D::dotProduct(tempY, viewDirection));
//    QVector3D tempZ(x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, c + z * z * (1 - c));
//    newDirection.setZ(QVector3D::dotProduct(tempZ, viewDirection));
//    viewCenter = pos + newDirection;
//}

//void Camera::switchTo3D()
//{
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(50.0f, (GLdouble)viewportWidth / viewportHeight, 0.1f, 1000.0f);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}

//void Camera::switchTo2D()
//{
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    //2D窗口的左右下上边界
//    gluOrtho2D(-viewportWidth / 2, viewportWidth / 2, -viewportHeight / 2, viewportHeight / 2);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}

//void Camera::setViewportSize(int width, int height)
//{
//    viewportWidth = width;
//    viewportHeight = height;
//    viewport[0] = 0;
//    viewport[1] = 0;
//    viewport[2] = viewportWidth;
//    viewport[3] = viewportHeight;
//}

//Ray Camera::createRayFromScreen(int x, int y)
//{
//    int winX = x;
//    int winY = viewportHeight - y;
//    //获取该点对应的前裁剪面的点坐标
//    GLdouble nearPosX, nearPosY, nearPosZ;
//    gluUnProject(winX, winY, 0.0f, modelView, projection, viewport, &nearPosX, &nearPosY, &nearPosZ);
//    GLdouble farPosX, farPosY, farPosZ;
//    gluUnProject(winX, winY, 1.0f, modelView, projection, viewport, &farPosX, &farPosY, &farPosZ);
//    Ray ray(nearPosX, nearPosY, nearPosZ, farPosX, farPosY, farPosZ);
//    return ray;
//}

//void Camera::setMoveSpeed(int dir)
//{
//    moveSpeed += 2 * dir;
//    if (moveSpeed < 1)
//        moveSpeed = 1;
//    else if (moveSpeed > 100)
//        moveSpeed = 100;
//}
