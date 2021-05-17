#include "BBCoordinateSystem.h"
#include "BBGLBuffers.h"
#include "BBGLShader.h"
#include "BBUtils.h"
#include "BBCamera.h"
#include "BBBoundingBox.h"


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

BBCoordinateComponent::BBCoordinateComponent(const float px, const float py, const float pz,
                                             const float rx, const float ry, const float rz,
                                             const float sx, const float sy, const float sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{


    m_SelectedAxis = BBAxisName::AxisNULL;
}

void BBCoordinateComponent::setSelectedAxis(BBAxisFlags axis)
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

void BBCoordinateComponent::setVertexColor(BBAxisFlags axis, bool bSelected)
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

void BBCoordinateComponent::setVertexColor(int start, int end, QVector3D color)
{
    for (int i = start; i < end; i++)
    {
        m_pVertexBuffer->setColor(i, color);
    }
}

void BBCoordinateComponent::setVertexColor(int start, int end, QVector4D color)
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

BBCoordinateArrow::BBCoordinateArrow(const float px, const float py, const float pz,
                                     const float rx, const float ry, const float rz,
                                     const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag",
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

BBCoordinateAxis::BBCoordinateAxis(const float px, const float py, const float pz,
                                   const float rx, const float ry, const float rz,
                                   const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag");
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

BBCoordinateRectFace::BBCoordinateRectFace(const float px, const float py, const float pz,
                                           const float rx, const float ry, const float rz,
                                           const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag");
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

BBCoordinateQuarterCircle::BBCoordinateQuarterCircle(const float px, const float py, const float pz,
                                                     const float rx, const float ry, const float rz,
                                                     const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag",
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

BBCoordinateCircle::BBCoordinateCircle(const float px, const float py, const float pz,
                                       const float rx, const float ry, const float rz,
                                       const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag",
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

BBCoordinateTickMark::BBCoordinateTickMark(const float px, const float py, const float pz,
                                           const float rx, const float ry, const float rz,
                                           const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag",
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

BBCoordinateCube::BBCoordinateCube(const float px, const float py, const float pz,
                                   const float rx, const float ry, const float rz,
                                   const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag",
                    m_pIndexes,
                    m_nIndexCount);
}

void BBCoordinateCube::changePositionAfterScale(const float x, const float y, const float z)
{
    // Only scale the axis, the size of the cube does not change
    for (int i = 0; i < 8; i++)
    {
        m_pVertexBuffer->setPosition(i, 1.0f * x + m_Sign[i].x() * m_fHalfLength,
                                                   m_Sign[i].y() * m_fHalfLength,
                                                   m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setPosition(i + 8, m_Sign[i].x() * m_fHalfLength,
                                 1.0f * y + m_Sign[i].y() * m_fHalfLength,
                                            m_Sign[i].z() * m_fHalfLength);
        m_pVertexBuffer->setPosition(i + 16, m_Sign[i].x() * m_fHalfLength,
                                             m_Sign[i].y() * m_fHalfLength,
                                  1.0f * z + m_Sign[i].z() * m_fHalfLength);
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

BBCoordinateTriangleFace::BBCoordinateTriangleFace(const float px, const float py, const float pz,
                                                   const float rx, const float ry, const float rz,
                                                   const float sx, const float sy, const float sz)
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "coordinate.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "coordinate.frag");
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
}

BBCoordinateSystem::~BBCoordinateSystem()
{
    BB_SAFE_DELETE(m_pSelectedObject);
}

QMatrix4x4 BBCoordinateSystem::getRenderModelMatrix(const QVector3D cameraPos)
{
    // The coordinate system cannot be rendered according to its own scale
    // otherwise, the coordinate system will be very small when the camera is far away from it
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_Position);
    float fDistance = (cameraPos - m_Position).length();
    modelMatrix.scale(fDistance / 6.5f);
    return modelMatrix;
}

bool BBCoordinateSystem::hit(BBRay ray,
                             BBBoundingBox *pBoundingBox1, BBAxisFlags axis1,
                             BBBoundingBox *pBoundingBox2, BBAxisFlags axis2,
                             BBBoundingBox *pBoundingBox3, BBAxisFlags axis3,
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
    if (pObject != nullptr)
    {
        m_Position = pObject->getPosition();
        m_Rotation = pObject->getRotation();
    }
    else
    {
        m_Position = QVector3D(0, 0, 0);
        m_Rotation = QVector3D(0, 0, 0);
    }
}

void BBCoordinateSystem::resetLastMousePos()
{
    // When no movement
    QVector3D pos;
    m_LastMousePos = pos;
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
    if (m_pSelectedObject == nullptr)
        return;

    QMatrix4x4 modelMatrix = getRenderModelMatrix(pCamera->getPosition());

    m_pCoordinateArrow->render(modelMatrix, pCamera);
    m_pCoordinateAxis->render(modelMatrix, pCamera);
    m_pCoordinateRectFace->render(modelMatrix, pCamera);
}

void BBPositionCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pCoordinateArrow->resize(fWidth, fHeight);
    m_pCoordinateAxis->resize(fWidth, fHeight);
    m_pCoordinateRectFace->resize(fWidth, fHeight);
}

void BBPositionCoordinateSystem::setSelectedAxis(BBAxisFlags axis)
{
    // change color of indicator
    m_pCoordinateArrow->setSelectedAxis(axis);
    m_pCoordinateAxis->setSelectedAxis(axis);

    m_SelectedAxis = axis;
}

bool BBPositionCoordinateSystem::mouseMoveEvent(BBRay ray)
{    
    if (m_pSelectedObject == nullptr)
        return false;

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
        if (!hit(ray,
                 m_pBoundingBoxX, BBAxisName::AxisX,
                 m_pBoundingBoxY, BBAxisName::AxisY,
                 m_pBoundingBoxZ, BBAxisName::AxisZ,
                 fDistance))
        {
            return false;
        }
    }

    QVector3D mousePos;
    if (m_SelectedAxis == BBAxisName::AxisNULL)
    {
        return false;
    }
    else if ((m_SelectedAxis == BBAxisName::AxisY)
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
    else
    {
        return false;
    }

    // Just start to move, no need to deal with, and no displacement can be calculated
    if (m_LastMousePos.isNull())
    {
        m_LastMousePos = mousePos;
        return false;
    }

    QVector3D mouseDisplacement = mousePos - m_LastMousePos;
    if (m_SelectedAxis & BBAxisName::AxisX)
    {
        // The length of the projection of the mouse's displacement on the axis
        float d = mouseDisplacement.x();
        m_Position = m_Position + QVector3D(d, 0, 0);
    }
    if (m_SelectedAxis & BBAxisName::AxisY)
    {
        float d = mouseDisplacement.y();
        m_Position = m_Position + QVector3D(0, d, 0);
    }
    if (m_SelectedAxis & BBAxisName::AxisZ)
    {
        float d = mouseDisplacement.z();
        m_Position = m_Position + QVector3D(0, 0, d);
    }

    m_pSelectedObject->setPosition(m_Position);
    // Update, used to calculate the next displacement
    m_LastMousePos = mousePos;
    // The return value indicates whether the transform has really performed
    return true;
}


//------------------------------
//  BBRotationCoordinateSystem
//------------------------------

BBRotationCoordinateSystem::BBRotationCoordinateSystem()
    : BBCoordinateSystem()
{

}

BBRotationCoordinateSystem::~BBRotationCoordinateSystem()
{

}

void BBRotationCoordinateSystem::init()
{

}

void BBRotationCoordinateSystem::render(BBCamera *pCamera)
{

}

void BBRotationCoordinateSystem::resize(float fWidth, float fHeight)
{

}

void BBRotationCoordinateSystem::setSelectedAxis(BBAxisFlags axis)
{

}

bool BBRotationCoordinateSystem::mouseMoveEvent(BBRay ray)
{

}


//------------------------------
//  BBScaleCoordinateSystem
//------------------------------

BBScaleCoordinateSystem::BBScaleCoordinateSystem()
    : BBCoordinateSystem()
{

}

BBScaleCoordinateSystem::~BBScaleCoordinateSystem()
{

}

void BBScaleCoordinateSystem::init()
{

}

void BBScaleCoordinateSystem::render(BBCamera *pCamera)
{

}

void BBScaleCoordinateSystem::resize(float fWidth, float fHeight)
{

}

void BBScaleCoordinateSystem::setSelectedAxis(BBAxisFlags axis)
{

}

bool BBScaleCoordinateSystem::mouseMoveEvent(BBRay ray)
{

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

bool BBTransformCoordinateSystem::mouseMoveEvent(const BBRay ray)
{
    m_bTransforming = false;
    if (m_ModeKey == m_PositionCoordinateSystemModeKey)
    {
        m_bTransforming = m_pPositionCoordinateSystem->mouseMoveEvent(ray);
    }
    else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
    {
        m_bTransforming = m_pRotationCoordinateSystem->mouseMoveEvent(ray);
    }
    else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
    {
        m_bTransforming = m_pScaleCoordinateSystem->mouseMoveEvent(ray);
    }
    // if false, other mouse events will be processed
    return m_bTransforming;
}

void BBTransformCoordinateSystem::setCoordinateSystemMode(const char key)
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

void BBTransformCoordinateSystem::setSelectedObject(BBGameObject *pObject)
{
    // The selected operation object has not changed, no additional operation is required
    if (m_pSelectedObject == pObject)
    {
        return;
    }
    // Operation object changed

//    //清空多选项 包围盒 指示器等隐藏
//    int count = mSelectedObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        mSelectedObjects.at(i)->setVisible(false);
//    }
//    mSelectedObjects.clear();

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
    setCoordinateSystemMode(m_ModeKey);
}

void BBTransformCoordinateSystem::stopTransform()
{
    m_pPositionCoordinateSystem->resetLastMousePos();
//    mRotationCoordinate->resetLastMousePos();
//    mRotationCoordinate->stopRotate();
//    mScaleCoordinate->resetLastMousePos();
//    mScaleCoordinate->stopScale();

    m_bTransforming = false;
}



///******************************************************************
//        Sector
//  *********************************************************************/

//Sector::Sector()
//    : RenderableCoordinate()
//{

//}

//Sector::Sector(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
//    : RenderableCoordinate(px, py, pz, rx, ry, rz, sx, sy, sz)
//{

//}

//void Sector::init()
//{
//    mVertexBuffer = new VertexBuffer();
//    //最小可旋转1度 2pi/360
//    //+1存中心点
//    mVertexBuffer->setSize(361);
//    mVertexBuffer->setPosition(360, 0.0f, 0.0f, 0.0f);
//    mVertexBuffer->setColor(360, mGrayTransparency);
//    for (int i = 0; i < 360; i++)
//    {
//        float c = 0.8f * cosf(0.017453f * i);
//        float s = 0.8f * sinf(0.017453f * i);
//        //圆环
//        mVertexBuffer->setPosition(i, 0.0f, c, s);
//        mVertexBuffer->setColor(i, mGray);
//    }

//    mIndexCount = 1080;
//    mIndexes = new unsigned short[mIndexCount];
//    for (int i = 0; i < 360; i++)
//    {
//        mIndexes[3 * i] = 360;
//        mIndexes[3 * i + 1] = i;
//        mIndexes[3 * i + 2] = i + 1;
//    }
//    mIndexes[1079] = 0;

//    mShader.init("../../../../BBearEngine/resources/shaders/coordinate.vert",
//                 "../../../../BBearEngine/resources/shaders/coordinate.frag", mIndexes, mIndexCount);

//}

//void Sector::render(QMatrix4x4 modelMatrix, Camera camera)
//{
//    //1度为一个单位 1个三角形 3个索引值
//    int indexCount = abs(mAngle) * 3;
//    unsigned short *indexes = new unsigned short[indexCount];

//    //正角 左半圆显示扇形角度
//    if (mAngle > 0)
//    {
//        //前部分索引
//        for (int i = 0; i < indexCount; i++)
//        {
//            indexes[i] = mIndexes[i];
//        }
//    }
//    //负角 右半圆显示扇形角度
//    else
//    {
//        //后部分索引
//        for (int i = 0; i < indexCount; i++)
//        {
//            indexes[i] = mIndexes[mIndexCount - i - 1];
//        }
//    }

//    mShader.bingEBO(indexes, indexCount);
//    RenderableObject::render(modelMatrix, camera);
//}

//void Sector::draw()
//{
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_ALWAYS);
//    glDrawElements(GL_TRIANGLES, abs(mAngle) * 3, GL_UNSIGNED_SHORT, 0);
//    glDepthFunc(GL_LEQUAL);
//}

//void Sector::reset(QVector3D position)
//{
//    //在需要修改渲染内容时调用
//    mPosition = position;
//    //清空上一次渲染的角度
//    mAngle = 0;
//    mLastCosRadius = 1;
//    mIsUpper180 = false;
//    mIsLowerMinus180 = false;
//}

//int Sector::setRotateAngle(int nowAngle, float nowCosRadius)
//{
//    //角度由正变为负 并且上一次的角度为钝角 是从180到181 而不是-179
//    if (!mIsUpper180 && mAngle > 0 && nowAngle < 0 && mLastCosRadius < 0)
//    {
//        mIsUpper180 = true;
//    }
//    else if (mIsUpper180 && nowAngle > 0)
//    {
//        mIsUpper180 = false;
//    }
//    //角度由负变为正 并且上一次的角度为钝角
//    if (!mIsLowerMinus180 && mAngle < 0 && nowAngle > 0 && mLastCosRadius < 0)
//    {
//        mIsLowerMinus180 = true;
//    }
//    else if (mIsLowerMinus180 && nowAngle < 0)
//    {
//        mIsLowerMinus180 = false;
//    }

//    if (mIsUpper180)
//    {
//        nowAngle += 360;
//    }
//    else if (mIsLowerMinus180)
//    {
//        nowAngle -= 360;
//    }

//    //本次数据保存为last 用于下一次计算
//    mLastCosRadius = nowCosRadius;
//    //设置正确需要旋转的角度
//    int deltaAngle = nowAngle - mAngle;
//    mAngle = nowAngle;
//    //返回角度的变化值 用于进行模型的旋转
//    return deltaAngle;
//}




///******************************************************************
//        RotationCoordinate
//  *********************************************************************/

//RotationCoordinate::RotationCoordinate()
//    : Coordinate()
//{
//    mQuarterRound = new QuarterRound();
//    yozSurface = new QuarterRoundBoundingBox2D(0, 0, 0, 1, PlaneName::YOZ);
//    xozSurface = new QuarterRoundBoundingBox2D(0, 0, 0, 1, PlaneName::XOZ);
//    xoySurface = new QuarterRoundBoundingBox2D(0, 0, 0, 1, PlaneName::XOY);
//    isRotating = false;
//    mRound = new Round();
//    mTickLine = new TickLine();
//    mSector = new Sector();
//}

//void RotationCoordinate::init()
//{
//    mQuarterRound->init();
//    mRound->init();
//    mTickLine->init();
//    mSector->init();
//}

//void RotationCoordinate::render(Camera camera)
//{
//    //没有选中模型时
//    if (mSelectedObject == nullptr)
//        return;

//    QMatrix4x4 modelMatrix = getModelMatrix(camera.pos);

//    //正在旋转的时候渲染圆面 不渲染坐标轴 也不进行坐标轴的碰撞检测
//    //旋转坐标轴始终正对着视线
//    QVector3D temp = camera.pos - mPosition;
//    int xSign = temp.x() >= 0 ? 1 : -1;
//    int ySign = temp.y() >= 0 ? 1 : -1;
//    int zSign = temp.z() >= 0 ? 1 : -1;
//    if (!isRotating)
//    {
//        modelMatrix.scale(xSign, ySign, zSign);
//        //圆的四分之一碰撞区域也要镜像变换
//        yozSurface->setSign(xSign, ySign, zSign);
//        xozSurface->setSign(xSign, ySign, zSign);
//        xoySurface->setSign(xSign, ySign, zSign);

//        mQuarterRound->render(modelMatrix, camera);

//        //碰撞yoz面 表示绕x轴旋转
//        float distance;
//        hitBoundingBox(modelMatrix, yozSurface, xozSurface, xoySurface,
//                       AxisName::AxisX, AxisName::AxisY, AxisName::AxisZ, distance);
//    }
//    else
//    {
//        //旋转时的渲染
//        //根据位置指向摄像机的向量 求圆盘位于左边还是右边 用于判断显示哪一半边的扇形
//        if (mSelectedAxis == AxisName::AxisY)
//        {
//            //旋转圆是以yoz面为标准写的 需要旋转
//            modelMatrix.rotate(-90, 0, 1, 0);
//            modelMatrix.rotate(90, 0, 0, 1);
//        }
//        else if(mSelectedAxis == AxisName::AxisZ)
//        {
//            modelMatrix.rotate(-90, 0, 1, 0);
//        }
//        mRound->render(modelMatrix, camera);
//        mTickLine->render(modelMatrix, camera);
//        mSector->render(modelMatrix, camera);
//    }
//}

//void RotationCoordinate::resize(float width, float height)
//{
//    mQuarterRound->resize(width, height);
//    mRound->resize(width, height);
//    mTickLine->resize(width, height);
//    mSector->resize(width, height);
//}

//void RotationCoordinate::setSelectedAxis(AxisFlags axis)
//{
//    mQuarterRound->setSelectedAxis(axis);
//    mSelectedAxis = axis;
//}

//bool RotationCoordinate::rotate(Ray ray)
//{
//    //坐标隐藏后 不渲染 但鼠标事件会触发
//    if (mSelectedObject == nullptr)
//        return false;

//    QVector3D mousePos;
//    QVector3D rotateAxis;
//    if (mSelectedAxis == AxisName::AxisNULL)
//    {
//        return false;
//    }
//    else if (mSelectedAxis == AxisName::AxisX)
//    {
//        mousePos = ray.computeIntersectWithYOZPlane(mPosition.x());
//        rotateAxis = QVector3D(1, 0, 0);
//    }
//    else if (mSelectedAxis == AxisName::AxisY)
//    {
//        mousePos = ray.computeIntersectWithXOZPlane(mPosition.y());
//        rotateAxis = QVector3D(0, 1, 0);
//    }
//    else if (mSelectedAxis == AxisName::AxisZ)
//    {
//        mousePos = ray.computeIntersectWithXOYPlane(mPosition.z());
//        rotateAxis = QVector3D(0, 0, 1);
//    }
//    else
//    {
//        return false;
//    }

//    //刚开始旋转 算不出位移 只记录上一次鼠标位置 并开启正在旋转的开关
//    if (lastMousePos.isNull())
//    {
//        isRotating = true;
//        lastMousePos = mousePos;
//        mLastVector = (lastMousePos - mPosition).normalized();
//        //旋转圆盘重置位置 清空上一次渲染的角度 等信息
//        mSector->reset(mPosition);
//        return false;
//    }

//    //计算两次鼠标位置与坐标原点组成的夹角
//    QVector3D nowVector = (mousePos - mPosition).normalized();
//    float cosRadius = QVector3D::dotProduct(mLastVector, nowVector);
//    int angle = round(acosf(cosRadius) / 3.141593f * 180);
//    QVector3D crossResult = QVector3D::crossProduct(mLastVector, nowVector);
//    float sign = crossResult.x() + crossResult.y() + crossResult.z();
//    sign = sign > 0 ? 1 : -1;
//    angle = sign * angle;
//    int deltaAngle = mSector->setRotateAngle(angle, cosRadius);

//    mSelectedObject->setRotation(deltaAngle, rotateAxis);

//    //记录开始旋转时的鼠标位置 不用每次更新！ lastMousePos = mousePos;
//    return true;
//}

//void RotationCoordinate::stopRotate()
//{
//    //鼠标释放的时候调用
//    isRotating = false;
//}

///******************************************************************
//        ScaleCoordinate
//  *********************************************************************/

//ScaleCoordinate::ScaleCoordinate()
//    : Coordinate()
//{
//    mAxis = new Axis();
//    mCube = new Cube();
//    surface = new TriangleSurface();
//    yozSurface = new RectBoundingBox2D(0.0f, 0.15f, 0.15f, 0.0f, 0.15f, 0.15f);
//    xozSurface = new RectBoundingBox2D(0.15f, 0.0f, 0.15f, 0.15f, 0.0f, 0.15f);
//    xoySurface = new RectBoundingBox2D(0.15f, 0.15f, 0.0f, 0.15f, 0.15f, 0.0f);
//    xBoundingBox = new BoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.69f, 0.0f, 0.0f, 0.39f, 0.08f, 0.08f);
//    yBoundingBox = new BoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.69f, 0.0f, 0.08f, 0.39f, 0.08f);
//    zBoundingBox = new BoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, 0.0f, 0.0f, 0.69f, 0.08f, 0.08f, 0.39f);
//    xyzSurface = new TriangleBoundingBox2D(QVector3D(0.3f, 0.0f, 0.0f),
//                                           QVector3D(0.0f, 0.3f, 0.0f),
//                                           QVector3D(0.0f, 0.0f, 0.3f));
//    mScale = QVector3D(1, 1, 1);
//}

//void ScaleCoordinate::init()
//{
//    mAxis->init();
//    mCube->init();
//    surface->init();
//}

//void ScaleCoordinate::render(Camera camera)
//{
//    if (mSelectedObject == nullptr)
//        return;

//    QMatrix4x4 modelMatrix = getModelMatrix(camera.pos);
//    modelMatrix.rotate(QQuaternion::fromEulerAngles(mRotation));

//    mCube->scale(mScale.x(), mScale.y(), mScale.z());
//    mCube->render(modelMatrix, camera);
//    QMatrix4x4 axisModelMatrix = modelMatrix;
//    axisModelMatrix.scale(mScale);
//    mAxis->render(axisModelMatrix, camera);
//    surface->render(modelMatrix, camera);

//    //缩放坐标轴时 无需计算坐标系的碰撞
//    if (!lastMousePos.isNull())
//        return;

//    //在一个面上缩放 无需检测单个坐标轴
//    float distance;
//    if (hitBoundingBox(modelMatrix, yozSurface, xozSurface, xoySurface,
//                       AxisName::AxisY | AxisName::AxisZ,
//                       AxisName::AxisX | AxisName::AxisZ,
//                       AxisName::AxisX | AxisName::AxisY, distance))
//    {
//        //选中了两个轴 是否第三个轴也被选中 对中间的三角形进行碰撞检测
//        float xyzDistance;
//        if (xyzSurface->hitBoundingBox(mRay, xyzDistance, modelMatrix))
//        {
//            if (xyzDistance <= distance)
//            {
//                //中间三角形碰撞点离得更近 选中三个轴
//                setSelectedAxis(AxisName::AxisX | AxisName::AxisY | AxisName::AxisZ);
//            }
//        }
//        return;
//    }

//    hitBoundingBox(modelMatrix, xBoundingBox, yBoundingBox, zBoundingBox,
//                   AxisName::AxisX, AxisName::AxisY, AxisName::AxisZ, distance);
//}

//void ScaleCoordinate::resize(float width, float height)
//{
//    mAxis->resize(width, height);
//    mCube->resize(width, height);
//    surface->resize(width, height);
//}

//void ScaleCoordinate::setSelectedAxis(AxisFlags axis)
//{
//    mAxis->setSelectedAxis(axis);
//    mCube->setSelectedAxis(axis);
//    mSelectedAxis = axis;
//}

//bool ScaleCoordinate::scale(Ray ray)
//{
//    //坐标隐藏后 不渲染 但鼠标事件会触发
//    if (mSelectedObject == nullptr)
//        return false;

//    QVector3D mousePos;
//    QMatrix4x4 rotMatrix;
//    rotMatrix.rotate(QQuaternion::fromEulerAngles(mRotation));
//    if (mSelectedAxis == AxisName::AxisNULL)
//    {
//        return false;
//    }
//    else if ((mSelectedAxis == AxisName::AxisY)
//            || (mSelectedAxis == (AxisName::AxisX | AxisName::AxisY)))
//    {
//        ray.computeIntersectWithPlane(mPosition, rotMatrix * QVector3D(0, 0, 1), mousePos);
//    }
//    else if ((mSelectedAxis == AxisName::AxisX || mSelectedAxis == AxisName::AxisZ)
//            || (mSelectedAxis == (AxisName::AxisX | AxisName::AxisZ)))
//    {
//        ray.computeIntersectWithPlane(mPosition, rotMatrix * QVector3D(0, 1, 0), mousePos);
//    }
//    else if (mSelectedAxis == (AxisName::AxisY | AxisName::AxisZ))
//    {
//        ray.computeIntersectWithPlane(mPosition, rotMatrix * QVector3D(1, 0, 0), mousePos);
//    }
//    else if (mSelectedAxis == (AxisName::AxisX | AxisName::AxisY | AxisName::AxisZ))
//    {
//        ray.computeIntersectWithPlane(mPosition, rotMatrix * QVector3D(1, 1, 1), mousePos);
//    }
//    else
//    {
//        return false;
//    }

//    //刚开始缩放 算不出位移 只记录开始缩放时鼠标位置 并开启正在缩放的开关
//    if (lastMousePos.isNull())
//    {
//        mIsScaling = true;
//        lastMousePos = mousePos;
//        //刚开始缩放时的缩放值
//        mSelectedObjectScale = mSelectedObject->getScale();
//        return false;
//    }

//    QVector3D mouseDisplacement = mousePos - lastMousePos;
//    mScale = QVector3D(1, 1, 1);
//    QVector3D dir;
//    if (mSelectedAxis == AxisName::AxisX)
//    {
//        //鼠标位移在选中轴上的投影的长度
//        dir = QVector3D(1, 0, 0).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == AxisName::AxisY)
//    {
//        dir = QVector3D(0, 1, 0).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == AxisName::AxisZ)
//    {
//        dir = QVector3D(0, 0, 1).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == (AxisName::AxisY | AxisName::AxisZ))
//    {
//        dir = QVector3D(0, 1, 1).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == (AxisName::AxisX | AxisName::AxisZ))
//    {
//        dir = QVector3D(1, 0, 1).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == (AxisName::AxisX | AxisName::AxisY))
//    {
//        dir = QVector3D(1, 1, 0).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * dir;
//    }
//    else if (mSelectedAxis == (AxisName::AxisX | AxisName::AxisY | AxisName::AxisZ))
//    {
//        dir = QVector3D(1, -1, 1).normalized();
//        mScale += QVector3D::dotProduct(rotMatrix * dir, mouseDisplacement) * QVector3D(1, 1, 1);
//    }

//    mSelectedObject->setScale(mScale * mSelectedObjectScale);
//    return true;
//}

//void ScaleCoordinate::stopScale()
//{
//    mIsScaling = false;
//    mScale = QVector3D(1, 1, 1);
//}

///******************************************************************
//        TransformCoordinate
//  *********************************************************************/

//void TransformCoordinate::setSelectedObjects(QList<GameObject*> objects, CenterPoint *center)
//{
//    //多选的操作对象没变 不用做额外操作
//    //if (mSelectedObjects == objects)
//        //return;
//    //有个这句 当子节点取消选中时 真正的多选对象列表并没有变 会出错

//    //原来的操作对象包围盒 指示器等隐藏
//    int count = mSelectedObjects.count();
//    for (int i = 0; i < count; i++)
//    {
//        mSelectedObjects.at(i)->setVisible(false);
//    }
//    //现在的操作对象显示包围盒 指示器等
//    count = objects.count();
//    for (int i = 0; i < count; i++)
//    {
//        objects.at(i)->setVisible(true);
//    }
//    //刷新当前选中
//    mSelectedObjects = objects;
//    //选中对象们的重心
//    mSelectedObject = center;
//    setCoordinateMode(mModeKey);
//}

//GameObject *TransformCoordinate::getSelectedObject()
//{
//    return mSelectedObject;
//}

//char TransformCoordinate::getTransformModeKey()
//{
//    return mModeKey;
//}

//void TransformCoordinate::update()
//{
//    setCoordinateMode(mModeKey);
//}

