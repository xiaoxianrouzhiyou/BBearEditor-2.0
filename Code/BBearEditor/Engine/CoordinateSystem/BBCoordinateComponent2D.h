#ifndef BBCOORDINATECOMPONENT2D_H
#define BBCOORDINATECOMPONENT2D_H


#include "Base/BBRenderableObject2D.h"

class BBCoordinateComponent2D : public BBRenderableObject2D
{
protected:
    BBCoordinateComponent2D(int x, int y);

    void init() override;

    BBAxisFlags m_SelectedAxis;

public:
    void setSelectedAxis(const BBAxisFlags &axis);

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

#endif // BBCOORDINATECOMPONENT2D_H
