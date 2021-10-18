#ifndef BBPINCONSTRAINT_H
#define BBPINCONSTRAINT_H


#include "BBBaseConstraint.h"

class BBPinConstraint : public BBBaseConstraint
{
public:
    BBPinConstraint(BBBaseBody *pBody, int nParticleIndex, const QVector3D &fixedPosition);

    void doConstraint(float fDeltaTime) override;

    inline void setFixedPosition(float x, float y, float z) { setFixedPosition(QVector3D(x, y, z)); }
    inline void setFixedPosition(const QVector3D &position) { m_FixedPosition = position; }

private:
    int m_nParticleIndex;
    QVector3D m_FixedPosition;
};

#endif // BBPINCONSTRAINT_H
