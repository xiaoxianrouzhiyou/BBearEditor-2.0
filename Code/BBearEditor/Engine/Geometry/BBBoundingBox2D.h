#ifndef BBBOUNDINGBOX2D_H
#define BBBOUNDINGBOX2D_H


#include "Base/BBRenderableObject2D.h"

class BBBoundingBox2D : public BBRenderableObject2D
{
public:
    BBBoundingBox2D();
    BBBoundingBox2D(int x, int y, int nWidth, int nHeight);

    void init() override;
};


class BBAABBBoundingBox2D : public BBBoundingBox2D
{
public:
    BBAABBBoundingBox2D(float fCenterX, float fCenterY, float fHalfLengthX, float fHalfLengthY, int nWidth, int nHeight);
    BBAABBBoundingBox2D(int x, int y, int nHalfLengthX, int nHalfLengthY);

    void init() override;
    bool hit(int x, int y) override;

    int getHalfLengthX() { return m_nHalfLengthX; }
    int getHalfLengthY() { return m_nHalfLengthY; }

private:
    int m_nHalfLengthX;
    int m_nHalfLengthY;
};


class BBQuarterCircleBoundingBox2D : public BBBoundingBox2D
{
public:
    BBQuarterCircleBoundingBox2D(int x, int y, int nRadius);

    bool hit(int x, int y) override;

private:
    int m_nRadius;
};

#endif // BBBOUNDINGBOX2D_H
