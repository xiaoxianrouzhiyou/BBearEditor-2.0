#ifndef BBSPHPARTICLESYSTEM_H
#define BBSPHPARTICLESYSTEM_H


#include <QVector3D>

struct BBSPHParticle
{
    QVector3D m_Position;
    QVector3D m_Acceleration;
    QVector3D m_Velocity;
    QVector3D m_FinalVelocity;
    float m_fDensity;
    float m_fPressure;
    // Index to the next particle
    int m_nNextIndex;

    BBSPHParticle()
    {
        m_fDensity = 0.0f;
        m_fPressure = 0.0f;
        m_nNextIndex = -1;
    }
};

class BBSPHParticleSystem
{
public:
    BBSPHParticleSystem();
    ~BBSPHParticleSystem();

private:
    BBSPHParticle *m_pParticleBuffer;
    unsigned int m_nParticleCount;
    unsigned int m_nBufferCapacity;
};

#endif // BBSPHPARTICLESYSTEM_H
