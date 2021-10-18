#ifndef BBCLOTHMESH_H
#define BBCLOTHMESH_H


#include "Base/BBRenderableObject.h"

class BBClothBody;

class BBClothMesh : public BBRenderableObject
{
public:
    BBClothMesh(int nWidth = 10, int nHeight = 6);

    void init() override;

    void updatePhysicsCalculatedPositions(BBClothBody *pClothBody);

public:
    inline std::vector<int> getLeftVertexIndexes() { return m_LeftVertexIndexes; }

private:
    int m_nWidth;
    int m_nHeight;

    std::vector<int> m_LeftVertexIndexes;
};

#endif // BBCLOTHMESH_H
