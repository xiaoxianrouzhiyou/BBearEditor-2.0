#ifndef BBCLOTHMESH_H
#define BBCLOTHMESH_H


#include "Base/BBRenderableObject.h"

class BBClothBody;

class BBClothMesh : public BBRenderableObject
{
public:
    BBClothMesh(float fWidth = 10.0f, float fHeight = 6.0f, float fUnitStep = 0.125f);

    void init() override;

    void updatePhysicsCalculatedPositions(BBClothBody *pClothBody);

public:
    inline std::vector<int> getLeftVertexIndexes() { return m_LeftVertexIndexes; }
    inline int getTopLeftVertexIndex() { return 0; }

private:
    float m_fWidth;
    float m_fHeight;
    float m_fUnitStep;
    int m_nColumn;
    int m_nRow;

    std::vector<int> m_LeftVertexIndexes;
};

#endif // BBCLOTHMESH_H
