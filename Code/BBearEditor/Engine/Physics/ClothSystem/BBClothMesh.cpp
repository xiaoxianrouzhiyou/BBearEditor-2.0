#include "BBClothMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "../Body/BBClothBody.h"


BBClothMesh::BBClothMesh(int nWidth, int nHeight)
    : BBRenderableObject()
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
}

void BBClothMesh::init()
{
    int nColumn = m_nWidth + 1;
    int nRow = m_nHeight + 1;
    m_pVBO = new BBVertexBufferObject(nColumn * nRow);
    m_nIndexCount = m_nWidth * m_nHeight * 6;
    m_pIndexes = new unsigned short[m_nIndexCount];
    int nIndexesIndex = 0;

    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nColumn; j++)
        {
            QVector3D position(j, i, 0);
            int nIndex = i * nColumn + j;
            m_pVBO->setPosition(nIndex, position);
            m_pVBO->setColor(nIndex, BBConstant::m_LightGreen);
            m_pVBO->setTexcoord(nIndex, 1.0f / m_nWidth * j, 1.0f / m_nHeight * i);
            m_pVBO->setNormal(nIndex, 0, 0, 1);
            m_pVBO->setTangent(nIndex, -1, 0, 0, -1);

            if (i < m_nHeight && j < m_nWidth)
            {
                m_pIndexes[nIndexesIndex++] = nIndex;
                m_pIndexes[nIndexesIndex++] = nIndex + 1;
                m_pIndexes[nIndexesIndex++] = nIndex + nColumn + 1;

                m_pIndexes[nIndexesIndex++] = nIndex;
                m_pIndexes[nIndexesIndex++] = nIndex + nColumn + 1;
                m_pIndexes[nIndexesIndex++] = nIndex + nColumn;
            }

            if (j == 0)
            {
                m_LeftVertexIndexes.push_back(nIndex);
            }
        }
    }

    m_pCurrentMaterial->init("base", BB_PATH_RESOURCE_SHADER(base.vert), BB_PATH_RESOURCE_SHADER(base.frag));
    
    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_TRIANGLES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBClothMesh::updatePhysicsCalculatedPositions(BBClothBody *pClothBody)
{
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setPosition(i, pClothBody->getParticlePosition(i));
    }
    m_pVBO->submitData();
}
