#include "BBVertexBufferObject.h"


BBVertexBufferObject::BBVertexBufferObject(int nVertexCount)
    : BBBufferObject()
{
    setSize(nVertexCount);
}

BBVertexBufferObject::~BBVertexBufferObject()
{
    BB_SAFE_DELETE_ARRAY(m_pVertexes);
}

void BBVertexBufferObject::setPosition(int index, float x, float y, float z, float w)
{
    m_pVertexes[index].m_fPosition[0] = x;
    m_pVertexes[index].m_fPosition[1] = y;
    m_pVertexes[index].m_fPosition[2] = z;
    m_pVertexes[index].m_fPosition[3] = w;
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
    return QVector3D(m_pVertexes[index].m_fPosition[0],
                     m_pVertexes[index].m_fPosition[1],
                     m_pVertexes[index].m_fPosition[2]);
}

void BBVertexBufferObject::setColor(int index, float r, float g, float b, float a)
{
    m_pVertexes[index].m_fColor[0] = r;
    m_pVertexes[index].m_fColor[1] = g;
    m_pVertexes[index].m_fColor[2] = b;
    m_pVertexes[index].m_fColor[3] = a;
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
    m_pVertexes[index].m_fTexcoord[0] = u;
    m_pVertexes[index].m_fTexcoord[1] = v;
    m_pVertexes[index].m_fTexcoord[2] = 0.0f;
    m_pVertexes[index].m_fTexcoord[3] = 0.0f;
}

void BBVertexBufferObject::setTexcoord(int index, const QVector2D &uv)
{
    setTexcoord(index, uv.x(), uv.y());
}

void BBVertexBufferObject::setNormal(int index, float x, float y, float z)
{
    m_pVertexes[index].m_fNormal[0] = x;
    m_pVertexes[index].m_fNormal[1] = y;
    m_pVertexes[index].m_fNormal[2] = z;
    m_pVertexes[index].m_fNormal[3] = 1.0f;
}

void BBVertexBufferObject::setNormal(int index, const QVector3D &normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBVertexBufferObject::setNormal(int index, const QVector4D &normal)
{
    setNormal(index, normal.x(), normal.y(), normal.z());
}

void BBVertexBufferObject::submitData()
{
    updateData(GL_ARRAY_BUFFER, sizeof(BBVertex) * m_nVertexCount, m_pVertexes);
}

void BBVertexBufferObject::draw(GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount)
{
    glDrawArrays(eDrawPrimitiveType, nDrawStartIndex, nDrawCount);
}

void BBVertexBufferObject::setSize(int nVertexCount, GLenum hint)
{
    m_nVertexCount = nVertexCount;
    m_pVertexes = new BBVertex[m_nVertexCount];
    memset(m_pVertexes, 0, sizeof(BBVertex) * m_nVertexCount);
    m_Name = createBufferObject(GL_ARRAY_BUFFER, sizeof(BBVertex) * m_nVertexCount, hint, NULL);
}
