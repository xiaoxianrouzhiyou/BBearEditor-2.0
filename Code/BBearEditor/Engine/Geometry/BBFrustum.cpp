#include "BBFrustum.h"
#include "Utils/BBUtils.h"
#include "Render/BBCamera.h"
#include "BBPlane.h"
#include "BBRay.h"


/**
 * @brief BBFrustum::BBFrustum
 * @param pCamera
 * @param x                             screen coordinate       top-left
 * @param y                             original point is in the top-left
 * @param nWidth
 * @param nHeight
 */
BBFrustum::BBFrustum(BBCamera *pCamera, int x, int y, int nWidth, int nHeight)
{
    m_nTopLeftX = x;
    m_nTopLeftY = y;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    // 3D ray of the 4 vertexes
    BBRay topLeft = pCamera->createRayFromScreen(x, y);
    BBRay topRight = pCamera->createRayFromScreen(x + nWidth, y);
    BBRay bottomRight = pCamera->createRayFromScreen(x + nWidth, y + nHeight);
    BBRay bottomLeft = pCamera->createRayFromScreen(x, y + nHeight);

    m_pLeft = new BBPlane(topLeft.getNearPoint(), bottomLeft.getNearPoint(), topLeft.getFarPoint());
    m_pRight = new BBPlane(topRight.getNearPoint(), topRight.getFarPoint(), bottomRight.getNearPoint());
    m_pTop = new BBPlane(topLeft.getNearPoint(), topLeft.getFarPoint(), topRight.getNearPoint());
    m_pBottom = new BBPlane(bottomLeft.getNearPoint(), bottomRight.getNearPoint(), bottomLeft.getFarPoint());
    m_pFront = new BBPlane(topLeft.getNearPoint(), topRight.getNearPoint(), bottomRight.getNearPoint());
    m_pBack = new BBPlane(topLeft.getFarPoint(), bottomLeft.getFarPoint(), bottomRight.getFarPoint());
}

BBFrustum::~BBFrustum()
{
    BB_SAFE_DELETE(m_pLeft);
    BB_SAFE_DELETE(m_pRight);
    BB_SAFE_DELETE(m_pTop);
    BB_SAFE_DELETE(m_pBottom);
    BB_SAFE_DELETE(m_pFront);
    BB_SAFE_DELETE(m_pBack);
}

bool BBFrustum::contain(const QVector3D &point) const
{
    if (m_pLeft->distance(point) < 0)
        return false;
    if (m_pRight->distance(point) < 0)
        return false;
    if (m_pTop->distance(point) < 0)
        return false;
    if (m_pBottom->distance(point) < 0)
        return false;
    return true;
}

bool BBFrustum::contain(const BBPlane &left, const BBPlane &right,
                        const BBPlane &top, const BBPlane &bottom,
                        const BBPlane &front, const BBPlane &back,
                        const QVector3D &point)
{
    if (left.distance(point) < 0)
        return false;
    if (right.distance(point) < 0)
        return false;
    if (top.distance(point) < 0)
        return false;
    if (bottom.distance(point) < 0)
        return false;
    if (front.distance(point) < 0)
        return false;
    if (back.distance(point) < 0)
        return false;
    return true;
}

/**
 * @brief BBFrustum::containWithInvertedRightAndBottom          invert direction of right and bottom
 * @param left
 * @param right
 * @param top
 * @param bottom
 * @param front
 * @param back
 * @param point
 * @return
 */
bool BBFrustum::containWithInvertedRightAndTop(const BBPlane &left, const BBPlane &right,
                                               const BBPlane &top, const BBPlane &bottom,
                                               const QVector3D &pointOnFront, const QVector3D &pointOnBack,
                                               const QVector3D &point)
{
    if (left.distance(point) < 0)
        return false;
    if (right.distance(point) > 0)
        return false;
    if (top.distance(point) > 0)
        return false;
    if (bottom.distance(point) < 0)
        return false;
    if (BBPlane::distance(pointOnFront, m_pFront->getNormal(), point) < 0)
        return false;
    if (BBPlane::distance(pointOnBack, m_pBack->getNormal(), point) < 0)
        return false;
    return true;
}


/**
 * @brief BBFrustumCluster::BBFrustumCluster
 * @param pFrustum
 * @param nCountX
 * @param nCountY
 * @param nCountZ
 */
