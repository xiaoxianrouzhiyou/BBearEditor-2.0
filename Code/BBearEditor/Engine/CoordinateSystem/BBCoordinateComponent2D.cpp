#include "BBCoordinateComponent2D.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBDrawCall.h"


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
    m_pCurrentMaterial->init("coordinate2D",
                             BB_PATH_RESOURCE_SHADER(coordinate2D.vert),
                             BB_PATH_RESOURCE_SHADER(coordinate.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setZFunc(GL_ALWAYS);

    BBRenderableObject2D::init();
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
