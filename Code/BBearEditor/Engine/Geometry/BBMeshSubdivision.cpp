#include "BBMeshSubdivision.h"

BBMeshSubdivision::BBMeshSubdivision(const BBMeshSubdivisionMeshType &eType)
{
    if (eType == Triangle)
        m_nMeshUnitPointNum = 3;
    else if (eType == Quadrangle)
        m_nMeshUnitPointNum = 4;
}
