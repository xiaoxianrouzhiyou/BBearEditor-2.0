#ifndef BBSPRITE2D_H
#define BBSPRITE2D_H


#include "Base/BBRenderableObject2D.h"

class BBCanvas;

class BBSprite2D : public BBRenderableObject2D
{
public:
    BBSprite2D();
    BBSprite2D(int x, int y, int nWidth, int nHeight);
    ~BBSprite2D();

    void init() override;

public:
    void bindCanvas(BBCanvas *pCanvas);

private:
//    BBRectBoundingBox2D *m_pBoundingBox2D;
};

#endif // BBSPRITE2D_H
