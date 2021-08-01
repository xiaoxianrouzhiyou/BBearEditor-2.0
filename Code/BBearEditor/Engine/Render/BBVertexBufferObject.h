#ifndef BBVERTEXBUFFEROBJECT_H
#define BBVERTEXBUFFEROBJECT_H


#include "BBBufferObject.h"

struct BBVertex
{
    float m_fPosition[4];
    float m_fColor[4];
    float m_fTexcoord[4];
    float m_fNormal[4];
    float m_fTangent[4];
    float m_fBiTangent[4];
};

class BBVertexBufferObject : public BBBufferObject
{
public:
    BBVertexBufferObject(int nVertexCount);
    ~BBVertexBufferObject();

    void setPosition(int index, float x, float y, float z, float w = 1.0f);
    void setPosition(int index, const QVector3D &position);
    void setPosition(int index, const QVector4D &position);
    QVector3D getPosition(int index);

    void setColor(int index, float r, float g, float b, float a = 1.0f);
    void setColor(int index, const QVector3D &rgb);
    void setColor(int index, const QVector4D &rgba);

    void setTexcoord(int index, float u, float v);
    void setTexcoord(int index, const QVector2D &uv);
    QVector2D getTexcoord(int index);

    void setNormal(int index, float x, float y, float z);
    void setNormal(int index, const QVector3D &normal);
    void setNormal(int index, const QVector4D &normal);
    QVector3D getNormal(int index);

    void computeTangent(unsigned short *pVertexIndexes, int nIndexCount);
    void setTangent(int index, float x, float y, float z);
    void setTangent(int index, const QVector3D &tangent);
    void setBiTangent(int index, float x, float y, float z);
    void setBiTangent(int index, const QVector3D &bitangent);

    inline int getVertexCount() { return m_nVertexCount; }

    void submitData();
    void draw(GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount);

private:
    void setSize(int nVertexCount, GLenum hint = GL_STATIC_DRAW);
    void _computeTangent(int nVertexIndex,
                         const QVector3D &pos0, const QVector3D &pos1, const QVector3D &pos2,
                         const QVector2D &uv0, const QVector2D &uv1, const QVector2D &uv2);

    BBVertex *m_pVertexes;
    int m_nVertexCount;
};

#endif // BBVERTEXBUFFEROBJECT_H
