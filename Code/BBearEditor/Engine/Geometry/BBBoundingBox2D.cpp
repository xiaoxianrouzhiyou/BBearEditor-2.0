#include "BBBoundingBox2D.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBDrawCall.h"


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

void BBBoundingBox2D::init()
{
    BBRenderableObject2D::init();
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 0.0f, 0.0f, 0.0f, 0.0f);
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

void BBAABBBoundingBox2D::init()
{
    m_pVBO = new BBVertexBufferObject(8);

    m_pVBO->setPosition(0, 0.5f, 0.5f, 0.0f);
    m_pVBO->setPosition(1, -0.5f, 0.5f, 0.0f);
    m_pVBO->setPosition(2, -0.5f, 0.5f, 0.0f);
    m_pVBO->setPosition(3, -0.5f, -0.5f, 0.0f);
    m_pVBO->setPosition(4, -0.5f, -0.5f, 0.0f);
    m_pVBO->setPosition(5, 0.5f, -0.5f, 0.0f);
    m_pVBO->setPosition(6, 0.5f, -0.5f, 0.0f);
    m_pVBO->setPosition(7, 0.5f, 0.5f, 0.0f);
    for (int i = 0; i < 8; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_OrangeRed);
    }

    BBBoundingBox2D::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_LINES, 0, 8);
    appendDrawCall(pDrawCall);
}

bool BBAABBBoundingBox2D::hit(int x, int y)
{
    if (x > m_Position.x() + m_nHalfLengthX)
        return false;
    if (x < m_Position.x() - m_nHalfLengthX)
        return false;
    if (y > m_Position.y() + m_nHalfLengthY)
        return false;
    if (y < m_Position.y() - m_nHalfLengthY)
        return false;
    return true;
}


/**
 * @brief BBQuarterCircleBoundingBox2D::BBQuarterCircleBoundingBox2D
 * @param x
 * @param y
 * @param nRadius
 */
BBQuarterCircleBoundingBox2D::BBQuarterCircleBoundingBox2D(int x, int y, int nRadius)
    : BBBoundingBox2D(x, y, nRadius * 2, nRadius * 2)
{
    m_nRadius = nRadius;
}

bool BBQuarterCircleBoundingBox2D::hit(int x, int y)
{
    if (x < m_Position.x())
        return false;
    if (y < m_Position.y())
        return false;
    if (((x - m_Position.x()) * (x - m_Position.x()) + (y - m_Position.y()) * (y - m_Position.y())) > m_nRadius * m_nRadius)
        return false;
    return true;
}
