#include "BBFrustum.h"
#include "Utils/BBUtils.h"
#include "Render/BBCamera.h"
#include "BBPlane.h"
#include "BBRay.h"


/**
 * @brief BBFrustum::BBFrustum
 * @param pCamera
 * @param x                             screen coordinate       bottom-left
 * @param y                             original point is in the top-left
 * @param nWidth
 * @param nHeight
 */
BBFrustum::BBFrustum(BBCamera *pCamera, int x, int y, int nWidth, int nHeight)
{
    m_nBottomLeftX = x;
    m_nBottomLeftY = y;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    // 3D ray of the 4 vertexes
    BBRay topLeft = pCamera->createRayFromScreen(x, y - nHeight);
    BBRay topRight = pCamera->createRayFromScreen(x + nWidth, y - nHeight);
    BBRay bottomRight = pCamera->createRayFromScreen(x + nWidth, y);
    BBRay bottomLeft = pCamera->createRayFromScreen(x, y);

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
                        const QVector3D &point) const
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
bool BBFrustumCluster::contain(int nIndexX, int nIndexY, int nIndexZ, const QVector3D &point)
{
    BBPlane left = m_pXCrossSections[nIndexX];
    BBPlane right = m_pXCrossSections[nIndexX + 1].invert();
}

void BBFrustumCluster::calculateXCrossSections(BBCamera *pCamera, int nCount)
{
    int nStep = m_nWidth / (nCount + 1);
    if (nStep <= 0)
    {
        m_pXCrossSections = nullptr;
        return;
    }

    // +2 save m_pLeft and m_pRight for more convenient calculation
    m_pXCrossSections = new BBPlane[nCount + 2];
    m_pXCrossSections[0] = *m_pLeft;
    int x = m_nBottomLeftX;
    for (int i = 1; i < nCount + 1; i++)
    {
        x += nStep;
        BBRay topRay = pCamera->createRayFromScreen(x, m_nBottomLeftY - m_nHeight);
        BBRay bottomRay = pCamera->createRayFromScreen(x, m_nBottomLeftY);
        // just save positive direction
        m_pXCrossSections[i] = BBPlane(topRay.getFarPoint(), topRay.getNearPoint(), bottomRay.getNearPoint());
    }
    m_pXCrossSections[nCount + 1] = *m_pRight;
}

void BBFrustumCluster::calculateYCrossSections(BBCamera *pCamera, int nCount)
{
    int nStep = m_nHeight / (nCount + 1);
    if (nStep <= 0)
    {
        m_pYCrossSections = nullptr;
        return;
    }

    m_pYCrossSections = new BBPlane[nCount + 2];
    m_pYCrossSections[0] = *m_pBottom;
    int y = m_nBottomLeftY;
    for (int i = 1; i < nCount + 1; i++)
    {
        y -= nStep;
        BBRay leftRay = pCamera->createRayFromScreen(m_nBottomLeftX, y);
        BBRay rightRay = pCamera->createRayFromScreen(m_nBottomLeftX + m_nWidth, y);
        // just save positive direction
        m_pYCrossSections[i] = BBPlane(leftRay.getNearPoint(), rightRay.getNearPoint(), rightRay.getFarPoint());
    }
    m_pYCrossSections[nCount + 1] = *m_pTop;
}

void BBFrustumCluster::calculateZCrossSectionPoints(BBCamera *pCamera, int nCount)
{
    int nStep = pCamera->getDepth() / (nCount + 1);
    if (nStep <= 0)
    {
        m_pZCrossSectionPoints = nullptr;
        return;
    }

    BBRay ray = pCamera->createRayFromScreen(pCamera->getViewportWidth() / 2, pCamera->getViewportHeight() / 2);
    QVector3D dir = ray.getDirection();
    m_pZCrossSectionPoints = new QVector3D[nCount + 2];
    for (int i = 0; i < nCount + 2; i++)
    {
        m_pZCrossSectionPoints[i] = ray.getNearPoint() + nStep * i * dir;
    }
}
