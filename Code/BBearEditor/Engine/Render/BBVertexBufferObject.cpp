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
    return QVector3D(m_pVertexes[index].m_fPosition[0], m_pVertexes[index].m_fPosition[1], m_pVertexes[index].m_fPosition[2]);
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

QVector2D BBVertexBufferObject::getTexcoord(int index)
{
    return QVector2D(m_pVertexes[index].m_fTexcoord[0], m_pVertexes[index].m_fTexcoord[1]);
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

QVector3D BBVertexBufferObject::getNormal(int index)
{
    return QVector3D(m_pVertexes[index].m_fNormal[0], m_pVertexes[index].m_fNormal[1], m_pVertexes[index].m_fNormal[2]);
}

void BBVertexBufferObject::computeTangent(unsigned short *pVertexIndexes, int nIndexCount)
{
    // For the time, only triangles with EBO are considered
    BB_PROCESS_ERROR_RETURN(pVertexIndexes);
    for (int i = 0; i < nIndexCount; i += 3)
    {
        int nIndex0 = pVertexIndexes[i];
        int nIndex1 = pVertexIndexes[i + 1];
        int nIndex2 = pVertexIndexes[i + 2];

        QVector3D pos0 = getPosition(nIndex0);
        QVector3D pos1 = getPosition(nIndex1);
        QVector3D pos2 = getPosition(nIndex2);

        QVector2D uv0 = getTexcoord(nIndex0);
        QVector2D uv1 = getTexcoord(nIndex1);
        QVector2D uv2 = getTexcoord(nIndex2);

        _computeTangent(nIndex0, pos0, pos1, pos2, uv0, uv1, uv2);
        _computeTangent(nIndex1, pos1, pos2, pos0, uv1, uv2, uv0);
        _computeTangent(nIndex2, pos2, pos0, pos1, uv2, uv0, uv1);
    }
}

void BBVertexBufferObject::setTangent(int index, float x, float y, float z)
{
    m_pVertexes[index].m_fTangent[0] = x;
    m_pVertexes[index].m_fTangent[1] = y;
    m_pVertexes[index].m_fTangent[2] = z;
    m_pVertexes[index].m_fTangent[3] = 1.0f;
}

void BBVertexBufferObject::setTangent(int index, const QVector3D &tangent)
{
    setTangent(index, tangent.x(), tangent.y(), tangent.z());
}

void BBVertexBufferObject::setBiTangent(int index, float x, float y, float z)
{
    m_pVertexes[index].m_fBiTangent[0] = x;
    m_pVertexes[index].m_fBiTangent[1] = y;
    m_pVertexes[index].m_fBiTangent[2] = z;
    m_pVertexes[index].m_fBiTangent[3] = 1.0f;
}

void BBVertexBufferObject::setBiTangent(int index, const QVector3D &bitangent)
{
    setBiTangent(index, bitangent.x(), bitangent.y(), bitangent.z());
}

QVector3D BBVertexBufferObject::getTangent(int index)
{
    return QVector3D(m_pVertexes[index].m_fTangent[0], m_pVertexes[index].m_fTangent[1], m_pVertexes[index].m_fTangent[2]);
}

QVector3D BBVertexBufferObject::getBiTangent(int index)
{
    return QVector3D(m_pVertexes[index].m_fBiTangent[0], m_pVertexes[index].m_fBiTangent[1], m_pVertexes[index].m_fBiTangent[2]);
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

void BBVertexBufferObject::_computeTangent(int nVertexIndex,
                                           const QVector3D &pos0, const QVector3D &pos1, const QVector3D &pos2,
                                           const QVector2D &uv0, const QVector2D &uv1, const QVector2D &uv2)
{
    QVector3D e0 = pos1 - pos0;
    QVector3D e1 = pos2 - pos0;
    QVector2D deltaUV0 = uv1 - uv0;
    QVector2D deltaUV1 = uv2 - uv0;

    float f = 1.0f / (deltaUV0.x() * deltaUV1.y() - deltaUV1.x() * deltaUV0.y());
    QVector3D tangent;
    tangent.setX(f * (deltaUV1.y() * e0.x() - deltaUV0.y() * e1.x()));
    tangent.setY(f * (deltaUV1.y() * e0.y() - deltaUV0.y() * e1.y()));
    tangent.setZ(f * (deltaUV1.y() * e0.z() - deltaUV0.y() * e1.z()));
    tangent.normalize();
    QVector3D bitangent;
    bitangent.setX(f * (-deltaUV1.x() * e0.x() + deltaUV0.x() * e1.x()));
    bitangent.setY(f * (-deltaUV1.x() * e0.y() + deltaUV0.x() * e1.y()));
    bitangent.setZ(f * (-deltaUV1.x() * e0.z() + deltaUV0.x() * e1.z()));
    bitangent.normalize();

    setTangent(nVertexIndex, tangent);
    setBiTangent(nVertexIndex, bitangent);
}
