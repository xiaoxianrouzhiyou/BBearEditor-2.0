#ifndef BBBENDINGCONSTRAINT_H
#define BBBENDINGCONSTRAINT_H


#include "BBBaseConstraint.h"

class BBBendingConstraint : public BBBaseConstraint
{
public:
    BBBendingConstraint(BBBaseBody *pBody, int nParticleIndex1, int nParticleIndex2, int nParticleIndex3, float fElasticModulus);

    void doConstraint(float fDeltaTime) override;

private:
    int m_nParticleIndex1;
    int m_nParticleIndex2;
    int m_nParticleIndex3;
    float m_fElasticModulus;
    float m_fOriginLength;
};

#endif // BBBENDINGCONSTRAINT_H
