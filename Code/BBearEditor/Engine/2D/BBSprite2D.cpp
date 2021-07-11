#include "BBSprite2D.h"


BBSprite2D::BBSprite2D()
    : BBSprite2D(0, 0, 100, 100)
{

}

BBSprite2D::BBSprite2D(int nCenterX, int nCenterY, int nWidth, int nHeight)
    : BBRenderableObject2D(nCenterX, nCenterY, nWidth, nHeight)
{

}

BBSprite2D::~BBSprite2D()
{

}

void BBSprite2D::bindCanvas(BBCanvas *pCanvas)
{

}
