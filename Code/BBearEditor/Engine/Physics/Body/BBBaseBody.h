#ifndef BBBASEBODY_H
#define BBBASEBODY_H


#include <QVector3D>

class BBBaseConstraint;

class BBBaseBody
{
public:
    BBBaseBody(int nParticleCount, float fMass);
    virtual ~BBBaseBody();

    void dampenVelocities(float fDeltaTime);
    void predictPositions(float fDeltaTime);
    void projectConstraints(float fDeltaTime);
    void updateVelocities(float fDeltaTime, float fStopThreshold2);
    void updatePositions();

    inline void setParticlePosition(int nIndex, const QVector3D &position) { m_pPositions[nIndex] = position; }
    inline void setParticlePredictedPosition(int nIndex, const QVector3D &position) { m_pPredictedPositions[nIndex] = position; }
    inline void setParticleVelocity(int nIndex, const QVector3D &velocity) { m_pVelocities[nIndex] = velocity; }

    inline int getParticleCount() { return m_nParticleCount; }
    inline float getParticleMass() { return m_fParticleMass; }
    inline QVector3D getParticlePosition(int nIndex) { return m_pPositions[nIndex]; }
    inline QVector3D getParticlePredictedPosition(int nIndex) { return m_pPredictedPositions[nIndex]; }
    inline QVector3D getParticleVelocity(int nIndex) { return m_pVelocities[nIndex]; }

protected:
    int m_nParticleCount;

    float m_fParticleMass;
    float m_fDamping;

    QVector3D *m_pPositions;
    QVector3D *m_pPredictedPositions;
    QVector3D *m_pVelocities;
    std::vector<BBBaseConstraint*> m_Constraints;
};

#endif // BBBASEBODY_H
