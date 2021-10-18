#include "BBCloth.h"
#include "BBClothMesh.h"
#include "../Body/BBClothBody.h"
#include "../Solver/BBPBDSolver.h"
#include "../Force/BBDirectionalForce.h"


BBCloth::BBCloth(int nWidth, int nHeight)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pClothMesh = new BBClothMesh(nWidth, nHeight);
    m_pClothBody = new BBClothBody(m_pClothMesh, 1.0f, 0.25f);
    m_pPBDSolver = new BBPBDSolver();
}

BBCloth::~BBCloth()
{
    BB_SAFE_DELETE(m_pPBDSolver);
    BB_SAFE_DELETE(m_pClothBody);
    BB_SAFE_DELETE(m_pClothMesh);
}

void BBCloth::init()
{
    m_pClothBody->initPinConstraints(BBClothPinConstraintType::Left);
    m_pPBDSolver->addBody(m_pClothBody);
    BBDirectionalForce *pWindForce = new BBDirectionalForce(10, 1, 0);
    m_pPBDSolver->addForce(pWindForce);
    BBDirectionalForce *pGravity = new BBDirectionalForce(0, -9.8f, 0);
    m_pPBDSolver->addForce(pGravity);
}

void BBCloth::render(BBCamera *pCamera)
{
    m_pPBDSolver->solve(BB_CONSTANT_UPDATE_RATE_MS);
    m_pClothMesh->updatePhysicsCalculatedPositions(m_pClothBody);
    m_pClothMesh->render(pCamera);
}
