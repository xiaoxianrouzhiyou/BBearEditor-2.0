#include "BBCamera.h"
#include "math.h"
#include "Geometry/BBRay.h"
#include "Base/BBGameObject.h"
#include "Scene/BBSceneManager.h"
#include "BBDrawCall.h"


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

    m_fNearPlane = 0.1f;
    m_fFarPlane = 1000.0f;
    m_fDepth = 1000.0f - 0.1f;
    m_ProjectionMatrix.perspective(50.0f, 800.0f / 600.0f, m_fNearPlane, m_fFarPlane);

    m_fDisplacement = 0.0f;
    m_pFrustumCluster = nullptr;
}

BBCamera::~BBCamera()
{
    BB_SAFE_DELETE(m_pFrustumCluster);
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
        m_fDisplacement += d;
    }
    else if (m_bMoveRight)
    {
        m_Position = m_Position + d * rightDirection;
        m_ViewCenter = m_ViewCenter + d * rightDirection;
        m_fDisplacement += d;
    }
    if (m_bMoveForward)
    {
        m_Position = m_Position + d * forwardDirection;
        m_ViewCenter = m_ViewCenter + d * forwardDirection;
        m_fDisplacement += d;
    }
    else if (m_bMoveBack)
    {
        m_Position = m_Position - d * forwardDirection;
        m_ViewCenter = m_ViewCenter - d * forwardDirection;
        m_fDisplacement += d;
    }
    if (m_bMoveUp)
    {
        m_Position = m_Position + d * m_Up;
        m_ViewCenter = m_ViewCenter + d * m_Up;
        m_fDisplacement += d;
    }
    else if (m_bMoveDown)
    {
        m_Position = m_Position - d * m_Up;
        m_ViewCenter = m_ViewCenter - d * m_Up;
        m_fDisplacement += d;
    }

    // When the camera position changes, update render queue
    if (m_fDisplacement >= 10.0f)
    {
        BBSceneManager::getRenderQueue()->updateOrder();
        m_fDisplacement = 0.0f;
    }

    // Reset, eliminate the influence of the previous frame on the current matrix
    glLoadIdentity();
    gluLookAt(m_Position.x(), m_Position.y(), m_Position.z(),
              m_ViewCenter.x(), m_ViewCenter.y(), m_ViewCenter.z(),
              m_Up.x(), m_Up.y(), m_Up.z());
    glGetDoublev(GL_MODELVIEW_MATRIX, m_pModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, m_pProjection);
    m_ViewMatrix = QMatrix4x4(m_pModelView[0], m_pModelView[4], m_pModelView[8], m_pModelView[12],
                              m_pModelView[1], m_pModelView[5], m_pModelView[9], m_pModelView[13],
                              m_pModelView[2], m_pModelView[6], m_pModelView[10], m_pModelView[14],
                              m_pModelView[3], m_pModelView[7], m_pModelView[11], m_pModelView[15]);
}

void BBCamera::setViewportSize(int nWidth, int nHeight)
{
    m_nViewportWidth = nWidth;
    m_nViewportHeight = nHeight;
    m_pViewport[0] = 0;
    m_pViewport[1] = 0;
    m_pViewport[2] = m_nViewportWidth;
    m_pViewport[3] = m_nViewportHeight;

    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.perspective(50.0f, (float) nWidth / nHeight, m_fNearPlane, m_fFarPlane);

    if (m_pFrustumCluster)
    {
        BB_SAFE_DELETE(m_pFrustumCluster);
    }
    m_pFrustumCluster = new BBFrustumCluster(this, 0, 0, m_nViewportWidth, m_nViewportHeight, 2, 2, 1);
}

void BBCamera::switchTo3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, (GLdouble) m_nViewportWidth / m_nViewportHeight, m_fNearPlane, m_fFarPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void BBCamera::switchTo2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // left, right, bottom, top edge of 2D window
    gluOrtho2D(-m_nViewportWidth / 2, m_nViewportWidth / 2, -m_nViewportHeight / 2, m_nViewportHeight / 2);
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

void BBCamera::lookAt(BBGameObject *pGameObject)
{
    pGameObject->showCloseUp(m_Position, m_ViewCenter);
}

BBRay BBCamera::createRayFromScreen(int x, int y)
{
    int winX = x;
    int winY = m_nViewportHeight - y;
    // Get the 3D point coordinates corresponding to the 2D point in the front clipping plane
    GLdouble nearPosX, nearPosY, nearPosZ;
    gluUnProject(winX, winY, 0.0f, m_pModelView, m_pProjection, m_pViewport, &nearPosX, &nearPosY, &nearPosZ);
    GLdouble farPosX, farPosY, farPosZ;
    gluUnProject(winX, winY, 1.0f, m_pModelView, m_pProjection, m_pViewport, &farPosX, &farPosY, &farPosZ);
    BBRay ray(nearPosX, nearPosY, nearPosZ, farPosX, farPosY, farPosZ);
    return ray;
}

QVector3D BBCamera::projectPointToScreenSpace(const QVector3D &point)
{
    QVector3D posOnNdcSpace = m_ProjectionMatrix * point;
    return QVector3D(posOnNdcSpace.x() * m_nViewportWidth / 2.0f,
                     posOnNdcSpace.y() * m_nViewportHeight / 2.0f,
                     0.0f);
}

QVector4D BBCamera::projectPointToScreenSpace(const QVector4D &point)
{
    QVector4D posOnClipSpace = m_ProjectionMatrix * point;
    QVector4D posOnNdcSpace = QVector4D(posOnClipSpace.x() / posOnClipSpace.w(),
                                        posOnClipSpace.y() / posOnClipSpace.w(),
                                        posOnClipSpace.z() / posOnClipSpace.w(),
                                        posOnClipSpace.w());
    return QVector4D(posOnNdcSpace.x() * m_nViewportWidth / 2.0f,
                     posOnNdcSpace.y() * m_nViewportHeight / 2.0f,
                     0.0f,
                     1.0f);
}

bool BBCamera::isFrustumContainPoint(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ, const QVector3D &point)
{
    return m_pFrustumCluster->contain(nFrustumIndexX, nFrustumIndexY, nFrustumIndexZ, point);
}

bool BBCamera::isSphereContainFrustum(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ,
                                      const QVector3D &center, float fRadius)
{
    return m_pFrustumCluster->containedInSphere(nFrustumIndexX, nFrustumIndexY, nFrustumIndexZ, center, fRadius);
}

bool BBCamera::isFrustumIntersectWithAABB(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ,
                                          BBAABBBoundingBox3D *pAABB)
{
    return m_pFrustumCluster->computeIntersectWithAABB(nFrustumIndexX, nFrustumIndexY, nFrustumIndexZ, pAABB);
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
