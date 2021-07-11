#ifndef BBSPRITE2D_H
#define BBSPRITE2D_H


#include "Base/BBRenderableObject2D.h"

class BBCanvas;

class BBSprite2D : public BBRenderableObject2D
{
public:
    BBSprite2D();
    BBSprite2D(int nCenterX, int nCenterY, int nWidth, int nHeight);
    ~BBSprite2D();

public:
    void bindCanvas(BBCanvas *pCanvas);
};

#endif // BBSPRITE2D_H
