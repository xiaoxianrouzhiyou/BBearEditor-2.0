#ifndef BBDISTANCECONSTRAINT_H
#define BBDISTANCECONSTRAINT_H


#include "BBBaseConstraint.h"

class BBDistanceConstraint : public BBBaseConstraint
{
public:
    BBDistanceConstraint(BBBaseBody *pBody, int nParticleIndex1, int nParticleIndex2, float fElasticModulus);

    void doConstraint(float fDeltaTime) override;

private:
    int m_nParticleIndex1;
    int m_nParticleIndex2;
    float m_fElasticModulus;
    float m_fOriginLength;
};

#endif // BBDISTANCECONSTRAINT_H
