#include "BBCoordinateComponent2D.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBDrawCall.h"
#include "Scene/BBRendererManager.h"


/**
 * @brief BBCoordinateComponent2D::BBCoordinateComponent2D
 * @param nCenterX
 * @param nCenterY
 * @param nWidth
 * @param nHeight
 */
BBCoordinateComponent2D::BBCoordinateComponent2D(int x, int y)
    : BBRenderableObject2D(x, y, 120, 120)
{
    m_SelectedAxis = BBAxisName::AxisNULL;
}

void BBCoordinateComponent2D::init()
{
    m_pCurrentMaterial = BBRendererManager::createCoordinateUIMaterial();
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0, 0.0f, 0.0f);
    BBRenderableObject::init();
}

void BBCoordinateComponent2D::setSelectedAxis(const BBAxisFlags &axis)
{
    BB_PROCESS_ERROR_RETURN((axis ^ m_SelectedAxis));
    // The axis that was selected last time returns to normal
    if (m_SelectedAxis != BBAxisName::AxisNULL)
    {
        setVertexColor(m_SelectedAxis, false);
    }
    if (axis != BBAxisName::AxisNULL)
    {
        setVertexColor(axis, true);
    }
    m_SelectedAxis = axis;
}


/**
 * @brief BBPositionCoordinateComponent2D::BBPositionCoordinateComponent2D
 */
BBPositionCoordinateComponent2D::BBPositionCoordinateComponent2D(int x, int y)
    : BBCoordinateComponent2D(x, y)
{

}

