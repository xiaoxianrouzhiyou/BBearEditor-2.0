#ifndef BBHORIZONTALPLANE_H
#define BBHORIZONTALPLANE_H


#include "BBRenderableObject.h"


class BBHorizontalPlane : public BBRenderableObject
{
public:
    BBHorizontalPlane();

    void init() override;
    void render(BBCamera *pCamera) override;

private:
    void draw() override;
};

#endif // BBHORIZONTALPLANE_H
