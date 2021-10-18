#ifndef BBPBDSOLVER_H
#define BBPBDSOLVER_H


#include <vector>

class BBForce;
class BBBaseBody;

class BBPBDSolver
{
public:
    BBPBDSolver(int nSolverIteration = 4, int nCollisionIteration = 2);
    ~BBPBDSolver();

    void addForce(BBForce *pForce);
    void removeForce(BBForce *pForce);
    void addBody(BBBaseBody *pBody);
    void removeBody(BBBaseBody *pBody);

    void solve(float fDeltaTime);

private:
    void applyForce(float fDeltaTime);
    void predictPositions(float fDeltaTime);
    void projectConstraints();
    void updateVelocities(float fDeltaTime);
    void updatePositions();

private:
    int m_nSolverIteration;
    int m_nCollisionIteration;
    float m_fStopThreshold;

    std::vector<BBForce*> m_Forces;
    std::vector<BBBaseBody*> m_PhysicsBodies;
};

#endif // BBPBDSOLVER_H
