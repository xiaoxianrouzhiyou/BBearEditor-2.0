#include "BBCoordinateSystem.h"
#include "Render/BBGLBuffers.h"
#include "Render/BBGLShader.h"
#include "BBUtils.h"
#include "Render/BBCamera.h"
#include "Geometry/BBBoundingBox.h"
#include "BBGameObjectSet.h"


//------------------------------
//  BBCoordinateComponent
//------------------------------

QVector3D BBCoordinateComponent::m_Red = QVector3D(0.937255f, 0.378431f, 0.164706f);
QVector4D BBCoordinateComponent::m_RedTransparency = QVector4D(0.937255f, 0.378431f, 0.164706f, 0.7f);
QVector3D BBCoordinateComponent::m_Green = QVector3D(0.498039f, 0.827451f, 0.25098f);
QVector4D BBCoordinateComponent::m_GreenTransparency = QVector4D(0.498039f, 0.827451f, 0.25098f, 0.7f);
QVector3D BBCoordinateComponent::m_Blue = QVector3D(0.341176f, 0.662745f, 1.0f);
QVector4D BBCoordinateComponent::m_BlueTransparency = QVector4D(0.341176f, 0.662745f, 1.0f, 0.7f);
QVector3D BBCoordinateComponent::m_Yellow = QVector3D(1.0f, 1.0f, 0.305882f);
QVector3D BBCoordinateComponent::m_Gray = QVector3D(0.8f, 0.8f, 0.8f);
QVector4D BBCoordinateComponent::m_GrayTransparency = QVector4D(0.8f, 0.8f, 0.8f, 0.7f);
//QVector3D BBCoordinateComponent::m_Red = QVector3D(0.909804f, 0.337255f, 0.333333f);
//QVector4D BBCoordinateComponent::m_RedTransparency = QVector4D(0.909804f, 0.337255f, 0.333333f, 0.5f);
//QVector3D BBCoordinateComponent::m_Green = QVector3D(0.356863f, 0.729412f, 0.619608f);
//QVector4D BBCoordinateComponent::m_GreenTransparency = QVector4D(0.356863f, 0.729412f, 0.619608f, 0.5f);
//QVector3D BBCoordinateComponent::m_Blue = QVector3D(0.384314f, 0.631373f, 0.847059f);
//QVector4D BBCoordinateComponent::m_BlueTransparency = QVector4D(0.384314f, 0.631373f, 0.847059f, 0.5f);
//QVector3D BBCoordinateComponent::m_Yellow = QVector3D(0.847059f, 0.603922f, 0.309804f);

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
    int count1 = m_pVertexBuffer->getVertexCount() / 3;
    int count2 = count1 * 2;
    int count3 = m_pVertexBuffer->getVertexCount();
    if (axis & BBAxisName::AxisX)
    {
        if (bSelected)
        {
            setVertexColor(0, count1, m_Yellow);
        }
        else
        {
            setVertexColor(0, count1, m_Red);
        }
    }
    if (axis & BBAxisName::AxisY)
    {
        if (bSelected)
        {
            setVertexColor(count1, count2, m_Yellow);
        }
        else
        {
            setVertexColor(count1, count2, m_Green);
        }
    }
    if (axis & BBAxisName::AxisZ)
    {
        if (bSelected)
        {
            setVertexColor(count2, count3, m_Yellow);
        }
        else
        {
            setVertexColor(count2, count3, m_Blue);
        }
    }
}

void BBCoordinateComponent::setVertexColor(int start, int end, const QVector3D &color)
{
    for (int i = start; i < end; i++)
    {
        m_pVertexBuffer->setColor(i, color);
    }
}

void BBCoordinateComponent::setVertexColor(int start, int end, const QVector4D &color)
{
    for (int i = start; i < end; i++)
    {
        m_pVertexBuffer->setColor(i, color);
    }
}


