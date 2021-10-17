#ifndef BBSPHPARTICLESYSTEM_H
#define BBSPHPARTICLESYSTEM_H


#include <QVector3D>

#define MAX_PARTICLE 500

struct BBSPHParticle
{
    QVector3D m_Position;
    QVector3D m_Acceleration;
    QVector3D m_Velocity;
    QVector3D m_EvalVelocity;
    float m_fDensity;
    float m_fPressure;

    QVector3D m_SumGradient;
    float m_SumGradient2;
    QVector3D m_PredictedPosition;
    float m_fPredictedDensity;
    float m_fCorrectPressure;
    QVector3D m_CorrectPressureForce;
    float m_fDensityError;
    float m_fKernel;

    // Index to the next particle
    int m_nNextIndex;

    BBSPHParticle()
    {
        m_Position = QVector3D(0, 0, 0);
        m_Acceleration = QVector3D(0, 0, 0);
        m_Velocity = QVector3D(0, 0, 0);
        m_EvalVelocity = QVector3D(0, 0, 0);
        m_fDensity = 0.0f;
        m_fPressure = 0.0f;
        m_SumGradient = QVector3D(0, 0, 0);
        m_SumGradient2 = 0.0f;
        m_PredictedPosition = QVector3D(0, 0, 0);
        m_fPredictedDensity = 0.0f;
        m_fCorrectPressure = 0.0f;
        m_CorrectPressureForce = QVector3D(0, 0, 0);
        m_fDensityError = 0.0f;
        m_fKernel = 0.0f;
        m_nNextIndex = -1;
    }
};

class BBSPHParticleSystem
{
public:
    BBSPHParticleSystem();
    virtual ~BBSPHParticleSystem();

    void reset(unsigned int nCapacity);
    BBSPHParticle* addParticle(float x, float y, float z);

    BBSPHParticle* getParticle(unsigned int nIndex);
    int getSize() { return m_nParticleCount; }

private:
    BBSPHParticle *m_pParticleBuffer;
    unsigned int m_nParticleCount;
    unsigned int m_nBufferCapacity;
};

#endif // BBSPHPARTICLESYSTEM_H
