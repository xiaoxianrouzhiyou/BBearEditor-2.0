#ifndef BBCOORDINATECOMPONENT2D_H
#define BBCOORDINATECOMPONENT2D_H


#include "Base/BBRenderableObject2D.h"
#include "BBCoordinateComponent.h"

class BBCoordinateComponent2D : public BBRenderableObject2D
{
public:
    BBCoordinateComponent2D(int x, int y);

    void init() override;

    void setSelectedAxis(const BBAxisFlags &axis);

protected:
    BBAxisFlags m_SelectedAxis;

private:
    virtual void setVertexColor(const BBAxisFlags &axis, bool bSelected) = 0;
};


class BBPositionCoordinateComponent2D : public BBCoordinateComponent2D
{
public:
    BBPositionCoordinateComponent2D(int x, int y);

    void init() override;

    void setVertexColor(const BBAxisFlags &axis, bool bSelected) override;
};


class BBCoordinateCircle2D : public BBCoordinateCircle
{
public:
    BBCoordinateCircle2D(int x, int y, int nWidth, int nHeight);

    void init() override;
    void resize(float fWidth, float fHeight) override;
};


class BBCoordinateTickMark2D : public BBCoordinateTickMark
{
public:
    BBCoordinateTickMark2D(int x, int y, int nWidth, int nHeight);

    void init() override;
    void resize(float fWidth, float fHeight) override;
};


class BBCoordinateSector2D : public BBCoordinateSector
{
public:
    BBCoordinateSector2D(int x, int y, int nWidth, int nHeight);

    void init() override;
    void resize(float fWidth, float fHeight) override;
};


class BBRotationCoordinateComponent2D : public BBCoordinateComponent2D
{
public:
    BBRotationCoordinateComponent2D(int x, int y);

    void init() override;

    void setVertexColor(const BBAxisFlags &axis, bool bSelected) override;
};


class BBScaleCoordinateComponent2D : public BBCoordinateComponent2D
{
public:
    BBScaleCoordinateComponent2D(int x, int y);

    void init() override;

    void setVertexColor(const BBAxisFlags &axis, bool bSelected) override;
    void scale(int nDeltaX, int nDeltaY);
    void reset();
};

#endif // BBCOORDINATECOMPONENT2D_H
