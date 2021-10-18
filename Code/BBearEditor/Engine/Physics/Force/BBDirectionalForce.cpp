#include "BBDirectionalForce.h"
#include "../Body/BBBaseBody.h"


BBDirectionalForce::BBDirectionalForce(float x, float y, float z)
    : BBDirectionalForce(QVector3D(x, y, z))
{

}

BBDirectionalForce::BBDirectionalForce(const QVector3D &direction)
    : BBForce()
{
    m_Direction = direction;
}

void BBDirectionalForce::applyForce(BBBaseBody *pBody, float fDeltaTime)
{
    for (int i = 0; i < pBody->getParticleCount(); i++)
    {
        QVector3D velocity = pBody->getParticleVelocity(i);
        velocity += m_Direction * fDeltaTime;
        pBody->setParticleVelocity(i, velocity);
    }
}
