#ifndef BBRENDERABLEOBJECT2D_H
#define BBRENDERABLEOBJECT2D_H


#include "Base/BBRenderableObject.h"

class BBCanvas;

class BBRenderableObject2D : public BBRenderableObject
{
public:
    BBRenderableObject2D();
    BBRenderableObject2D(int x, int y, int nWidth = 500, int nHeight = 500);
    ~BBRenderableObject2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void render(BBCanvas *pCanvas) override;

protected:

};

#endif // BBRENDERABLEOBJECT2D_H
