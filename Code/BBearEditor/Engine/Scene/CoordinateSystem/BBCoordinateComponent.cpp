#include "BBCoordinateComponent.h"
#include "Render/BBMaterial.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/BufferObject/BBElementBufferObject.h"


/**
 * @brief BBCoordinateComponent::BBCoordinateComponent
 */
BBCoordinateComponent::BBCoordinateComponent()
    : BBCoordinateComponent(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateComponent::BBCoordinateComponent(float px, float py, float pz,
                                             float rx, float ry, float rz,
                                             float sx, float sy, float sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_SelectedAxis = BBAxisName::AxisNULL;
}

void BBCoordinateComponent::init()
{
    m_pCurrentMaterial->init("coordinate",
                             BB_PATH_RESOURCE_SHADER(coordinate.vert),
                             BB_PATH_RESOURCE_SHADER(coordinate.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setZFunc(GL_ALWAYS);
//    m_pCurrentMaterial->getBaseRenderPass()->setLineWidth(1.5f);

    BBRenderableObject::init();
}

void BBCoordinateComponent::setSelectedAxis(const BBAxisFlags &axis)
{
    if (axis == m_SelectedAxis)
        return;

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

void BBCoordinateComponent::setVertexColor(const BBAxisFlags &axis, bool bSelected)
{
    int count1 = m_pVBO->getVertexCount() / 3;
    int count2 = count1 * 2;
    int count3 = m_pVBO->getVertexCount();
    if (axis & BBAxisName::AxisX)
    {
        if (bSelected)
        {
            setVertexColor(0, count1, BBConstant::m_Yellow);
        }
        else
        {
            setVertexColor(0, count1, BBConstant::m_Red);
        }
    }
    if (axis & BBAxisName::AxisY)
    {
        if (bSelected)
        {
            setVertexColor(count1, count2, BBConstant::m_Yellow);
        }
        else
        {
            setVertexColor(count1, count2, BBConstant::m_Green);
        }
    }
    if (axis & BBAxisName::AxisZ)
    {
        if (bSelected)
        {
            setVertexColor(count2, count3, BBConstant::m_Yellow);
        }
        else
        {
            setVertexColor(count2, count3, BBConstant::m_Blue);
        }
    }
    // update
    m_pVBO->submitData();
}

void BBCoordinateComponent::setVertexColor(int start, int end, const QVector3D &color)
{
    for (int i = start; i < end; i++)
    {
        m_pVBO->setColor(i, color);
    }
}

void BBCoordinateComponent::setVertexColor(int start, int end, const QVector4D &color)
{
    for (int i = start; i < end; i++)
    {
        m_pVBO->setColor(i, color);
    }
}


/**
 * @brief BBCoordinateArrow::BBCoordinateArrow
 */
BBCoordinateArrow::BBCoordinateArrow()
    : BBCoordinateArrow(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateArrow::BBCoordinateArrow(float px, float py, float pz,
                                     float rx, float ry, float rz,
                                     float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateArrow::init()
{
    m_pVBO = new BBVertexBufferObject(39);

    m_pVBO->setPosition(0, 1.2f, 0.0f, 0.0f);
    m_pVBO->setPosition(1, 0.85f, 0.1f, 0.0f);
    m_pVBO->setPosition(2, 0.85f, 0.086603f, 0.05f);
    m_pVBO->setPosition(3, 0.85f, 0.05f, 0.086603f);
    m_pVBO->setPosition(4, 0.85f, 0.0f, 0.1f);
    m_pVBO->setPosition(5, 0.85f, -0.05f, 0.086603f);
    m_pVBO->setPosition(6, 0.85f, -0.086603f, 0.05f);
    m_pVBO->setPosition(7, 0.85f, -0.1f, 0.0f);
    m_pVBO->setPosition(8, 0.85f, -0.086603f, -0.05f);
    m_pVBO->setPosition(9, 0.85f, -0.05f, -0.086603f);
    m_pVBO->setPosition(10, 0.85f, 0.0f, -0.1f);
    m_pVBO->setPosition(11, 0.85f, 0.05f, -0.086603f);
    m_pVBO->setPosition(12, 0.85f, 0.086603f, -0.05f);
    for (int i = 0; i < 13; i++)
        m_pVBO->setColor(i, BBConstant::m_Red);

    m_pVBO->setPosition(13, 0.0f, 1.2f, 0.0f);
    m_pVBO->setPosition(14, 0.1f, 0.85f, 0.0f);
    m_pVBO->setPosition(15, 0.086603f, 0.85f, 0.05f);
    m_pVBO->setPosition(16, 0.05f, 0.85f, 0.086603f);
    m_pVBO->setPosition(17, 0.0f, 0.85f, 0.1f);
    m_pVBO->setPosition(18, -0.05f, 0.85f, 0.086603f);
    m_pVBO->setPosition(19, -0.086603f, 0.85f, 0.05f);
    m_pVBO->setPosition(20, -0.1f, 0.85f, 0.0f);
    m_pVBO->setPosition(21, -0.086603f, 0.85f, -0.05f);
    m_pVBO->setPosition(22, -0.05f, 0.85f, -0.086603f);
    m_pVBO->setPosition(23, 0.0f, 0.85f, -0.1f);
    m_pVBO->setPosition(24, 0.05f, 0.85f, -0.086603f);
    m_pVBO->setPosition(25, 0.086603f, 0.85f, -0.05f);
    for (int i = 13; i < 26; i++)
        m_pVBO->setColor(i, BBConstant::m_Green);

    m_pVBO->setPosition(26, 0.0f, 0.0f, 1.2f);
    m_pVBO->setPosition(27, 0.1f, 0.0f, 0.85f);
    m_pVBO->setPosition(28, 0.086603f, 0.05f, 0.85f);
    m_pVBO->setPosition(29, 0.05f, 0.086603f, 0.85f);
    m_pVBO->setPosition(30, 0.0f, 0.1f, 0.85f);
    m_pVBO->setPosition(31, -0.05f, 0.086603f, 0.85f);
    m_pVBO->setPosition(32, -0.086603f, 0.05f, 0.85f);
    m_pVBO->setPosition(33, -0.1f, 0.0f, 0.85f);
    m_pVBO->setPosition(34, -0.086603f, -0.05f, 0.85f);
    m_pVBO->setPosition(35, -0.05f, -0.086603f, 0.85f);
    m_pVBO->setPosition(36, 0.0f, -0.1f, 0.85f);
    m_pVBO->setPosition(37, 0.05f, -0.086603f, 0.85f);
    m_pVBO->setPosition(38, 0.086603f, -0.05f, 0.85f);
    for (int i = 26; i < 39; i++)
        m_pVBO->setColor(i, BBConstant::m_Blue);

    m_nIndexCount = 108;
    unsigned short indexes[] = {0, 1, 2, 0, 2, 3, 0, 3, 4,
                                0, 4, 5, 0, 5, 6, 0, 6, 7,
                                0, 7, 8, 0, 8, 9, 0, 9, 10,
                                0, 10, 11, 0, 11, 12, 0, 12, 1,
                                13, 14, 15, 13, 15, 16, 13, 16, 17,
                                13, 17, 18, 13, 18, 19, 13, 19, 20,
                                13, 20, 21, 13, 21, 22, 13, 22, 23,
                                13, 23, 24, 13, 24, 25, 13, 25, 14,
                                26, 27, 28, 26, 28, 29, 26, 29, 30,
                                26, 30, 31, 26, 31, 32, 26, 32, 33,
                                26, 33, 34, 26, 34, 35, 26, 35, 36,
                                26, 36, 37, 26, 37, 38, 26, 38, 27};
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < m_nIndexCount; i++)
    {
        m_pIndexes[i] = indexes[i];
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_TRIANGLES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}


/**
 * @brief BBCoordinateAxis::BBCoordinateAxis
 */
BBCoordinateAxis::BBCoordinateAxis()
    : BBCoordinateAxis(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateAxis::BBCoordinateAxis(float px, float py, float pz,
                                   float rx, float ry, float rz,
                                   float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateAxis::init()
{
    m_pVBO = new BBVertexBufferObject(6);

    m_pVBO->setPosition(0, 1.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(1, 0.0f, 0.0f, 0.0f);

    m_pVBO->setPosition(2, 0.0f, 1.0f, 0.0f);
    m_pVBO->setPosition(3, 0.0f, 0.0f, 0.0f);

    m_pVBO->setPosition(4, 0.0f, 0.0f, 1.0f);
    m_pVBO->setPosition(5, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Red);
    }
    for (int i = 2; i < 4; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Green);
    }
    for (int i = 4; i < 6; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Blue);
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_LINES, 0, 6);
    appendDrawCall(pDrawCall);
}


/**
 * @brief BBCoordinateRectFace::BBCoordinateRectFace
 */
BBCoordinateRectFace::BBCoordinateRectFace()
    : BBCoordinateRectFace(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateRectFace::BBCoordinateRectFace(float px, float py, float pz,
                                           float rx, float ry, float rz,
                                           float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateRectFace::init()
{
    m_pVBO = new BBVertexBufferObject(24);

    // face
    m_pVBO->setPosition(0, 0.0f, 0.3f, 0.3f);
    m_pVBO->setPosition(1, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(2, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(3, 0.0f, 0.3f, 0.0f);

    m_pVBO->setPosition(4, 0.3f, 0.0f, 0.3f);
    m_pVBO->setPosition(5, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(6, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(7, 0.3f, 0.0f, 0.0f);

    m_pVBO->setPosition(8, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(9, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(10, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(11, 0.3f, 0.0f, 0.0f);

    // line
    m_pVBO->setPosition(12, 0.0f, 0.3f, 0.3f);
    m_pVBO->setPosition(13, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(14, 0.0f, 0.3f, 0.3f);
    m_pVBO->setPosition(15, 0.0f, 0.0f, 0.3f);

    m_pVBO->setPosition(16, 0.3f, 0.0f, 0.3f);
    m_pVBO->setPosition(17, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(18, 0.3f, 0.0f, 0.3f);
    m_pVBO->setPosition(19, 0.0f, 0.0f, 0.3f);

    m_pVBO->setPosition(20, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(21, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(22, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(23, 0.0f, 0.3f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_RedTransparency);
    }
    for (int i = 4; i < 8; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_GreenTransparency);
    }
    for (int i = 8; i < 12; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_BlueTransparency);
    }
    for (int i = 12; i < 16; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Red);
    }
    for (int i = 16; i < 20; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Green);
    }
    for (int i = 20; i < 24; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Blue);
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 12);
    appendDrawCall(pDrawCall);

    BBDrawCall *pDrawCall2 = new BBDrawCall;
    pDrawCall2->setMaterial(m_pCurrentMaterial);
    pDrawCall2->setVBO(m_pVBO, GL_LINES, 12, 24);
    appendDrawCall(pDrawCall2);
}


/**
 * @brief BBCoordinateQuarterCircle::BBCoordinateQuarterCircle
 */
BBCoordinateQuarterCircle::BBCoordinateQuarterCircle()
    : BBCoordinateQuarterCircle(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateQuarterCircle::BBCoordinateQuarterCircle(float px, float py, float pz,
                                                     float rx, float ry, float rz,
                                                     float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateQuarterCircle::init()
{
    m_pVBO = new BBVertexBufferObject(78);

    for (int i = 0; i < 13; i++)
    {
        float c1 = cosf(0.1309 * i);
        float s1 = sinf(0.1309 * i);
        float c2 = 0.8f * c1;
        float s2 = 0.8f * s1;
        m_pVBO->setPosition(i, 0.0f, c1, s1);
        m_pVBO->setColor(i, BBConstant::m_Red);
        m_pVBO->setPosition(i + 13, 0.0f, c2, s2);
        m_pVBO->setColor(i + 13, BBConstant::m_Red);
        m_pVBO->setPosition(i + 26, c1, 0.0f, s1);
        m_pVBO->setColor(i + 26, BBConstant::m_Green);
        m_pVBO->setPosition(i + 39, c2, 0.0f, s2);
        m_pVBO->setColor(i + 39, BBConstant::m_Green);
        m_pVBO->setPosition(i + 52, c1, s1, 0.0f);
        m_pVBO->setColor(i + 52, BBConstant::m_Blue);
        m_pVBO->setPosition(i + 65, c2, s2, 0.0f);
        m_pVBO->setColor(i + 65, BBConstant::m_Blue);
    }

    m_nIndexCount = 144;
    m_pIndexes = new unsigned short[m_nIndexCount];

    for (int i = 0; i < 12; i++)
    {
        m_pIndexes[i * 4] = i;
        m_pIndexes[i * 4 + 1] = i + 1;
        m_pIndexes[i * 4 + 2] = i + 14;
        m_pIndexes[i * 4 + 3] = i + 13;

        m_pIndexes[i * 4 + 48] = i + 26;
        m_pIndexes[i * 4 + 49] = i + 27;
        m_pIndexes[i * 4 + 50] = i + 40;
        m_pIndexes[i * 4 + 51] = i + 39;

        m_pIndexes[i * 4 + 96] = i + 52;
        m_pIndexes[i * 4 + 97] = i + 53;
        m_pIndexes[i * 4 + 98] = i + 66;
        m_pIndexes[i * 4 + 99] = i + 65;
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_QUADS, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}


/**
 * @brief BBCoordinateCircle::BBCoordinateCircle
 */
BBCoordinateCircle::BBCoordinateCircle()
    : BBCoordinateCircle(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateCircle::BBCoordinateCircle(float px, float py, float pz,
                                       float rx, float ry, float rz,
                                       float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateCircle::init()
{
    m_pVBO = new BBVertexBufferObject(96);

    for (int i = 0; i < 48; i++)
    {
        float c1 = cosf(0.1309 * i);
        float s1 = sinf(0.1309 * i);
        float c2 = 0.8f * c1;
        float s2 = 0.8f * s1;
        // Circle
        m_pVBO->setPosition(2 * i, 0.0f, c1, s1);
        m_pVBO->setColor(2 * i, BBConstant::m_Yellow);
        m_pVBO->setPosition(2 * i + 1, 0.0f, c2, s2);
        m_pVBO->setColor(2 * i + 1, BBConstant::m_Yellow);
    }

    m_nIndexCount = 192;
    m_pIndexes = new unsigned short[m_nIndexCount];

    for (int i = 0, j = 0; i < 48; i++, j+=2)
    {
        m_pIndexes[i * 4] = j;
        m_pIndexes[i * 4 + 1] = j + 1;
        m_pIndexes[i * 4 + 2] = j + 3;
        m_pIndexes[i * 4 + 3] = j + 2;

    }
    m_pIndexes[190] = 1;
    m_pIndexes[191] = 0;

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_QUADS, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}


/**
 * @brief BBCoordinateTickMark::BBCoordinateTickMark
 */
BBCoordinateTickMark::BBCoordinateTickMark()
    : BBCoordinateTickMark(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateTickMark::BBCoordinateTickMark(float px, float py, float pz,
                                           float rx, float ry, float rz,
                                           float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateTickMark::init()
{
    // unit = 10 degree, 360/10*2 = 72 Circles
    m_pVBO = new BBVertexBufferObject(72);

    for (int i = 0; i < 36; i++)
    {
        float c1 = cosf(0.174533f * i);
        float s1 = sinf(0.174533f * i);
        float c2;
        float s2;
        //0 30 60... there is a longer tick mark
        if (i % 3 == 0)
        {
            c2 = 0.86f * c1;
            s2 = 0.86f * s1;
        }
        else
        {
            c2 = 0.93f * c1;
            s2 = 0.93f * s1;
        }

        // Circle
        m_pVBO->setPosition(2 * i, 0.0f, c1, s1);
        m_pVBO->setColor(2 * i, BBConstant::m_Gray);
        m_pVBO->setPosition(2 * i + 1, 0.0f, c2, s2);
        m_pVBO->setColor(2 * i + 1, BBConstant::m_Gray);
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_LINES, 0, 72);
    appendDrawCall(pDrawCall);
}


/**
 * @brief BBCoordinateSector::BBCoordinateSector
 */
BBCoordinateSector::BBCoordinateSector()
    : BBCoordinateSector(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateSector::BBCoordinateSector(float px, float py, float pz,
                                       float rx, float ry, float rz,
                                       float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    reset();
}

void BBCoordinateSector::init()
{
    // unit = 1     2pi/360
    // +1 is center of circle
    m_pVBO = new BBVertexBufferObject(361);
    m_pVBO->setPosition(360, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(360, BBConstant::m_GrayTransparency);
    for (int i = 0; i < 360; i++)
    {
        float c = 0.8f * cosf(0.017453f * i);
        float s = 0.8f * sinf(0.017453f * i);
        // on the circle
        m_pVBO->setPosition(i, 0.0f, c, s);
        m_pVBO->setColor(i, BBConstant::m_Gray);
    }

    m_nIndexCount = 1080;
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < 360; i++)
    {
        m_pIndexes[3 * i] = 360;
        m_pIndexes[3 * i + 1] = i;
        m_pIndexes[3 * i + 2] = i + 1;
    }
    m_pIndexes[1079] = 0;

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    appendDrawCall(pDrawCall);
}

void BBCoordinateSector::render(BBCamera *pCamera)
{
    if (m_nAngle != 0)
    {
        // unit = 1 degree, 1 triangle, 3 indexes
        int nIndexCount = abs(m_nAngle) * 3;
        unsigned short *pIndexes = new unsigned short[nIndexCount];

        // if the angle is positive, show sector in front semicircle
        if (m_nAngle > 0)
        {
            for (int i = 0; i < nIndexCount; i++)
            {
                pIndexes[i] = m_pIndexes[i];
            }
        }
        // if the angle is negative, show sector in back semicircle
        else
        {
            for (int i = 0; i < nIndexCount; i++)
            {
                pIndexes[i] = m_pIndexes[m_nIndexCount - i - 1];
            }
        }

        m_pEBO->submitData(pIndexes, nIndexCount);
        m_pDrawCalls->setEBO(m_pEBO, GL_TRIANGLES, abs(m_nAngle) * 3, 0);
    }

    BBCoordinateComponent::render(pCamera);
}

void BBCoordinateSector::setAngle(int nDeltaAngle)
{
    m_nAngle += nDeltaAngle;
    // m_nAngle in [-360, 360]
    if (m_nAngle > 360)
    {
        m_nAngle -= 360;
    }
    else if (m_nAngle < -360)
    {
        m_nAngle += 360;
    }
}

void BBCoordinateSector::reset()
{
    m_nAngle = 0;
}


/**
 * @brief BBCoordinateCube::BBCoordinateCube
 */
QVector3D BBCoordinateCube::m_Sign[8] = { QVector3D(1, 1, 1),
                                          QVector3D(1, -1, 1),
                                          QVector3D(1, -1, -1),
                                          QVector3D(1, 1, -1),
                                          QVector3D(-1, 1, 1),
                                          QVector3D(-1, -1, 1),
                                          QVector3D(-1, -1, -1),
                                          QVector3D(-1, 1, -1) };

BBCoordinateCube::BBCoordinateCube()
    : BBCoordinateCube(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateCube::BBCoordinateCube(float px, float py, float pz,
                                   float rx, float ry, float rz,
                                   float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_fHalfLength = 0.08f;
}

void BBCoordinateCube::init()
{
    m_pVBO = new BBVertexBufferObject(24);

    for (int i = 0; i < 8; i++)
    {
        m_pVBO->setPosition(i, 1.0f + m_Sign[i].x() * m_fHalfLength,
                                      m_Sign[i].y() * m_fHalfLength,
                                      m_Sign[i].z() * m_fHalfLength);
        m_pVBO->setColor(i, BBConstant::m_Red);
        m_pVBO->setPosition(i + 8, m_Sign[i].x() * m_fHalfLength,
                            1.0f + m_Sign[i].y() * m_fHalfLength,
                                   m_Sign[i].z() * m_fHalfLength);
        m_pVBO->setColor(i + 8, BBConstant::m_Green);
        m_pVBO->setPosition(i + 16, m_Sign[i].x() * m_fHalfLength,
                                    m_Sign[i].y() * m_fHalfLength,
                             1.0f + m_Sign[i].z() * m_fHalfLength);
        m_pVBO->setColor(i + 16, BBConstant::m_Blue);
    }

    m_nIndexCount = 72;
    m_pIndexes = new unsigned short[m_nIndexCount];
    unsigned short indexes[] = {0, 1, 2, 3,
                                4, 5, 6, 7,
                                0, 1, 5, 4,
                                3, 7, 6, 2,
                                0, 4, 7, 3,
                                1, 5, 6, 2};
    for (int i = 0; i < 24; i++)
    {
        m_pIndexes[i] = indexes[i];
        m_pIndexes[i + 24] = indexes[i] + 8;
        m_pIndexes[i + 48] = indexes[i] + 16;
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_QUADS, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBCoordinateCube::move(const QVector3D &delta)
{
    // the size of the cube does not change
    // As scale, the position of cube changes
    for (int i = 0; i < 8; i++)
    {
        m_pVBO->setPosition(i,
                            m_Sign[i].x() * m_fHalfLength + 1.0f + delta.x(),
                            m_Sign[i].y() * m_fHalfLength,
                            m_Sign[i].z() * m_fHalfLength);
        m_pVBO->setPosition(i + 8,
                            m_Sign[i].x() * m_fHalfLength,
                            m_Sign[i].y() * m_fHalfLength + 1.0f + delta.y(),
                            m_Sign[i].z() * m_fHalfLength);
        m_pVBO->setPosition(i + 16,
                            m_Sign[i].x() * m_fHalfLength,
                            m_Sign[i].y() * m_fHalfLength,
                            m_Sign[i].z() * m_fHalfLength + 1.0f + delta.z());
    }
    m_pVBO->submitData();
}


/**
 * @brief BBCoordinateTriangleFace::BBCoordinateTriangleFace
 */
BBCoordinateTriangleFace::BBCoordinateTriangleFace()
    : BBCoordinateTriangleFace(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBCoordinateTriangleFace::BBCoordinateTriangleFace(float px, float py, float pz,
                                                   float rx, float ry, float rz,
                                                   float sx, float sy, float sz)
    : BBCoordinateComponent(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBCoordinateTriangleFace::init()
{
    m_pVBO = new BBVertexBufferObject(36);

    // GL_TRIANGLES
    m_pVBO->setPosition(0, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(1, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(2, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(3, 0.0f, 0.3f, 0.3f);
    m_pVBO->setPosition(4, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(5, 0.0f, 0.3f, 0.0f);

    m_pVBO->setPosition(6, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(7, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(8, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(9, 0.3f, 0.0f, 0.3f);
    m_pVBO->setPosition(10, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(11, 0.3f, 0.0f, 0.0f);

    m_pVBO->setPosition(12, 0.0f, 0.0f, 0.0f);
    m_pVBO->setPosition(13, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(14, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(15, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(16, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(17, 0.3f, 0.0f, 0.0f);

    // GL_LINES
    m_pVBO->setPosition(18, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(19, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(20, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(21, 0.0f, 0.3f, 0.3f);
    m_pVBO->setPosition(22, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(23, 0.0f, 0.3f, 0.3f);

    m_pVBO->setPosition(24, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(25, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(26, 0.0f, 0.0f, 0.3f);
    m_pVBO->setPosition(27, 0.3f, 0.0f, 0.3f);
    m_pVBO->setPosition(28, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(29, 0.3f, 0.0f, 0.3f);

    m_pVBO->setPosition(30, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(31, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(32, 0.0f, 0.3f, 0.0f);
    m_pVBO->setPosition(33, 0.3f, 0.3f, 0.0f);
    m_pVBO->setPosition(34, 0.3f, 0.0f, 0.0f);
    m_pVBO->setPosition(35, 0.3f, 0.3f, 0.0f);

    for (int i = 0; i < 6; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_RedTransparency);
    }
    for (int i = 6; i < 12; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_GreenTransparency);
    }
    for (int i = 12; i < 18; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_BlueTransparency);
    }
    for (int i = 18; i < 24; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Red);
    }
    for (int i = 24; i < 30; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Green);
    }
    for (int i = 30; i < 36; i++)
    {
        m_pVBO->setColor(i, BBConstant::m_Blue);
    }

    BBCoordinateComponent::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_TRIANGLES, 0, 18);
    appendDrawCall(pDrawCall);

    BBDrawCall *pDrawCall2 = new BBDrawCall;
    pDrawCall2->setMaterial(m_pCurrentMaterial);
    pDrawCall2->setVBO(m_pVBO, GL_LINES, 18, 36);
    appendDrawCall(pDrawCall2);
}
