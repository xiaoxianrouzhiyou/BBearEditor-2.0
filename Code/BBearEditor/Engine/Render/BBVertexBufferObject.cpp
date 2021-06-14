#include "BBVertexBufferObject.h"


BBVertexBufferObject::BBVertexBufferObject(int nVertexCount)
{
    setSize(nVertexCount);
}

BBVertexBufferObject::~BBVertexBufferObject()
{
    BB_SAFE_DELETE_ARRAY(m_fPosition);
    BB_SAFE_DELETE_ARRAY(m_fColor);
    BB_SAFE_DELETE_ARRAY(m_fTexcoord);
    BB_SAFE_DELETE_ARRAY(m_fNormal);
}

void BBVertexBufferObject::setPosition(int index, float x, float y, float z, float w)
{
    m_fPosition[index * 4] = x;
    m_fPosition[index * 4 + 1] = y;
    m_fPosition[index * 4 + 2] = z;
    m_fPosition[index * 4 + 3] = w;
}

void BBVertexBufferObject::setPosition(int index, const QVector3D &position)
{
    setPosition(index, position.x(), position.y(), position.z());
}

void BBVertexBufferObject::setPosition(int index, const QVector4D &position)
{
    setPosition(index, position.x(), position.y(), position.z(), position.w());
}

QVector3D BBVertexBufferObject::getPosition(int index)
{
    return QVector3D(m_fPosition[index * 4], m_fPosition[index * 4 + 1], m_fPosition[index * 4 + 2]);
}

void BBVertexBufferObject::setColor(int index, float r, float g, float b, float a)
{
    m_fColor[index * 4] = r;
    m_fColor[index * 4 + 1] = g;
    m_fColor[index * 4 + 2] = b;
    m_fColor[index * 4 + 3] = a;
}

void BBVertexBufferObject::setColor(int index, const QVector3D &rgb)
{
    setColor(index, rgb.x(), rgb.y(), rgb.z());
}

void BBVertexBufferObject::setColor(int index, const QVector4D &rgba)
{
    setColor(index, rgba.x(), rgba.y(), rgba.z(), rgba.w());
}

void BBVertexBufferObject::setTexcoord(int index, float u, float v)
{
    m_fTexcoord[index * 2] = u;
    m_fTexcoord[index * 2 + 1] = v;
}

void BBVertexBufferObject::setTexcoord(int index, const QVector2D &uv)
{
    setTexcoord(index, uv.x(), uv.y());
}

void BBVertexBufferObject::setNormal(int index, float x, float y, float z)
{
    m_fNormal[index * 4] = x;
    m_fNormal[index * 4 + 1] = y;
    m_fNormal[index * 4 + 2] = z;
    m_fNormal[index * 4 + 3] = 1.0f;
}

void BBVertexBufferObject::setNormal(int index, const QVector3D &normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBVertexBufferObject::setNormal(int index, const QVector4D &normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBVertexBufferObject::setSize(int nVertexCount)
{
    m_nVertexCount = nVertexCount;
    m_fPosition = new float[m_nVertexCount * 4];
    m_fColor = new float[m_nVertexCount * 4];
    m_fTexcoord = new float[m_nVertexCount * 2];
    m_fNormal = new float[m_nVertexCount * 4];
    memset(m_fPosition, 0, sizeof(float) * m_nVertexCount * 4);
    memset(m_fColor, 0, sizeof(float) * m_nVertexCount * 4);
    memset(m_fTexcoord, 0, sizeof(float) * m_nVertexCount * 2);
    memset(m_fNormal, 0, sizeof(float) * m_nVertexCount * 4);
}
