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
