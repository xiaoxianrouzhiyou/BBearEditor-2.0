#include "BBCamera.h"
#include "math.h"

BBCamera::BBCamera()
    : m_Position(0, 1, 10), m_ViewCenter(0, 1, 0), m_Up(0, 1, 0)
{
    m_fMoveSpeed = 10.0f;
    resetMove();

    for (int i = 0; i < 16; i++)
    {
        m_pModelView[i] = 0;
        m_pProjection[i] = 0;
    }
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

    float d = m_fMoveSpeed * fDeltaTime;
    if (m_bMoveLeft)
    {
        m_Position = m_Position - d * rightDirection;
        m_ViewCenter = m_ViewCenter - d * rightDirection;
    }
    else if (m_bMoveRight)
    {
        m_Position = m_Position + d * rightDirection;
        m_ViewCenter = m_ViewCenter + d * rightDirection;
    }
    if (m_bMoveForward)
    {
        m_Position = m_Position + d * forwardDirection;
        m_ViewCenter = m_ViewCenter + d * forwardDirection;
    }
    else if (m_bMoveBack)
    {
        m_Position = m_Position - d * forwardDirection;
        m_ViewCenter = m_ViewCenter - d * forwardDirection;
    }
    if (m_bMoveUp)
    {
        m_Position = m_Position + d * m_Up;
        m_ViewCenter = m_ViewCenter + d * m_Up;
    }
    else if (m_bMoveDown)
    {
        m_Position = m_Position - d * m_Up;
        m_ViewCenter = m_ViewCenter - d * m_Up;
    }

    // Reset, eliminate the influence of the previous frame on the current matrix
    glLoadIdentity();
    gluLookAt(m_Position.x(), m_Position.y(), m_Position.z(),
              m_ViewCenter.x(), m_ViewCenter.y(), m_ViewCenter.z(),
              m_Up.x(), m_Up.y(), m_Up.z());
    glGetDoublev(GL_MODELVIEW_MATRIX, m_pModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, m_pProjection);
}

void BBCamera::setViewportSize(int width, int height)
{
    m_iViewportWidth = width;
    m_iViewportHeight = height;
    m_pViewport[0] = 0;
    m_pViewport[1] = 0;
    m_pViewport[2] = m_iViewportWidth;
    m_pViewport[3] = m_iViewportHeight;
}

void BBCamera::switchTo3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, (GLdouble) m_iViewportWidth / m_iViewportHeight, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void BBCamera::switchTo2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // left, right, bottom, top edge of 2D window
    gluOrtho2D(-m_iViewportWidth / 2, m_iViewportWidth / 2, -m_iViewportHeight / 2, m_iViewportHeight / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void BBCamera::move(char dir, bool bMove)
{
    switch (dir) {
    case 'A':
        m_bMoveLeft = bMove;
        break;
    case 'D':
        m_bMoveRight = bMove;
        break;
    case 'W':
        m_bMoveForward = bMove;
        break;
    case 'S':
        m_bMoveBack = bMove;
        break;
    case 'Q':
        m_bMoveDown = bMove;
        break;
    case 'E':
        m_bMoveUp = bMove;
        break;
    default:
        break;
    }
}

void BBCamera::pitch(float fAngle)
{
    // up down
    QVector3D forwardDirection = m_ViewCenter - m_Position;
    forwardDirection.normalize();
    QVector3D rightDirection = QVector3D::crossProduct(forwardDirection, m_Up);
    rightDirection.normalize();
    rotateView(fAngle, rightDirection.x(), rightDirection.y(), rightDirection.z());
}

void BBCamera::yaw(float fAngle)
{
    // left-right  Rotate around the Y axis
    rotateView(fAngle, m_Up.x(), m_Up.y(), m_Up.z());
}

void BBCamera::setMoveSpeed(int dir)
{
    m_fMoveSpeed += 2 * dir;
    if (m_fMoveSpeed < 1)
        m_fMoveSpeed = 1;
    else if (m_fMoveSpeed > 100)
        m_fMoveSpeed = 100;
}

void BBCamera::rotateView(float fAngle, float x, float y, float z)
{
    // Rotate an angle around an axis, the change of view
    QVector3D viewDirection = m_ViewCenter - m_Position;
    QVector3D newDirection;
    float c = cosf(fAngle);
    float s = sinf(fAngle);
    QVector3D tempX(c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s);
    newDirection.setX(QVector3D::dotProduct(tempX, viewDirection));
    QVector3D tempY(x * y * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s);
    newDirection.setY(QVector3D::dotProduct(tempY, viewDirection));
    QVector3D tempZ(x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, c + z * z * (1 - c));
    newDirection.setZ(QVector3D::dotProduct(tempZ, viewDirection));
    m_ViewCenter = m_Position + newDirection;
}



//void Camera::lookAt(GameObject *object)
//{
//    object->lookAtSelf(pos, viewCenter);
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


