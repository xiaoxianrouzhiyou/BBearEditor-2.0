#include "BBPBDSolver.h"
#include <algorithm>
#include "../Force/BBForce.h"
#include "../Body/BBBaseBody.h"


BBPBDSolver::BBPBDSolver(int nSolverIteration, int nCollisionIteration)
{
    m_nSolverIteration = nSolverIteration;
    m_nCollisionIteration = nCollisionIteration;
}

BBPBDSolver::~BBPBDSolver()
{

}

void BBPBDSolver::addForce(BBForce *pForce)
{
    m_Forces.push_back(pForce);
}

void BBPBDSolver::removeForce(BBForce *pForce)
{
    m_Forces.erase(std::remove(std::begin(m_Forces), std::end(m_Forces), pForce), std::end(m_Forces));
}

void BBPBDSolver::addBody(BBBaseBody *pBody)
{
    m_PhysicsBodies.push_back(pBody);
}

void BBPBDSolver::removeBody(BBBaseBody *pBody)
{
    m_PhysicsBodies.erase(std::remove(std::begin(m_PhysicsBodies), std::end(m_PhysicsBodies), pBody), std::end(m_PhysicsBodies));
}

void BBPBDSolver::solve(float fDeltaTime)
{
    if (fDeltaTime <= 0.0f)
        return;

    float dt = fDeltaTime / m_nSolverIteration;
    for (int i = 0; i < m_nSolverIteration; i++)
    {
        applyForce(fDeltaTime);
        predictPositions(fDeltaTime);
        projectConstraints();
        updateVelocities(fDeltaTime);
        updatePositions();
    }
}

void BBPBDSolver::applyForce(float fDeltaTime)
{
    for (int i = 0; i < m_PhysicsBodies.size(); i++)
    {
        BBBaseBody *pBody = m_PhysicsBodies[i];
        pBody->dampenVelocities(fDeltaTime);
        for (int j = 0; j < m_Forces.size(); j++)
        {
            m_Forces[j]->applyForce(pBody, fDeltaTime);
        }
    }
}

void BBPBDSolver::predictPositions(float fDeltaTime)
{
    for (int i = 0; i < m_PhysicsBodies.size(); i++)
    {
        m_PhysicsBodies[i]->predictPositions(fDeltaTime);
    }
}

void BBPBDSolver::projectConstraints()
{
    float fDeltaTime = 1.0f / m_nSolverIteration;
    for (int i = 0; i < m_nSolverIteration; i++)
    {
        for (int j = 0; j < m_PhysicsBodies.size(); j++)
        {
            m_PhysicsBodies[j]->projectConstraints(fDeltaTime);
        }
    }
}

void BBPBDSolver::updateVelocities(float fDeltaTime)
{
    float fThreshold2 = m_fStopThreshold * fDeltaTime;
    fThreshold2 = fThreshold2 * fThreshold2;
    for (int i = 0; i < m_PhysicsBodies.size(); i++)
    {
        m_PhysicsBodies[i]->updateVelocities(fDeltaTime, fThreshold2);
    }
}

void BBPBDSolver::updatePositions()
{
    for (int i = 0; i < m_PhysicsBodies.size(); i++)
    {
        m_PhysicsBodies[i]->updatePositions();
    }
}
