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
    m_VBO = createBufferObject(GL_ARRAY_BUFFER, sizeof(BBVertex) * m_nVertexCount, GL_STATIC_DRAW, m_pVertexes);
}

void BBVertexBufferObject::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BBVertex) * m_nVertexCount, m_pVertexes);
}

void BBVertexBufferObject::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BBVertexBufferObject::setSize(int nVertexCount)
{
    m_nVertexCount = nVertexCount;
    m_pVertexes = new BBVertex[m_nVertexCount];
    memset(m_pVertexes, 0, sizeof(BBVertex) * m_nVertexCount);
    m_VBO = createBufferObject(GL_ARRAY_BUFFER, sizeof(BBVertex) * m_nVertexCount, GL_STATIC_DRAW, NULL);
}

GLuint BBVertexBufferObject::createBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *pData)
{
    GLuint bufferObject;
    glGenBuffers(1, &bufferObject);
    glBindBuffer(bufferType, bufferObject);
    glBufferData(bufferType, size, pData, usage);
    glBindBuffer(bufferType, 0);
    return bufferObject;
}
