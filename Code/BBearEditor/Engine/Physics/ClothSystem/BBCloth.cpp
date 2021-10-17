#include "BBCloth.h"
#include "BBClothMesh.h"
#include "../Body/BBClothBody.h"


BBCloth::BBCloth(int nWidth, int nHeight)
{
    m_pClothMesh = new BBClothMesh(nWidth, nHeight);
    m_pClothBody = new BBClothBody(m_pClothMesh, 1.0f, 0.25f);
}

BBCloth::~BBCloth()
{
    BB_SAFE_DELETE(m_pClothBody);
    BB_SAFE_DELETE(m_pClothMesh);
}

void BBCloth::init()
{

}