//------------------------------
//  BBCoordinateArrow
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(39);

    m_pVertexBuffer->setPosition(0, 1.2f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(1, 0.85f, 0.1f, 0.0f);
    m_pVertexBuffer->setPosition(2, 0.85f, 0.086603f, 0.05f);
    m_pVertexBuffer->setPosition(3, 0.85f, 0.05f, 0.086603f);
    m_pVertexBuffer->setPosition(4, 0.85f, 0.0f, 0.1f);
    m_pVertexBuffer->setPosition(5, 0.85f, -0.05f, 0.086603f);
    m_pVertexBuffer->setPosition(6, 0.85f, -0.086603f, 0.05f);
    m_pVertexBuffer->setPosition(7, 0.85f, -0.1f, 0.0f);
    m_pVertexBuffer->setPosition(8, 0.85f, -0.086603f, -0.05f);
    m_pVertexBuffer->setPosition(9, 0.85f, -0.05f, -0.086603f);
    m_pVertexBuffer->setPosition(10, 0.85f, 0.0f, -0.1f);
    m_pVertexBuffer->setPosition(11, 0.85f, 0.05f, -0.086603f);
    m_pVertexBuffer->setPosition(12, 0.85f, 0.086603f, -0.05f);
    for (int i = 0; i < 13; i++)
        m_pVertexBuffer->setColor(i, m_Red);

    m_pVertexBuffer->setPosition(13, 0.0f, 1.2f, 0.0f);
    m_pVertexBuffer->setPosition(14, 0.1f, 0.85f, 0.0f);
    m_pVertexBuffer->setPosition(15, 0.086603f, 0.85f, 0.05f);
    m_pVertexBuffer->setPosition(16, 0.05f, 0.85f, 0.086603f);
    m_pVertexBuffer->setPosition(17, 0.0f, 0.85f, 0.1f);
    m_pVertexBuffer->setPosition(18, -0.05f, 0.85f, 0.086603f);
    m_pVertexBuffer->setPosition(19, -0.086603f, 0.85f, 0.05f);
    m_pVertexBuffer->setPosition(20, -0.1f, 0.85f, 0.0f);
    m_pVertexBuffer->setPosition(21, -0.086603f, 0.85f, -0.05f);
    m_pVertexBuffer->setPosition(22, -0.05f, 0.85f, -0.086603f);
    m_pVertexBuffer->setPosition(23, 0.0f, 0.85f, -0.1f);
    m_pVertexBuffer->setPosition(24, 0.05f, 0.85f, -0.086603f);
    m_pVertexBuffer->setPosition(25, 0.086603f, 0.85f, -0.05f);
    for (int i = 13; i < 26; i++)
        m_pVertexBuffer->setColor(i, m_Green);

    m_pVertexBuffer->setPosition(26, 0.0f, 0.0f, 1.2f);
    m_pVertexBuffer->setPosition(27, 0.1f, 0.0f, 0.85f);
    m_pVertexBuffer->setPosition(28, 0.086603f, 0.05f, 0.85f);
    m_pVertexBuffer->setPosition(29, 0.05f, 0.086603f, 0.85f);
    m_pVertexBuffer->setPosition(30, 0.0f, 0.1f, 0.85f);
    m_pVertexBuffer->setPosition(31, -0.05f, 0.086603f, 0.85f);
    m_pVertexBuffer->setPosition(32, -0.086603f, 0.05f, 0.85f);
    m_pVertexBuffer->setPosition(33, -0.1f, 0.0f, 0.85f);
    m_pVertexBuffer->setPosition(34, -0.086603f, -0.05f, 0.85f);
    m_pVertexBuffer->setPosition(35, -0.05f, -0.086603f, 0.85f);
    m_pVertexBuffer->setPosition(36, 0.0f, -0.1f, 0.85f);
    m_pVertexBuffer->setPosition(37, 0.05f, -0.086603f, 0.85f);
    m_pVertexBuffer->setPosition(38, 0.086603f, -0.05f, 0.85f);
    for (int i = 26; i < 39; i++)
        m_pVertexBuffer->setColor(i, m_Blue);

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

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateArrow::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_TRIANGLES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateAxis
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(6);

    m_pVertexBuffer->setPosition(0, 1.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(1, 0.0f, 0.0f, 0.0f);

    m_pVertexBuffer->setPosition(2, 0.0f, 1.0f, 0.0f);
    m_pVertexBuffer->setPosition(3, 0.0f, 0.0f, 0.0f);

    m_pVertexBuffer->setPosition(4, 0.0f, 0.0f, 1.0f);
    m_pVertexBuffer->setPosition(5, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; i++)
    {
        m_pVertexBuffer->setColor(i, m_Red);
    }
    for (int i = 2; i < 4; i++)
    {
        m_pVertexBuffer->setColor(i, m_Green);
    }
    for (int i = 4; i < 6; i++)
    {
        m_pVertexBuffer->setColor(i, m_Blue);
    }

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag));
}

