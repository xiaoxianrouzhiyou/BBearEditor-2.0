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
    m_pCurrentMaterial->init("base",
                             BB_PATH_RESOURCE_SHADER(base.vert),
                             BB_PATH_RESOURCE_SHADER(base.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);

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

    BBDrawCall *pDrawCall = new BBDrawCall();
    pDrawCall->setMaterial(m_pCurrentMaterial);
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


/**
 * @brief BBPointLightIndicator::BBPointLightIndicator
 */
BBPointLightIndicator::BBPointLightIndicator()
    : BBPointLightIndicator(QVector3D(0, 0, 0))
{

}

BBPointLightIndicator::BBPointLightIndicator(const QVector3D &position)
    : BBLightIndicator(position, QVector3D(0, 0, 0))
{

}

void BBPointLightIndicator::init()
{
    m_pVBO = new BBVertexBufferObject(144);
    for (int i = 0; i < 48; i++)
    {
        float c = cosf(0.1309f * i);
        float s = sinf(0.1309f * i);

        m_pVBO->setPosition(i, c, 0.0f, s);
        m_pVBO->setNormal(i, c, 0.0f, s);
        m_pVBO->setColor(i, 0.909804f, 0.337255f, 0.333333f);

        m_pVBO->setPosition(i + 48, 0.0f, c, s);
        m_pVBO->setNormal(i + 48, 0.0f, c, s);
        m_pVBO->setColor(i + 48, 0.909804f, 0.337255f, 0.333333f);

        m_pVBO->setPosition(i + 96, c, s, 0.0f);
        m_pVBO->setNormal(i + 96, c, s, 0.0f);
        m_pVBO->setColor(i + 96, 0.909804f, 0.337255f, 0.333333f);
    }

    m_nIndexCount = 288;
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < 48; i++)
    {
        m_pIndexes[2 * i] = i;
        m_pIndexes[2 * i + 1] = i + 1;
        m_pIndexes[2 * i + 96] = i + 48;
        m_pIndexes[2 * i + 97] = i + 49;
        m_pIndexes[2 * i + 192] = i + 96;
        m_pIndexes[2 * i + 193] = i + 97;
    }
    m_pIndexes[95] = 0;
    m_pIndexes[191] = 48;
    m_pIndexes[287] = 96;

    m_pCurrentMaterial->init("diffuse_indicator",
                             BB_PATH_RESOURCE_SHADER(diffuse_indicator.vert),
                             BB_PATH_RESOURCE_SHADER(diffuse.frag));
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);
    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall();
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBPointLightIndicator::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_Position);
    modelMatrix.scale(m_Scale);
    BBLightIndicator::render(modelMatrix, pCamera);
}


/**
 * @brief BBSpotLightIndicator::BBSpotLightIndicator
 */
BBSpotLightIndicator::BBSpotLightIndicator()
    : BBSpotLightIndicator(QVector3D(0, 0, 0), QVector3D(0, 0, 0))
{

}

BBSpotLightIndicator::BBSpotLightIndicator(const QVector3D &position, const QVector3D &rotation)
    : BBLightIndicator(position, rotation)
{

}

void BBSpotLightIndicator::init()
{
    m_pVBO = new BBVertexBufferObject(25);
    for (int i = 0; i < 24; i++)
    {
        float c = 0.267949f * cosf(0.261799f * i);
        float s = 0.267949f * sinf(0.261799f * i);
        m_pVBO->setPosition(i, c, -1.0f, s);
        m_pVBO->setColor(i, 0.909804f, 0.337255f, 0.333333f);
    }
    m_pVBO->setPosition(24, 0.0f, 0.0f, 0.0f);
    m_pVBO->setColor(24, 0.909804f, 0.337255f, 0.333333f);

    m_nIndexCount = 56;
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < 24; i++)
    {
        m_pIndexes[2 * i] = i;
        m_pIndexes[2 * i + 1] = i + 1;
    }
    m_pIndexes[47] = 0;
    for (int i = 0; i < 4; i++)
    {
        m_pIndexes[2 * i + 48] = 24;
        m_pIndexes[2 * i + 49] = 6 * i;
    }

    BBLightIndicator::init();

    BBDrawCall *pDrawCall = new BBDrawCall();
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBSpotLightIndicator::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_Position);
    BBLightIndicator::render(modelMatrix, pCamera);
}
