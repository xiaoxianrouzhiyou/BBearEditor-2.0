#ifndef BBCOORDINATECOMPONENT2D_H
#define BBCOORDINATECOMPONENT2D_H


#include "Base/BBRenderableObject2D.h"

class BBCoordinateComponent2D : public BBRenderableObject2D
{
protected:
    BBCoordinateComponent2D(int x, int y);

    void init() override;

    BBAxisFlags m_SelectedAxis;
};

class BBPositionCoordinateComponent2D : public BBCoordinateComponent2D
{
public:
    BBPositionCoordinateComponent2D(int x, int y);

    void init() override;
};

#endif // BBCOORDINATECOMPONENT2D_H