void BBCoordinateAxis::draw()
{
    glEnable(GL_DEPTH_TEST);
    // Handle all
    glDepthFunc(GL_ALWAYS);
    glLineWidth(1.5f);
    for (int i = 0; i < 3; i++)
        glDrawArrays(GL_LINES, i * 2, 2);
    // Set back to default
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateRectFace
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(24);

    m_pVertexBuffer->setPosition(0, 0.0f, 0.3f, 0.3f);
    m_pVertexBuffer->setPosition(1, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(2, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(3, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(4, 0.0f, 0.3f, 0.3f);
    m_pVertexBuffer->setPosition(5, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(6, 0.0f, 0.3f, 0.3f);
    m_pVertexBuffer->setPosition(7, 0.0f, 0.0f, 0.3f);

    m_pVertexBuffer->setPosition(8, 0.3f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(9, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(10, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(11, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(12, 0.3f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(13, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(14, 0.3f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(15, 0.0f, 0.0f, 0.3f);

    m_pVertexBuffer->setPosition(16, 0.3f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(17, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(18, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(19, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(20, 0.3f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(21, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(22, 0.3f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(23, 0.0f, 0.3f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        m_pVertexBuffer->setColor(i, m_RedTransparency);
    }
    for (int i = 4; i < 8; i++)
    {
        m_pVertexBuffer->setColor(i, m_Red);
    }
    for (int i = 8; i < 12; i++)
    {
        m_pVertexBuffer->setColor(i, m_GreenTransparency);
    }
    for (int i = 12; i < 16; i++)
    {
        m_pVertexBuffer->setColor(i, m_Green);
    }
    for (int i = 16; i < 20; i++)
    {
        m_pVertexBuffer->setColor(i, m_BlueTransparency);
    }
    for (int i = 20; i < 24; i++)
    {
        m_pVertexBuffer->setColor(i, m_Blue);
    }

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag));
}

void BBCoordinateRectFace::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glLineWidth(1.5f);
    for (int i = 0; i < 3; i++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i * 8, 4);
        glDrawArrays(GL_LINES, i * 8 + 4, 2);
        glDrawArrays(GL_LINES, i * 8 + 6, 2);
    }
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateQuarterCircle
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(78);

    for (int i = 0; i < 13; i++)
    {
        float c1 = cosf(0.1309 * i);
        float s1 = sinf(0.1309 * i);
        float c2 = 0.8f * c1;
        float s2 = 0.8f * s1;
        m_pVertexBuffer->setPosition(i, 0.0f, c1, s1);
        m_pVertexBuffer->setColor(i, m_Red);
        m_pVertexBuffer->setPosition(i + 13, 0.0f, c2, s2);
        m_pVertexBuffer->setColor(i + 13, m_Red);
        m_pVertexBuffer->setPosition(i + 26, c1, 0.0f, s1);
        m_pVertexBuffer->setColor(i + 26, m_Green);
        m_pVertexBuffer->setPosition(i + 39, c2, 0.0f, s2);
        m_pVertexBuffer->setColor(i + 39, m_Green);
        m_pVertexBuffer->setPosition(i + 52, c1, s1, 0.0f);
        m_pVertexBuffer->setColor(i + 52, m_Blue);
        m_pVertexBuffer->setPosition(i + 65, c2, s2, 0.0f);
        m_pVertexBuffer->setColor(i + 65, m_Blue);
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

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateQuarterCircle::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_QUADS, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateCircle
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(96);

    for (int i = 0; i < 48; i++)
    {
        float c1 = cosf(0.1309 * i);
        float s1 = sinf(0.1309 * i);
        float c2 = 0.8f * c1;
        float s2 = 0.8f * s1;
        // Circle
        m_pVertexBuffer->setPosition(2 * i, 0.0f, c1, s1);
        m_pVertexBuffer->setColor(2 * i, m_Yellow);
        m_pVertexBuffer->setPosition(2 * i + 1, 0.0f, c2, s2);
        m_pVertexBuffer->setColor(2 * i + 1, m_Yellow);
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

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateCircle::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_QUADS, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateTickMark
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(72);

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
        m_pVertexBuffer->setPosition(2 * i, 0.0f, c1, s1);
        m_pVertexBuffer->setColor(2 * i, m_Gray);
        m_pVertexBuffer->setPosition(2 * i + 1, 0.0f, c2, s2);
        m_pVertexBuffer->setColor(2 * i + 1, m_Gray);
    }

    m_nIndexCount = 72;
    m_pIndexes = new unsigned short[m_nIndexCount];

    for (int i = 0; i < 72; i += 2)
    {
        m_pIndexes[i] = i;
        m_pIndexes[i + 1] = i + 2;
    }
    m_pIndexes[71] = 0;

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateTickMark::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_LINES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glLineWidth(1.5f);
    for (int i = 0; i < 36; i++)
        glDrawArrays(GL_LINES, i * 2, 2);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateSector
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(361);
    m_pVertexBuffer->setPosition(360, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setColor(360, m_GrayTransparency);
    for (int i = 0; i < 360; i++)
    {
        float c = 0.8f * cosf(0.017453f * i);
        float s = 0.8f * sinf(0.017453f * i);
        // on the circle
        m_pVertexBuffer->setPosition(i, 0.0f, c, s);
        m_pVertexBuffer->setColor(i, m_Gray);
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

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateSector::render(BBCamera *pCamera)
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

    m_pShader->bindElementBufferObject(pIndexes, nIndexCount);
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

void BBCoordinateSector::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_TRIANGLES, abs(m_nAngle) * 3, GL_UNSIGNED_SHORT, 0);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateCube
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(24);

    for (int i = 0; i < 8; i++)
    {
        m_pVertexBuffer->setPosition(i, 1.0f + m_Sign[i].x() * m_fHalfLength,
                                               m_Sign[i].y() * m_fHalfLength,
                                               m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setColor(i, m_Red);
        m_pVertexBuffer->setPosition(i + 8, m_Sign[i].x() * m_fHalfLength,
                                     1.0f + m_Sign[i].y() * m_fHalfLength,
                                            m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setColor(i + 8, m_Green);
        m_pVertexBuffer->setPosition(i + 16, m_Sign[i].x() * m_fHalfLength,
                                             m_Sign[i].y() * m_fHalfLength,
                                      1.0f + m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setColor(i + 16, m_Blue);
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

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag),
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateCube::move(const QVector3D &delta)
{
    // the size of the cube does not change
    // As scale, the position of cube changes
    for (int i = 0; i < 8; i++)
    {
        m_pVertexBuffer->setPosition(i,
                                     m_Sign[i].x() * m_fHalfLength + 1.0f + delta.x(),
                                     m_Sign[i].y() * m_fHalfLength,
                                     m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setPosition(i + 8,
                                     m_Sign[i].x() * m_fHalfLength,
                                     m_Sign[i].y() * m_fHalfLength + 1.0f + delta.y(),
                                     m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setPosition(i + 16,
                                     m_Sign[i].x() * m_fHalfLength,
                                     m_Sign[i].y() * m_fHalfLength,
                                     m_Sign[i].z() * m_fHalfLength + 1.0f + delta.z());
    }
}

void BBCoordinateCube::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDrawElements(GL_QUADS, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateTriangleFace
//------------------------------

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
    m_pVertexBuffer = new BBGLVertexBuffer(36);

    m_pVertexBuffer->setPosition(0, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(1, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(2, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(3, 0.0f, 0.3f, 0.3f);
    m_pVertexBuffer->setPosition(4, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(5, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(6, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(7, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(8, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(9, 0.0f, 0.3f, 0.3f);
    m_pVertexBuffer->setPosition(10, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(11, 0.0f, 0.3f, 0.3f);

    m_pVertexBuffer->setPosition(12, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(13, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(14, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(15, 0.3f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(16, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(17, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(18, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(19, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(20, 0.0f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(21, 0.3f, 0.0f, 0.3f);
    m_pVertexBuffer->setPosition(22, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(23, 0.3f, 0.0f, 0.3f);

    m_pVertexBuffer->setPosition(24, 0.0f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(25, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(26, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(27, 0.3f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(28, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(29, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(30, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(31, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(32, 0.0f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(33, 0.3f, 0.3f, 0.0f);
    m_pVertexBuffer->setPosition(34, 0.3f, 0.0f, 0.0f);
    m_pVertexBuffer->setPosition(35, 0.3f, 0.3f, 0.0f);

    for (int i = 0; i < 6; i++)
    {
        m_pVertexBuffer->setColor(i, m_RedTransparency);
    }
    for (int i = 6; i < 12; i++)
    {
        m_pVertexBuffer->setColor(i, m_Red);
    }
    for (int i = 12; i < 18; i++)
    {
        m_pVertexBuffer->setColor(i, m_GreenTransparency);
    }
    for (int i = 18; i < 24; i++)
    {
        m_pVertexBuffer->setColor(i, m_Green);
    }
    for (int i = 24; i < 30; i++)
    {
        m_pVertexBuffer->setColor(i, m_BlueTransparency);
    }
    for (int i = 30; i < 36; i++)
    {
        m_pVertexBuffer->setColor(i, m_Blue);
    }

    m_pShader->init(BB_PATH_RESOURCE_SHADER(coordinate.vert),
                    BB_PATH_RESOURCE_SHADER(coordinate.frag));
}

void BBCoordinateTriangleFace::draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glLineWidth(1.5f);
    for (int i = 0; i < 3; i++)
    {
        glDrawArrays(GL_TRIANGLES, i * 12, 3);
        glDrawArrays(GL_TRIANGLES, i * 12 + 3, 3);
        glDrawArrays(GL_LINES, i * 12 + 6, 2);
        glDrawArrays(GL_LINES, i * 12 + 8, 2);
        glDrawArrays(GL_LINES, i * 12 + 10, 2);
    }
    glDepthFunc(GL_LEQUAL);
}


//------------------------------
//  BBCoordinateSystem
//------------------------------

BBCoordinateSystem::BBCoordinateSystem()
    : BBGameObject()
{
    m_pSelectedObject = NULL;
    m_bTransforming = false;
}

BBCoordinateSystem::~BBCoordinateSystem()
{
    BB_SAFE_DELETE(m_pSelectedObject);
}

void BBCoordinateSystem::render(BBCamera *pCamera)
{
    // The coordinate system will not become smaller as the camera is zoomed out
    // need to scale according to the distance
    float fDistance = (pCamera->getPosition() - m_Position).length();
    setScale(fDistance / 6.5f);
}

bool BBCoordinateSystem::hit(const BBRay &ray,
                             BBBoundingBox *pBoundingBox1, const BBAxisFlags &axis1,
                             BBBoundingBox *pBoundingBox2, const BBAxisFlags &axis2,
                             BBBoundingBox *pBoundingBox3, const BBAxisFlags &axis3,
                             float &fDistance)
{
    // axis1 is the coordinate axis name corresponding to pBoundingBox1
    // size of m_pCoordinateRectFace is 0.3
    float d1, d2, d3;
    bool bResult1 = pBoundingBox1->hit(ray, d1);
    bool bResult2 = pBoundingBox2->hit(ray, d2);
    bool bResult3 = pBoundingBox3->hit(ray, d3);
    if (!bResult1 && !bResult2 && !bResult3)
    {
        setSelectedAxis(BBAxisName::AxisNULL);
        return false;
    }

    if (d1 < d2)
    {
        if (d1 < d3)
        {
            // yoz is nearest
            setSelectedAxis(axis1);
            fDistance = d1;
        }
        else
        {
            // xoy is nearest
            setSelectedAxis(axis3);
            fDistance = d3;
        }
    }
    else
    {
        if (d1 < d3)
        {
            // xoz is nearest
            setSelectedAxis(axis2);
            fDistance = d2;
        }
        else
        {
            if (d2 < d3)
            {
                // xoz is nearest
                setSelectedAxis(axis2);
                fDistance = d2;
            }
            else
            {
                // xoy is nearest
                setSelectedAxis(axis3);
                fDistance = d3;
            }
        }
    }

    return true;
}

void BBCoordinateSystem::setSelectedObject(BBGameObject *pObject)
{
    m_pSelectedObject = pObject;
    if (pObject != NULL)
    {
        setPosition(pObject->getPosition());
        setRotation(pObject->getRotation());
    }
    else
    {
        setPosition(QVector3D(0, 0, 0));
        setRotation(QVector3D(0, 0, 0));
        setSelectedAxis(BBAxisName::AxisNULL);
    }
}

void BBCoordinateSystem::stopTransform()
{
    // When no movement, reset mouse pos
    QVector3D pos;
    m_LastMousePos = pos;
    m_bTransforming = false;
}


//------------------------------
//  BBPositionCoordinateSystem
//------------------------------

BBPositionCoordinateSystem::BBPositionCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateArrow = new BBCoordinateArrow();
    m_pCoordinateAxis = new BBCoordinateAxis();
    m_pCoordinateRectFace = new BBCoordinateRectFace();
    m_pBoundingBoxX = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.75f, 0.0f, 0.0f, 0.45f, 0.1f, 0.1f);
    m_pBoundingBoxY = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.75f, 0.0f, 0.1f, 0.45f, 0.1f);
    m_pBoundingBoxZ = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.0f, 0.75f, 0.1f, 0.1f, 0.45f);
    m_pBoundingBoxYOZ = new BBRectBoundingBox2D(0.0f, 0.15f, 0.15f, 0.0f, 0.15f, 0.15f);
    m_pBoundingBoxXOZ = new BBRectBoundingBox2D(0.15f, 0.0f, 0.15f, 0.15f, 0.0f, 0.15f);
    m_pBoundingBoxXOY = new BBRectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
}

BBPositionCoordinateSystem::~BBPositionCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateArrow);
    BB_SAFE_DELETE(m_pCoordinateAxis);
    BB_SAFE_DELETE(m_pCoordinateRectFace);
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxZ);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
}

void BBPositionCoordinateSystem::init()
{
    m_pCoordinateArrow->init();
    m_pCoordinateAxis->init();
    m_pCoordinateRectFace->init();
}

void BBPositionCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);
    m_pCoordinateArrow->render(pCamera);
    m_pCoordinateAxis->render(pCamera);
    m_pCoordinateRectFace->render(pCamera);
}

void BBPositionCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pCoordinateArrow->resize(fWidth, fHeight);
    m_pCoordinateAxis->resize(fWidth, fHeight);
    m_pCoordinateRectFace->resize(fWidth, fHeight);
}

void BBPositionCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    // Modifying m_Position directly will cause an error
    // The position of the bounding box needs to be updated at the same time
    // otherwise, will trigger incorrect mouse events
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateArrow->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateAxis->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateRectFace->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxX->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
}

void BBPositionCoordinateSystem::setScale(float scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);
    m_pCoordinateArrow->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateAxis->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateRectFace->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
}

void BBPositionCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    // change color of indicator
    m_pCoordinateArrow->setSelectedAxis(axis);
    m_pCoordinateAxis->setSelectedAxis(axis);

    m_SelectedAxis = axis;
}

bool BBPositionCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // if hitting m_pCoordinateRectFace, no need to handle axis
        float fDistance;
        if (!hit(ray,
                 m_pBoundingBoxYOZ, BBAxisName::AxisY | BBAxisName::AxisZ,
                 m_pBoundingBoxXOZ, BBAxisName::AxisX | BBAxisName::AxisZ,
                 m_pBoundingBoxXOY, BBAxisName::AxisX | BBAxisName::AxisY,
                 fDistance))
        {
            // handle axis
            BB_END(!hit(ray,
                        m_pBoundingBoxX, BBAxisName::AxisX,
                        m_pBoundingBoxY, BBAxisName::AxisY,
                        m_pBoundingBoxZ, BBAxisName::AxisZ,
                        fDistance));
        }

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBPositionCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        if ((m_SelectedAxis == BBAxisName::AxisY)
         || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY)))
        {
            mousePos = ray.computeIntersectWithXOYPlane(m_Position.z());
        }
        else if ((m_SelectedAxis == BBAxisName::AxisX
               || m_SelectedAxis == BBAxisName::AxisZ)
              || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ)))
        {
            mousePos = ray.computeIntersectWithXOZPlane(m_Position.y());
        }
        else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            mousePos = ray.computeIntersectWithYOZPlane(m_Position.x());
        }

        // displacement can be computed
        if (!m_LastMousePos.isNull())
        {
            QVector3D mouseDisplacement = mousePos - m_LastMousePos;
            if (m_SelectedAxis & BBAxisName::AxisX)
            {
                // The length of the projection of the mouse's displacement on the axis
                float d = mouseDisplacement.x();
                setPosition(m_Position + QVector3D(d, 0, 0));
            }
            if (m_SelectedAxis & BBAxisName::AxisY)
            {
                float d = mouseDisplacement.y();
                setPosition(m_Position + QVector3D(0, d, 0));
            }
            if (m_SelectedAxis & BBAxisName::AxisZ)
            {
                float d = mouseDisplacement.z();
                setPosition(m_Position + QVector3D(0, 0, d));
            }
            m_pSelectedObject->setPosition(m_Position);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
    }
}


//------------------------------
//  BBRotationCoordinateSystem
//------------------------------

BBRotationCoordinateSystem::BBRotationCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateQuarterCircle = new BBCoordinateQuarterCircle();
    m_pBoundingBoxYOZ = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::YOZ);
    m_pBoundingBoxXOZ = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::XOZ);
    m_pBoundingBoxXOY = new BBQuarterCircleBoundingBox2D(0, 0, 0, 1, BBPlaneName::XOY);
    m_pCoordinateCircle = new BBCoordinateCircle();
    m_pCoordinateTickMark = new BBCoordinateTickMark();
    m_pCoordinateSector = new BBCoordinateSector();
}

BBRotationCoordinateSystem::~BBRotationCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateQuarterCircle);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
    BB_SAFE_DELETE(m_pCoordinateCircle);
    BB_SAFE_DELETE(m_pCoordinateTickMark);
    BB_SAFE_DELETE(m_pCoordinateSector);
}

void BBRotationCoordinateSystem::init()
{
    m_pCoordinateQuarterCircle->init();
    m_pCoordinateCircle->init();
    m_pCoordinateTickMark->init();
    m_pCoordinateSector->init();
}

void BBRotationCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);

    // When transforming, render circle. otherwise, render coordinate axis
    if (m_bTransforming)
    {
        m_pCoordinateCircle->render(pCamera);
        m_pCoordinateTickMark->render(pCamera);
        m_pCoordinateSector->render(pCamera);
    }
    else
    {
        // coordinate axis always faces camera
        QVector3D dir = pCamera->getPosition() - m_Position;
        dir.setX(dir.x() >= 0 ? 1 : -1);
        dir.setY(dir.y() >= 0 ? 1 : -1);
        dir.setZ(dir.z() >= 0 ? 1 : -1);
        m_pCoordinateQuarterCircle->setScale(m_pCoordinateQuarterCircle->getScale() * dir);
        m_pCoordinateQuarterCircle->render(pCamera);
        // bounding box also needs mirror transform
        m_pBoundingBoxYOZ->setQuadrantFlag(dir);
        m_pBoundingBoxXOZ->setQuadrantFlag(dir);
        m_pBoundingBoxXOY->setQuadrantFlag(dir);
    }
}

void BBRotationCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pCoordinateQuarterCircle->resize(fWidth, fHeight);
    m_pCoordinateCircle->resize(fWidth, fHeight);
    m_pCoordinateTickMark->resize(fWidth, fHeight);
    m_pCoordinateSector->resize(fWidth, fHeight);
}

void BBRotationCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateQuarterCircle->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateCircle->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateTickMark->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateSector->setPosition(position, bUpdateLocalTransform);
}

void BBRotationCoordinateSystem::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);
    m_pCoordinateQuarterCircle->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateCircle->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateTickMark->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateSector->setScale(scale, bUpdateLocalTransform);
}

void BBRotationCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    m_pCoordinateQuarterCircle->setSelectedAxis(axis);

    // m_pCoordinateCircle is based on face YOZ and needs to rotate
    QVector3D rot;
    if (m_SelectedAxis == BBAxisName::AxisY)
    {

        rot = QVector3D(0, -90, 90);
    }
    else if (m_SelectedAxis == BBAxisName::AxisZ)
    {
        rot = QVector3D(0, -90, 0);
    }
    m_pCoordinateCircle->setRotation(rot);
    m_pCoordinateTickMark->setRotation(rot);
    m_pCoordinateSector->setRotation(rot);

    m_SelectedAxis = axis;
}

bool BBRotationCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // hit face YOZ, rotate around AxisX
        float fDistance;
        BB_END(!hit(ray,
                    m_pBoundingBoxYOZ, BBAxisName::AxisX,
                    m_pBoundingBoxXOZ, BBAxisName::AxisY,
                    m_pBoundingBoxXOY, BBAxisName::AxisZ,
                    fDistance));

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBRotationCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        QVector3D rotAxis;

        if (m_SelectedAxis == BBAxisName::AxisX)
        {
            mousePos = ray.computeIntersectWithYOZPlane(m_Position.x());
            rotAxis = QVector3D(1, 0, 0);
        }
        else if (m_SelectedAxis == BBAxisName::AxisY)
        {
            mousePos = ray.computeIntersectWithXOZPlane(m_Position.y());
            rotAxis = QVector3D(0, 1, 0);
        }
        else if (m_SelectedAxis == BBAxisName::AxisZ)
        {
            mousePos = ray.computeIntersectWithXOYPlane(m_Position.z());
            rotAxis = QVector3D(0, 0, 1);
        }
        else
        {
            return;
        }

        // whether displacement can be computed
        if (m_LastMousePos.isNull())
        {
            // init
            m_pCoordinateSector->reset();
        }
        else
        {
            // compute the intersection angle between two of mouse pos dir
            QVector3D mousePosDir = (mousePos - m_Position).normalized();
            float c = QVector3D::dotProduct(m_LastMousePosDir, mousePosDir);
            int nDeltaAngle = round(acosf(c) / 3.141593f * 180);
            // compute the sign of angle
            QVector3D crossResult = QVector3D::crossProduct(m_LastMousePosDir, mousePosDir);
            float sign = crossResult.x() + crossResult.y() + crossResult.z();
            sign = sign > 0 ? 1 : -1;
            nDeltaAngle = sign * nDeltaAngle;

            m_pCoordinateSector->setAngle(nDeltaAngle);

            m_pSelectedObject->setRotation(nDeltaAngle, rotAxis);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
        m_LastMousePosDir = (m_LastMousePos - m_Position).normalized();
    }
}


//------------------------------
//  BBScaleCoordinateSystem
//------------------------------

BBScaleCoordinateSystem::BBScaleCoordinateSystem()
    : BBCoordinateSystem()
{
    m_pCoordinateCube = new BBCoordinateCube();
    m_pCoordinateAxis = new BBCoordinateAxis();
    m_pCoordinateTriangleFace = new BBCoordinateTriangleFace();
    m_pBoundingBoxX = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.69f, 0.0f, 0.0f, 0.39f, 0.08f, 0.08f);
    m_pBoundingBoxY = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.69f, 0.0f, 0.08f, 0.39f, 0.08f);
    m_pBoundingBoxZ = new BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.0f, 0.69f, 0.08f, 0.08f, 0.39f);
    m_pBoundingBoxYOZ = new BBRectBoundingBox2D(0.0f, 0.15f, 0.15f, 0.0f, 0.15f, 0.15f);
    m_pBoundingBoxXOZ = new BBRectBoundingBox2D(0.15f, 0.0f, 0.15f, 0.15f, 0.0f, 0.15f);
    m_pBoundingBoxXOY = new BBRectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
    m_pBoundingBoxXYZ = new BBTriangleBoundingBox2D(QVector3D(0.3f, 0.0f, 0.0f),
                                                    QVector3D(0.0f, 0.3f, 0.0f),
                                                    QVector3D(0.0f, 0.0f, 0.3f));
}

BBScaleCoordinateSystem::~BBScaleCoordinateSystem()
{
    BB_SAFE_DELETE(m_pCoordinateCube);
    BB_SAFE_DELETE(m_pCoordinateAxis);
    BB_SAFE_DELETE(m_pCoordinateTriangleFace);
    BB_SAFE_DELETE(m_pBoundingBoxX);
    BB_SAFE_DELETE(m_pBoundingBoxY);
    BB_SAFE_DELETE(m_pBoundingBoxZ);
    BB_SAFE_DELETE(m_pBoundingBoxYOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOZ);
    BB_SAFE_DELETE(m_pBoundingBoxXOY);
    BB_SAFE_DELETE(m_pBoundingBoxXYZ);
}

void BBScaleCoordinateSystem::init()
{
    m_pCoordinateCube->init();
    m_pCoordinateAxis->init();
    m_pCoordinateTriangleFace->init();
}

void BBScaleCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    BBCoordinateSystem::render(pCamera);
    m_pCoordinateCube->render(pCamera);
    m_pCoordinateAxis->render(pCamera);
    m_pCoordinateTriangleFace->render(pCamera);
}

void BBScaleCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pCoordinateCube->resize(fWidth, fHeight);
    m_pCoordinateAxis->resize(fWidth, fHeight);
    m_pCoordinateTriangleFace->resize(fWidth, fHeight);
}

void BBScaleCoordinateSystem::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setPosition(position, bUpdateLocalTransform);
    m_pCoordinateCube->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateAxis->setPosition(position, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxX->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setPosition(position, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateCube->setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateAxis->setRotation(rotation, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxX->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxY->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setRotation(rotation, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBCoordinateSystem::setScale(scale, bUpdateLocalTransform);

    // when transforming, cube and axis will change as scale operation
    if (m_bTransforming)
    {
        QVector3D delta = m_pSelectedObject->getScale() - m_SelectedObjectOriginalScale;
        m_pCoordinateCube->move(delta);
        // the normalized scale is not 0 but 1
        m_pCoordinateAxis->setScale(scale * (delta + QVector3D(1, 1, 1)), bUpdateLocalTransform);
    }
    else
    {
        // after stopTransform, go back
        m_pCoordinateCube->move(QVector3D(0, 0, 0));
        m_pCoordinateAxis->setScale(scale, bUpdateLocalTransform);
    }
    m_pCoordinateCube->setScale(scale, bUpdateLocalTransform);
    m_pCoordinateTriangleFace->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxX->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxYOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOZ->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXOY->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBoxXYZ->setScale(scale, bUpdateLocalTransform);
}

void BBScaleCoordinateSystem::setSelectedAxis(const BBAxisFlags &axis)
{
    // change color of indicator
    m_pCoordinateCube->setSelectedAxis(axis);
    m_pCoordinateAxis->setSelectedAxis(axis);

    m_SelectedAxis = axis;
}

bool BBScaleCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    do {
        // if transforming, there is no need to perform other operations
        BB_END(m_bTransforming);
        // otherwise, determine whether transform can be turned on

        BB_END(m_pSelectedObject == NULL);

        // handle collision detection, change color of related axis and get m_SelectedAxis
        // if hitting m_pCoordinateRectFace, no need to handle axis
        float fDistance;
        if (!hit(ray,
                 m_pBoundingBoxYOZ, BBAxisName::AxisY | BBAxisName::AxisZ,
                 m_pBoundingBoxXOZ, BBAxisName::AxisX | BBAxisName::AxisZ,
                 m_pBoundingBoxXOY, BBAxisName::AxisX | BBAxisName::AxisY,
                 fDistance))
        {
            // handle axis
            BB_END(!hit(ray,
                        m_pBoundingBoxX, BBAxisName::AxisX,
                        m_pBoundingBoxY, BBAxisName::AxisY,
                        m_pBoundingBoxZ, BBAxisName::AxisZ,
                        fDistance));
        }
        else
        {
            // Two axes are selected, determine whether the third axis is also selected
            // and perform collision detection on the triangle in the middle
            float d;
            if (m_pBoundingBoxXYZ->hit(ray, d))
            {
                if (d <= fDistance)
                {
                    // The middle triangle collision point is closer, select the three axes
                    setSelectedAxis(BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ);
                }
            }
        }

        // do not handle transform when mouse is not pressed
        BB_END(!bMousePressed);

        // meet the conditions, turn on m_bTransforming
        m_bTransforming = true;
    } while(0);

    transform(ray);

    // The return value indicates whether the transform has really performed
    return m_bTransforming;
}

void BBScaleCoordinateSystem::transform(const BBRay &ray)
{
    if (m_bTransforming)
    {
        // perform transform operation
        // compute now mouse pos
        QVector3D mousePos;
        QMatrix4x4 rotMatrix;
        rotMatrix.rotate(m_Quaternion);
        if ((m_SelectedAxis == BBAxisName::AxisY)
         || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY)))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(0, 0, 1), mousePos);
        }
        else if ((m_SelectedAxis == BBAxisName::AxisX || m_SelectedAxis == BBAxisName::AxisZ)
              || (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ)))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(0, 1, 0), mousePos);
        }
        else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(1, 0, 0), mousePos);
        }
        else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ))
        {
            ray.computeIntersectWithPlane(m_Position, rotMatrix * QVector3D(1, 1, 1), mousePos);
        }

        // whether displacement can be computed
        if (m_LastMousePos.isNull())
        {
            // init for the scale of cube and axis
            m_SelectedObjectOriginalScale = m_pSelectedObject->getScale();
        }
        else
        {
            QVector3D mouseDisplacement = mousePos - m_LastMousePos;
            QVector3D scale = m_pSelectedObject->getScale();
            QVector3D dir;
            if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY | BBAxisName::AxisZ))
            {
                dir = QVector3D(1, -1, 1).normalized();
                scale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * QVector3D(1, 1, 1).normalized();
            }
            else
            {
                if (m_SelectedAxis == BBAxisName::AxisX)
                {
                    // The length of the projection of the mouse's displacement on the axis
                    dir = QVector3D(1, 0, 0).normalized();
                }
                else if (m_SelectedAxis == BBAxisName::AxisY)
                {
                    dir = QVector3D(0, 1, 0).normalized();
                }
                else if (m_SelectedAxis == BBAxisName::AxisZ)
                {
                    dir = QVector3D(0, 0, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisY | BBAxisName::AxisZ))
                {
                    dir = QVector3D(0, 1, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisZ))
                {
                    dir = QVector3D(1, 0, 1).normalized();
                }
                else if (m_SelectedAxis == (BBAxisName::AxisX | BBAxisName::AxisY))
                {
                    dir = QVector3D(1, 1, 0).normalized();
                }
                scale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
            }
            m_pSelectedObject->setScale(scale);
        }

        // record and wait the next frame
        m_LastMousePos = mousePos;
    }
}


//------------------------------
//  BBTransformCoordinateSystem
//------------------------------

BBTransformCoordinateSystem::BBTransformCoordinateSystem()
    : BBGameObject()
{
    m_pPositionCoordinateSystem = new BBPositionCoordinateSystem();
    m_pRotationCoordinateSystem = new BBRotationCoordinateSystem();
    m_pScaleCoordinateSystem = new BBScaleCoordinateSystem();
    // default is m_pPositionCoordinateSystem
    m_ModeKey = m_PositionCoordinateSystemModeKey;
    m_pSelectedObject = nullptr;
    m_bTransforming = false;
}

BBTransformCoordinateSystem::~BBTransformCoordinateSystem()
{
    BB_SAFE_DELETE(m_pPositionCoordinateSystem);
    BB_SAFE_DELETE(m_pRotationCoordinateSystem);
    BB_SAFE_DELETE(m_pScaleCoordinateSystem);
    BB_SAFE_DELETE(m_pSelectedObject);
}

void BBTransformCoordinateSystem::init()
{
    m_pPositionCoordinateSystem->init();
    m_pRotationCoordinateSystem->init();
    m_pScaleCoordinateSystem->init();
}

void BBTransformCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    // transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_pPositionCoordinateSystem->render(pCamera);
    m_pRotationCoordinateSystem->render(pCamera);
    m_pScaleCoordinateSystem->render(pCamera);
    glDisable(GL_BLEND);
}

void BBTransformCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pPositionCoordinateSystem->resize(fWidth, fHeight);
    m_pRotationCoordinateSystem->resize(fWidth, fHeight);
    m_pScaleCoordinateSystem->resize(fWidth, fHeight);
}

void BBTransformCoordinateSystem::setSelectedObject(BBGameObject *pObject)
{
    // The selected operation object has not changed, no additional operation is required
    if (m_pSelectedObject == pObject)
    {
        return;
    }
    // Operation object changed

    // Clear multiple options
    for (int i = 0; i < m_SelectedObjects.count(); i++)
    {
        m_SelectedObjects.at(i)->setVisibility(false);
    }
    m_SelectedObjects.clear();

    // single selection
    if (m_pSelectedObject != nullptr)
    {
        // The bounding box and indicator of the original operation object are hidden
        m_pSelectedObject->setVisibility(false);
    }
    if (pObject != nullptr)
    {
        // show bounding box and indicator of now operation object
        pObject->setVisibility(true);
    }
    m_pSelectedObject = pObject;
    setCoordinateSystem(m_ModeKey);
}

void BBTransformCoordinateSystem::setSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet)
{
    // The operation object has not changed, no additional operations are required
    if (m_SelectedObjects == gameObjects)
        return;

    int count = m_SelectedObjects.count();
    for (int i = 0; i < count; i++)
    {
        m_SelectedObjects.at(i)->setVisibility(false);
    }
    count = gameObjects.count();
    for (int i = 0; i < count; i++)
    {
        gameObjects.at(i)->setVisibility(true);
    }
    m_SelectedObjects = gameObjects;
    // select center of gravity
    m_pSelectedObject = pSet;
    setCoordinateSystem(m_ModeKey);
}

