#include "BBLightIndicator.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/BBCamera.h"
#include "Render/BBVertexBufferObject.h"


/**
 * @brief BBLightIndicator::BBLightIndicator
 */
BBLightIndicator::BBLightIndicator()
    : BBLightIndicator(QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBLightIndicator::BBLightIndicator(const QVector3D &position, const QVector3D &rotation)
    : BBRenderableObject(position, rotation, QVector3D(1, 1, 1))
{

}

void BBLightIndicator::init()
{
    m_pMaterial->init("base",
                      BB_PATH_RESOURCE_SHADER(base.vert),
                      BB_PATH_RESOURCE_SHADER(base.frag));
    m_pMaterial->getBaseRenderPass()->setZTestState(false);
    m_pMaterial->getBaseRenderPass()->setLineWidth(1.5f);

    BBRenderableObject::init();
}


/**
 * @brief BBDirectionalLightIndicator::BBDirectionalLightIndicator
 */
BBDirectionalLightIndicator::BBDirectionalLightIndicator()
    : BBDirectionalLightIndicator(QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBDirectionalLightIndicator::BBDirectionalLightIndicator(const QVector3D &position, const QVector3D &rotation)
    : BBLightIndicator(position, rotation)
{

}

void BBDirectionalLightIndicator::init()
{
    m_pVBO = new BBVertexBufferObject(32);
    for (int i = 0, j = 24; i < 24; i++)
    {
        float c = 0.45f * cosf(0.261799f * i);
        float s = 0.45f * sinf(0.261799f * i);
        m_pVBO->setPosition(i, c, 1.4f, s);
        m_pVBO->setColor(i, 0.909804f, 0.337255f, 0.333333f);
        if (i % 3 == 0)
        {
            m_pVBO->setPosition(j, c, -1.4f, s);
            m_pVBO->setColor(j, 0.909804f, 0.337255f, 0.333333f);
            j++;
        }
    }

    m_nIndexCount = 64;
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < 24; i++)
    {
        m_pIndexes[2 * i] = i;
        m_pIndexes[2 * i + 1] = i + 1;
    }
    m_pIndexes[47] = 0;
    for (int i = 0; i < 8; i++)
    {
        m_pIndexes[48 + 2 * i] = 3 * i;
        m_pIndexes[48 + 2 * i + 1] = 24 + i;
    }

    BBLightIndicator::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBDirectionalLightIndicator::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_Position);
    // the size does not change with distance
    float fDistance = (pCamera->getPosition() - m_Position).length();
    modelMatrix.scale(fDistance / 12);
    modelMatrix.rotate(m_Quaternion);
    BBLightIndicator::render(modelMatrix, pCamera);
}
