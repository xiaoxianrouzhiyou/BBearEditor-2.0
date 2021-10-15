#ifndef BBCLOTHMESH_H
#define BBCLOTHMESH_H


#include "Base/BBRenderableObject.h"

class BBClothBody;

class BBClothMesh : public BBRenderableObject
{
public:
    BBClothMesh(int nWidth = 10, int nHeight = 6);

    void init() override;

private:
    int m_nWidth;
    int m_nHeight;
};

#endif // BBCLOTHMESH_H