void BBPositionCoordinateComponent2D::init()
{
    m_pVBO = new BBVertexBufferObject(18);

    // GL_TRIANGLES
    m_pVBO->setPosition(0, 1.2f, 0.0f, 0.0f);
    m_pVBO->setPosition(1, 0.85f, 0.1f, 0.0f);
    m_pVBO->setPosition(2, 0.85f, -0.1f, 0.0f);
    m_pVBO->setColor(0, BBConstant::m_Red);
    m_pVBO->setColor(1, BBConstant::m_Red);
    m_pVBO->setColor(2, BBConstant::m_Red);
    m_pVBO->setPosition(3, 0.0f, 1.2f, 0.0f);
    m_pVBO->setPosition(4, 0.1f, 0.85f, 0.0f);
    m_pVBO->setPosition(5, -0.1f, 0.85f, 0.0f);
    m_pVBO->setColor(3, BBConstant::m_Green);
    m_pVBO->setColor(4, BBConstant::m_Green);
    m_pVBO->setColor(5, BBConstant::m_Green);
    // GL_LINES
    m_pVBO->setPosition(6, 1.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(7, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(6, BBConstant::m_Red);
    m_pVBO->setColor(7, BBConstant::m_Red);
    m_pVBO->setPosition(8, 0.0f, 1.0f, 0.0f);
    m_pVBO->setPosition(9, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(8, BBConstant::m_Green);
    m_pVBO->setColor(9, BBConstant::m_Green);
    m_pVBO->setPosition(10, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(11, 0.3f, 0.3f, 0.0f);
    m_pVBO->setColor(10, BBConstant::m_Blue);
    m_pVBO->setColor(11, BBConstant::m_Blue);
    m_pVBO->setPosition(12, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(13, 0.3f, 0.3f, 0.0f);
    m_pVBO->setColor(12, BBConstant::m_Blue);
    m_pVBO->setColor(13, BBConstant::m_Blue);
    // GL_QUADS
    m_pVBO->setPosition(14, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(15, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(16, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(17, 0.3f, 0.0f, 0.0f);
    m_pVBO->setColor(14, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(15, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(16, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(17, BBConstant::m_BlueTransparency);

    BBCoordinateComponent2D::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLES, 0, 6);
    appendDrawCall(pDrawCall);

    BBDrawCall *pDrawCall2 = new BBDrawCall;
    pDrawCall2->setMaterial(m_pCurrentMaterial);
    pDrawCall2->setVBO(m_pVBO, GL_LINES, 6, 8);
    appendDrawCall(pDrawCall2);

    BBDrawCall *pDrawCall3 = new BBDrawCall;
    pDrawCall3->setMaterial(m_pCurrentMaterial);
    pDrawCall3->setVBO(m_pVBO, GL_QUADS, 14, 4);
    appendDrawCall(pDrawCall3);
}

void BBPositionCoordinateComponent2D::setVertexColor(const BBAxisFlags &axis, bool bSelected)
{
    QVector3D color;
    if (axis & BBAxisName::AxisX)
    {
        if (bSelected)
        {
            color = BBConstant::m_Yellow;
        }
        else
        {
            color = BBConstant::m_Red;
        }
        m_pVBO->setColor(0, color);
        m_pVBO->setColor(1, color);
        m_pVBO->setColor(2, color);
        m_pVBO->setColor(6, color);
        m_pVBO->setColor(7, color);
    }
    if (axis & BBAxisName::AxisY)
    {
        if (bSelected)
        {
            color = BBConstant::m_Yellow;
        }
        else
        {
            color = BBConstant::m_Green;
        }
        m_pVBO->setColor(3, color);
        m_pVBO->setColor(4, color);
        m_pVBO->setColor(5, color);
        m_pVBO->setColor(8, color);
        m_pVBO->setColor(9, color);
    }
    m_pVBO->submitData();
}


/**
 * @brief BBCoordinateCircle2D::BBCoordinateCircle2D
 * @param x
 * @param y
 */
BBCoordinateCircle2D::BBCoordinateCircle2D(int x, int y, int nWidth, int nHeight)
    : BBCoordinateCircle(x, y, 0, 0, -90, 0, 1, nWidth, nHeight)
{

}

void BBCoordinateCircle2D::init()
{
    BBCoordinateCircle::init();
    setCurrentMaterial(BBRendererManager::createCoordinateUIMaterial());
}

void BBCoordinateCircle2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}


/**
 * @brief BBCoordinateTickMark2D::BBCoordinateTickMark2D
 * @param x
 * @param y
 */
BBCoordinateTickMark2D::BBCoordinateTickMark2D(int x, int y, int nWidth, int nHeight)
    : BBCoordinateTickMark(x, y, 0, 0, -90, 0, 1, nWidth, nHeight)
{

}

void BBCoordinateTickMark2D::init()
{
    BBCoordinateTickMark::init();
    setCurrentMaterial(BBRendererManager::createCoordinateUIMaterial());
}

void BBCoordinateTickMark2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}


/**
 * @brief BBCoordinateSector2D::BBCoordinateSector2D
 * @param x
 * @param y
 */
BBCoordinateSector2D::BBCoordinateSector2D(int x, int y, int nWidth, int nHeight)
    : BBCoordinateSector(x, y, 0, 0, -90, 0, 1, nWidth, nHeight)
{

}

void BBCoordinateSector2D::init()
{
    BBCoordinateSector::init();
    setCurrentMaterial(BBRendererManager::createCoordinateUIMaterial());
}

void BBCoordinateSector2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}


/**
 * @brief BBRotationCoordinateComponent2D::BBRotationCoordinateComponent2D
 * @param x
 * @param y
 */
BBRotationCoordinateComponent2D::BBRotationCoordinateComponent2D(int x, int y)
    : BBCoordinateComponent2D(x, y)
{

}

void BBRotationCoordinateComponent2D::init()
{
    m_pVBO = new BBVertexBufferObject(26);

    for (int i = 0; i < 13; i++)
    {
        float c1 = cosf(0.1309 * i);
        float s1 = sinf(0.1309 * i);
        float c2 = 0.8f * c1;
        float s2 = 0.8f * s1;
        m_pVBO->setPosition(i, c1, s1, 0.0f);
        m_pVBO->setColor(i, BBConstant::m_Blue);
        m_pVBO->setPosition(i + 13, c2, s2, 0.0f);
        m_pVBO->setColor(i + 13, BBConstant::m_Blue);
    }

    m_nIndexCount = 48;
    m_pIndexes = new unsigned short[m_nIndexCount];

    for (int i = 0; i < 12; i++)
    {
        m_pIndexes[i * 4] = i;
        m_pIndexes[i * 4 + 1] = i + 1;
        m_pIndexes[i * 4 + 2] = i + 14;
        m_pIndexes[i * 4 + 3] = i + 13;
    }

    BBCoordinateComponent2D::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_QUADS, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBRotationCoordinateComponent2D::setVertexColor(const BBAxisFlags &axis, bool bSelected)
{
    if (axis & (BBAxisName::AxisX | BBAxisName::AxisY))
    {
        QVector3D color;
        if (bSelected)
        {
            color = BBConstant::m_Yellow;
        }
        else
        {
            color = BBConstant::m_Blue;
        }
        for (int i = 0; i < m_pVBO->getVertexCount(); i++)
        {
            m_pVBO->setColor(i, color);
        }
    }
    m_pVBO->submitData();
}


/**
 * @brief BBScaleCoordinateComponent2D::BBScaleCoordinateComponent2D
 * @param x
 * @param y
 */
BBScaleCoordinateComponent2D::BBScaleCoordinateComponent2D(int x, int y)
    : BBCoordinateComponent2D(x, y)
{

}

void BBScaleCoordinateComponent2D::init()
{
    m_pVBO = new BBVertexBufferObject(20);

    // GL_QUADS
    m_pVBO->setPosition(0, 1.08f, 0.08f, 0.0f);
    m_pVBO->setPosition(1, 0.92f, 0.08f, 0.0f);
    m_pVBO->setPosition(2, 0.92f, -0.08f, 0.0f);
    m_pVBO->setPosition(3, 1.08f, -0.08f, 0.0f);
    m_pVBO->setColor(0, BBConstant::m_Red);
    m_pVBO->setColor(1, BBConstant::m_Red);
    m_pVBO->setColor(2, BBConstant::m_Red);
    m_pVBO->setColor(3, BBConstant::m_Red);
    m_pVBO->setPosition(4, 0.08f, 1.08f, 0.0f);
    m_pVBO->setPosition(5, -0.08f, 1.08f, 0.0f);
    m_pVBO->setPosition(6, -0.08f, 0.92f, 0.0f);
    m_pVBO->setPosition(7, 0.08f, 0.92f, 0.0f);
    m_pVBO->setColor(4, BBConstant::m_Green);
    m_pVBO->setColor(5, BBConstant::m_Green);
    m_pVBO->setColor(6, BBConstant::m_Green);
    m_pVBO->setColor(7, BBConstant::m_Green);
    // GL_QUADS
    m_pVBO->setPosition(8, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(9, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(10, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(11, 0.3f, 0.0f, 0.0f);
    m_pVBO->setColor(8, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(9, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(10, BBConstant::m_BlueTransparency);
    m_pVBO->setColor(11, BBConstant::m_BlueTransparency);
    // GL_LINES
    m_pVBO->setPosition(12, 1.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(13, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(12, BBConstant::m_Red);
    m_pVBO->setColor(13, BBConstant::m_Red);
    m_pVBO->setPosition(14, 0.0f, 1.0f, 0.0f);
    m_pVBO->setPosition(15, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(14, BBConstant::m_Green);
    m_pVBO->setColor(15, BBConstant::m_Green);
    m_pVBO->setPosition(16, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(17, 0.3f, 0.3f, 0.0f);
    m_pVBO->setColor(16, BBConstant::m_Blue);
    m_pVBO->setColor(17, BBConstant::m_Blue);
    m_pVBO->setPosition(18, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(19, 0.3f, 0.3f, 0.0f);
    m_pVBO->setColor(18, BBConstant::m_Blue);
    m_pVBO->setColor(19, BBConstant::m_Blue);

    BBCoordinateComponent2D::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 12);
    appendDrawCall(pDrawCall);

    BBDrawCall *pDrawCall2 = new BBDrawCall;
    pDrawCall2->setMaterial(m_pCurrentMaterial);
    pDrawCall2->setVBO(m_pVBO, GL_LINES, 12, 8);
    appendDrawCall(pDrawCall2);
}

void BBScaleCoordinateComponent2D::setVertexColor(const BBAxisFlags &axis, bool bSelected)
{
    QVector3D color;
    if (axis & BBAxisName::AxisX)
    {
        if (bSelected)
        {
            color = BBConstant::m_Yellow;
        }
        else
        {
            color = BBConstant::m_Red;
        }
        m_pVBO->setColor(0, color);
        m_pVBO->setColor(1, color);
        m_pVBO->setColor(2, color);
        m_pVBO->setColor(3, color);
        m_pVBO->setColor(12, color);
        m_pVBO->setColor(13, color);
    }
    if (axis & BBAxisName::AxisY)
    {
        if (bSelected)
        {
            color = BBConstant::m_Yellow;
        }
        else
        {
            color = BBConstant::m_Green;
        }
        m_pVBO->setColor(4, color);
        m_pVBO->setColor(5, color);
        m_pVBO->setColor(6, color);
        m_pVBO->setColor(7, color);
        m_pVBO->setColor(14, color);
        m_pVBO->setColor(15, color);
    }
    m_pVBO->submitData();
}

void BBScaleCoordinateComponent2D::scale(int nDeltaX, int nDeltaY)
{
    float x = 1.0f * nDeltaX / m_nWidth;
    float y = 1.0f * nDeltaY / m_nHeight;

    m_pVBO->setPosition(0, 1.08f + x, 0.08f, 0.0f);
    m_pVBO->setPosition(1, 0.92f + x, 0.08f, 0.0f);
    m_pVBO->setPosition(2, 0.92f + x, -0.08f, 0.0f);
    m_pVBO->setPosition(3, 1.08f + x, -0.08f, 0.0f);
    m_pVBO->setPosition(12, 1.0f + x, 0.0f, 0.0f);

    m_pVBO->setPosition(4, 0.08f, 1.08f + y, 0.0f);
    m_pVBO->setPosition(5, -0.08f, 1.08f + y, 0.0f);
    m_pVBO->setPosition(6, -0.08f, 0.92f + y, 0.0f);
    m_pVBO->setPosition(7, 0.08f, 0.92f + y, 0.0f);
    m_pVBO->setPosition(14, 0.0f, 1.0f + y, 0.0f);

    m_pVBO->submitData();
}

void BBScaleCoordinateComponent2D::reset()
{
    scale(0, 0);
}
