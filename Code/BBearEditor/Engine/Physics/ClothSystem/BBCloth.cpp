#include "BBCloth.h"
#include "BBClothMesh.h"
#include "../Body/BBClothBody.h"
#include "../Solver/BBPBDSolver.h"
#include "../Force/BBDirectionalForce.h"


BBCloth::BBCloth(const QVector3D &position, int nWidth, int nHeight)
    : BBGameObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pClothMesh = nullptr;
    m_pClothBody = nullptr;
    m_pPBDSolver = nullptr;
}

BBCloth::~BBCloth()
{
    BB_SAFE_DELETE(m_pPBDSolver);
    BB_SAFE_DELETE(m_pClothBody);
    BB_SAFE_DELETE(m_pClothMesh);
}

void BBCloth::init()
{
    m_pClothMesh = new BBClothMesh(m_nWidth, m_nHeight);
    m_pClothMesh->init();

    m_pClothBody = new BBClothBody(m_pClothMesh, 0.5f, 0.5f);
    m_pClothBody->initPinConstraints(BBClothPinConstraintType::Left);

    m_pPBDSolver = new BBPBDSolver();
    m_pPBDSolver->addBody(m_pClothBody);
    BBDirectionalForce *pWindForce = new BBDirectionalForce(5, 0, 0);
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

void BBCloth::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pClothMesh->setPosition(position, bUpdateLocalTransform);
}
