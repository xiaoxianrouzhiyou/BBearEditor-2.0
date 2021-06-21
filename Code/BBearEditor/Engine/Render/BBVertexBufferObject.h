#ifndef BBVERTEXBUFFEROBJECT_H
#define BBVERTEXBUFFEROBJECT_H


#include "BBBufferObject.h"

struct BBVertex
{
    float m_fPosition[4];
    float m_fColor[4];
    float m_fTexcoord[4];
    float m_fNormal[4];
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

    void setNormal(int index, float x, float y, float z);
    void setNormal(int index, const QVector3D &normal);
    void setNormal(int index, const QVector4D &normal);

    inline int getVertexCount() { return m_nVertexCount; }

    void submitData();
    void setDrawParameter(GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount);
    void draw() override;

private:
    void setSize(int nVertexCount, GLenum hint = GL_STATIC_DRAW);

    BBVertex *m_pVertexes;
    int m_nVertexCount;
    GLenum m_eDrawPrimitiveType;
    int m_nDrawStartIndex;
    int m_nDrawCount;
};

#endif // BBVERTEXBUFFEROBJECT_H
