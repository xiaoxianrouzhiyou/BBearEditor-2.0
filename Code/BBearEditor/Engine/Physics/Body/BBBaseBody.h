#ifndef BBBASEBODY_H
#define BBBASEBODY_H


#include <QVector3D>

class BBBaseBody
{
public:
    BBBaseBody(int nParticleCount, float fMass);

    inline void setParticlePosition(int nIndex, const QVector3D &position) { m_pPositions[nIndex] = position; }
    inline void setParticlePredictedPosition(int nIndex, const QVector3D &position) { m_pPredictedPositions[nIndex] = position; }

    inline float getParticleMass() { return m_fParticleMass; }
    inline QVector3D getParticlePosition(int nIndex) { return m_pPositions[nIndex]; }
    inline QVector3D getParticlePredictedPosition(int nIndex) { return m_pPredictedPositions[nIndex]; }
    inline QVector3D getParticleVelocity(int nIndex) { return m_pVelocities[nIndex]; }

private:
    int m_nParticleCount;
    float m_fParticleMass;
    QVector3D *m_pPositions;
    QVector3D *m_pPredictedPositions;
    QVector3D *m_pVelocities;
};

#endif // BBBASEBODY_H