BBFrustumCluster::BBFrustumCluster(BBCamera *pCamera,
                                   int x, int y,
                                   int nWidth, int nHeight,
                                   int nCountX, int nCountY, int nCountZ)
    : BBFrustum(pCamera, x, y, nWidth, nHeight)
{
    m_pXCrossSections = nullptr;
    m_pYCrossSections = nullptr;
    m_pZCrossSectionPoints = nullptr;
    // Divide pFrustum into nCountX parts along X and save nCountX-1 planes
    calculateXCrossSections(pCamera, nCountX - 1);
    calculateYCrossSections(pCamera, nCountY - 1);
    calculateZCrossSectionPoints(pCamera, nCountZ - 1);
}

BBFrustumCluster::~BBFrustumCluster()
{
    BB_SAFE_DELETE_ARRAY(m_pXCrossSections);
    BB_SAFE_DELETE_ARRAY(m_pYCrossSections);
    BB_SAFE_DELETE_ARRAY(m_pZCrossSectionPoints);
}

/**
 * @brief BBFrustumCluster::contain         whether point is contained in the frustum specified by index
 * @param nIndexX
 * @param nIndexY
 * @param nIndexZ
 * @param point
 * @return
 */
bool BBFrustumCluster::contain(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ, const QVector3D &point)
{
    BBPlane left = m_pXCrossSections[nFrustumIndexX];
    BBPlane right = m_pXCrossSections[nFrustumIndexX + 1];
    BBPlane top = m_pYCrossSections[nFrustumIndexY];
    BBPlane bottom = m_pYCrossSections[nFrustumIndexY + 1];
    QVector3D pointOnFront = m_pZCrossSectionPoints[nFrustumIndexZ];
    QVector3D pointOnBack = m_pZCrossSectionPoints[nFrustumIndexZ + 1];
    return containWithInvertedRightAndTop(left, right, top, bottom, pointOnFront, pointOnBack, point);
}

bool BBFrustumCluster::containedInSphere(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ,
                                         const QVector3D &sphereCenter, float fSphereRadius)
{
    // to do
    return true;
}

void BBFrustumCluster::calculateXCrossSections(BBCamera *pCamera, int nCount)
{
    // +2 save m_pLeft and m_pRight for more convenient calculation
    m_pXCrossSections = new BBPlane[nCount + 2];
    m_pXCrossSections[0] = *m_pLeft;
    m_pXCrossSections[nCount + 1] = BBPlane::invert(*m_pRight);

    int nStep = m_nWidth / (nCount + 1);   
    int x = m_nTopLeftX;
    for (int i = 1; i < nCount + 1; i++)
    {
        x += nStep;
        BBRay topRay = pCamera->createRayFromScreen(x, m_nTopLeftY);
        BBRay bottomRay = pCamera->createRayFromScreen(x, m_nTopLeftY + m_nHeight);
        // just save positive direction
        m_pXCrossSections[i] = BBPlane(topRay.getFarPoint(), topRay.getNearPoint(), bottomRay.getNearPoint());
    }

}

void BBFrustumCluster::calculateYCrossSections(BBCamera *pCamera, int nCount)
{
    m_pYCrossSections = new BBPlane[nCount + 2];
    m_pYCrossSections[0] = BBPlane::invert(*m_pTop);
    m_pYCrossSections[nCount + 1] = *m_pBottom;

    int nStep = m_nHeight / (nCount + 1);
    int y = m_nTopLeftY;
    for (int i = 1; i < nCount + 1; i++)
    {
        y += nStep;
        BBRay leftRay = pCamera->createRayFromScreen(m_nTopLeftX, y);
        BBRay rightRay = pCamera->createRayFromScreen(m_nTopLeftX + m_nWidth, y);
        // just save positive direction
        m_pYCrossSections[i] = BBPlane(leftRay.getNearPoint(), rightRay.getNearPoint(), rightRay.getFarPoint());
    }
}

void BBFrustumCluster::calculateZCrossSectionPoints(BBCamera *pCamera, int nCount)
{
    int nStep = pCamera->getDepth() / (nCount + 1);

    BBRay ray = pCamera->createRayFromScreen(pCamera->getViewportWidth() / 2, pCamera->getViewportHeight() / 2);
    QVector3D dir = ray.getDirection();
    m_pZCrossSectionPoints = new QVector3D[nCount + 2];
    for (int i = 0; i < nCount + 2; i++)
    {
        m_pZCrossSectionPoints[i] = ray.getNearPoint() + nStep * i * dir;
    }
}
