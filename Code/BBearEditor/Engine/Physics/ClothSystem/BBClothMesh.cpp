#include "BBClothMesh.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "../Body/BBClothBody.h"
#include "Render/BBRenderPass.h"


BBClothMesh::BBClothMesh(float fWidth, float fHeight, float fUnitStep)
    : BBRenderableObject()
{
    m_fWidth = fWidth;
    m_fHeight = fHeight;
    m_fUnitStep = fUnitStep;
    m_nColumn = ceil(m_fWidth / m_fUnitStep) + 1;
    m_nRow = ceil(m_fHeight / m_fUnitStep) + 1;
    m_fUnitStep = m_fWidth / m_nColumn;
}

void BBClothMesh::init()
{
    m_pVBO = new BBVertexBufferObject(m_nColumn * m_nRow);
    m_nIndexCount = (m_nColumn - 1) * (m_nRow - 1) * 6;
    m_pIndexes = new unsigned short[m_nIndexCount];
    int nIndexesIndex = 0;

    for (int i = 0; i < m_nRow; i++)
    {
        for (int j = 0; j < m_nColumn; j++)
        {
            QVector3D position = QVector3D(j, i, 0) * m_fUnitStep;
            int nIndex = i * m_nColumn + j;
            m_pVBO->setPosition(nIndex, position);
            m_pVBO->setColor(nIndex, BBConstant::m_LightGreen);
            m_pVBO->setTexcoord(nIndex, 1.0f / m_fWidth * j, 1.0f / m_fHeight * i);
            m_pVBO->setNormal(nIndex, 0, 0, 1);
            m_pVBO->setTangent(nIndex, -1, 0, 0, -1);

            if (i < m_nRow - 1 && j < m_nColumn - 1)
            {
                m_pIndexes[nIndexesIndex++] = nIndex;
                m_pIndexes[nIndexesIndex++] = nIndex + 1;
                m_pIndexes[nIndexesIndex++] = nIndex + m_nColumn + 1;

                m_pIndexes[nIndexesIndex++] = nIndex;
                m_pIndexes[nIndexesIndex++] = nIndex + m_nColumn + 1;
                m_pIndexes[nIndexesIndex++] = nIndex + m_nColumn;
            }

            if (j == 0)
            {
                m_LeftVertexIndexes.push_back(nIndex);
            }
        }
    }

    m_pCurrentMaterial->init("base", BB_PATH_RESOURCE_SHADER(base.vert), BB_PATH_RESOURCE_SHADER(base.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setPolygonMode(GL_LINE);

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
