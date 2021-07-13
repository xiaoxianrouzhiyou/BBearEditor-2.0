#include "BBBoundingBox2D.h"


/**
 * @brief BBBoundingBox2D::BBBoundingBox2D
 */
BBBoundingBox2D::BBBoundingBox2D()
    : BBBoundingBox2D(0, 0, -1, -1)
{

}

BBBoundingBox2D::BBBoundingBox2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject2D(x, y, nWidth, nHeight)
{

}


/**
 * @brief BBAABBBoundingBox2D::BBAABBBoundingBox2D
 * @param fCenterX                                              3D coordinate
 * @param fCenterY                                              3D coordinate
 * @param fHalfLengthX                                          3D coordinate
 * @param fHalfLengthY                                          3D coordinate
 * @param nWidth                                                screen coordinate
 * @param nHeight                                               screen coordinate
 */
BBAABBBoundingBox2D::BBAABBBoundingBox2D(float fCenterX, float fCenterY, float fHalfLengthX, float fHalfLengthY,
                                         int nWidth, int nHeight)
    : BBBoundingBox2D(fCenterX * nWidth, fCenterY * nHeight, nWidth, nHeight)
{
    m_nHalfLengthX = fHalfLengthX * nWidth;
    m_nHalfLengthY = fHalfLengthY * nHeight;
}

BBAABBBoundingBox2D::BBAABBBoundingBox2D(int x, int y, int nHalfLengthX, int nHalfLengthY)
    : BBBoundingBox2D(x, y, nHalfLengthX * 2, nHalfLengthY * 2)
{
    m_nHalfLengthX = nHalfLengthX;
    m_nHalfLengthY = nHalfLengthY;
}

bool BBAABBBoundingBox2D::hit(int x, int y)
{
    if (x > m_nScreenX + m_nHalfLengthX)
        return false;
    if (x < m_nScreenX - m_nHalfLengthX)
        return false;
    if (y > m_nScreenY + m_nHalfLengthY)
        return false;
    if (y < m_nScreenY - m_nHalfLengthY)
        return false;
    return true;
}