bool BBTransformCoordinateSystem::mouseMoveEvent(const BBRay &ray, bool bMousePressed)
{
    m_bTransforming = false;
    if (m_ModeKey == m_PositionCoordinateSystemModeKey)
    {
        m_bTransforming = m_pPositionCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
    }
    else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
    {
        m_bTransforming = m_pRotationCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
    }
    else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
    {
        m_bTransforming = m_pScaleCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
    }
    // if false, other mouse events will be processed
    return m_bTransforming;
}

void BBTransformCoordinateSystem::setCoordinateSystem(char key)
{
    m_ModeKey = key;

    if (m_ModeKey == m_PositionCoordinateSystemModeKey)
    {
        m_pPositionCoordinateSystem->setSelectedObject(m_pSelectedObject);
        m_pRotationCoordinateSystem->setSelectedObject(nullptr);
        m_pScaleCoordinateSystem->setSelectedObject(nullptr);
    }
    else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
    {
        m_pPositionCoordinateSystem->setSelectedObject(nullptr);
        m_pRotationCoordinateSystem->setSelectedObject(m_pSelectedObject);
        m_pScaleCoordinateSystem->setSelectedObject(nullptr);
    }
    else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
    {
        m_pPositionCoordinateSystem->setSelectedObject(nullptr);
        m_pRotationCoordinateSystem->setSelectedObject(nullptr);
        m_pScaleCoordinateSystem->setSelectedObject(m_pSelectedObject);
    }
}

void BBTransformCoordinateSystem::stopTransform()
{
    m_pPositionCoordinateSystem->stopTransform();
    m_pRotationCoordinateSystem->stopTransform();
    m_pScaleCoordinateSystem->stopTransform();

    m_bTransforming = false;
}

void BBTransformCoordinateSystem::update()
{
    setCoordinateSystem(m_ModeKey);
}
