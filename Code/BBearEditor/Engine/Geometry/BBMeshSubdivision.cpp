#include "BBMeshSubdivision.h"
#include "Utils/BBUtils.h"


/**
 * @brief BBMeshSubdivision::BBMeshSubdivision
 * @param eType
 */
BBMeshSubdivision::BBMeshSubdivision(const BBMeshSubdivisionMeshType &eType)
{
    if (eType == Triangle)
        m_nMeshUnitPointNum = 3;
    else if (eType == Quadrangle)
        m_nMeshUnitPointNum = 4;
}


/**
 * @brief BBCatmullClarkMeshSubdivision::BBCatmullClarkMeshSubdivision
 * @param eType
 */
BBCatmullClarkMeshSubdivision::BBCatmullClarkMeshSubdivision(const BBMeshSubdivisionMeshType &eType)
    : BBMeshSubdivision(eType)
{
    qDebug() << "1";
}
