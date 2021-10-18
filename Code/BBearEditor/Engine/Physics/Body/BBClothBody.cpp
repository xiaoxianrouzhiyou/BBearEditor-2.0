#include "BBClothBody.h"
#include "../ClothSystem/BBClothMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "../Constraint/BBDistanceConstraint.h"
#include "../Constraint/BBPinConstraint.h"


BBClothBody::BBClothBody(BBClothMesh *pMesh, float fMass, float fElasticModulus)
    : BBBaseBody(pMesh->getVertexCount(), fMass)
{
    m_pClothMesh = pMesh;
    m_fElasticModulus = fElasticModulus;
    initPositions();
    initConstraints();
}

void BBClothBody::initPinConstraints(const BBClothPinConstraintType &eType)
{
    if (eType == Left)
    {
        std::vector<int> leftVertexIndexes = m_pClothMesh->getLeftVertexIndexes();
        for (int i = 0; i < leftVertexIndexes.size(); i++)
        {
            int nIndex = leftVertexIndexes[i];
            BBPinConstraint *pConstraint = new BBPinConstraint(this, nIndex);
            m_Constraints.push_back(pConstraint);
        }
    }
    else
    {

    }
}

void BBClothBody::initPositions()
{
    for (int i = 0; i < m_nParticleCount; i++)
    {
        m_pPositions[i] = m_pClothMesh->getVBO()->getPosition(i);
        m_pPredictedPositions[i] = m_pPositions[i];
    }
}

void BBClothBody::initConstraints()
{
    unsigned short *pVertexIndexes = m_pClothMesh->getVertexIndexes();
    int nIndexCount = m_pClothMesh->getIndexCount();

    for (int i = 0; i < nIndexCount; i += 3)
    {
        int nIndex0 = pVertexIndexes[i];
        int nIndex1 = pVertexIndexes[i + 1];
        int nIndex2 = pVertexIndexes[i + 2];

        // form three pairs
        initDistanceConstraint(nIndex0, nIndex1);
        initDistanceConstraint(nIndex1, nIndex2);
        initDistanceConstraint(nIndex0, nIndex2);
    }
}

void BBClothBody::initDistanceConstraint(int nTriangleIndex1, int nTriangleIndex2)
{
    int nParticleIndex1;
    int nParticleIndex2;
    if (nTriangleIndex1 < nTriangleIndex2)
    {
        nParticleIndex1 = nTriangleIndex1;
        nParticleIndex2 = nTriangleIndex2;
    }
    else
    {
        nParticleIndex1 = nTriangleIndex2;
        nParticleIndex2 = nTriangleIndex1;
    }
    BBDistanceConstraint *pConstraint = new BBDistanceConstraint(this, nParticleIndex1, nParticleIndex2, m_fElasticModulus);
    m_Constraints.push_back(pConstraint);
}
