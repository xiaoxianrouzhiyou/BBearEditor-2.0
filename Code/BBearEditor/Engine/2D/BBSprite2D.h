#ifndef BBSPRITE2D_H
#define BBSPRITE2D_H


#include "Base/BBGameObject.h"

class BBSprite2D : public BBGameObject
{
public:
    BBSprite2D(int x = 0, int y = 0, int nWidth = 50, int nHeight = 50);
    ~BBSprite2D();

    void init() override;
};

#endif // BBSPRITE2D_H
