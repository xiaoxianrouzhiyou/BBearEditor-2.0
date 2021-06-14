#ifndef BBVERTEXBUFFEROBJECT_H
#define BBVERTEXBUFFEROBJECT_H


#include "BBearGL.h"


class BBVertexBufferObject
{
public:
    BBVertexBufferObject(int nVertexCount);
    virtual ~BBVertexBufferObject();

    void setPosition(int index, float x, float y, float z, float w = 1.0f);
    void setPosition(int index, const QVector3D &position);
    void setPosition(int index, const QVector4D &position);
    QVector3D getPosition(int index);
    inline float *getPosition() { return m_fPosition; }

    void setColor(int index, float r, float g, float b, float a = 1.0f);
    void setColor(int index, const QVector3D &rgb);
    void setColor(int index, const QVector4D &rgba);
    inline float *getColor() { return m_fColor; }

    void setTexcoord(int index, float u, float v);
    void setTexcoord(int index, const QVector2D &uv);
    inline float *getTexcoord() { return m_fTexcoord; }

    void setNormal(int index, float x, float y, float z);
    void setNormal(int index, const QVector3D &normal);
    void setNormal(int index, const QVector4D &normal);
    inline float *getNormal() { return m_fNormal; }

    inline int getVertexCount() { return m_nVertexCount; }

private:
    void setSize(int nVertexCount);

    int m_nVertexCount;
    float *m_fPosition;
    float *m_fColor;
    float *m_fTexcoord;
    float *m_fNormal;
};

#endif // BBVERTEXBUFFEROBJECT_H
