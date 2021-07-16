#ifndef BBCLIPAREA2D_H
#define BBCLIPAREA2D_H


#include "Base/BBRenderableObject2D.h"

class BBClipArea2D : public BBRenderableObject2D
{
public:
    BBClipArea2D(int x = 0, int y = 0, int nWidth = 630, int nHeight = 400);

    void init() override;
};

#endif // BBCLIPAREA2D_H
