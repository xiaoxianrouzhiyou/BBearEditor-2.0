#ifndef BBCLOTHBODY_H
#define BBCLOTHBODY_H


#include "BBBaseBody.h"

enum BBClothPinConstraintType
{
    Left,
    TopLeft
};

class BBClothMesh;

class BBClothBody : public BBBaseBody
{
public:
    BBClothBody(BBClothMesh *pMesh, float fMass, float fElasticModulus);

    void initPinConstraints(const BBClothPinConstraintType &eType);

private:
    void initPositions();

    void initConstraints();
    void initDistanceConstraint(int nTriangleIndex1, int nTriangleIndex2);

    BBClothMesh *m_pClothMesh;
    float m_fElasticModulus;
};

#endif // BBCLOTHBODY_H
