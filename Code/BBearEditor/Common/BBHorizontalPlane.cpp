#include "BBHorizontalPlane.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBElementBufferObject.h"
#include "Render/BBMaterial.h"
#include "BBUtils.h"
#include "Render/BBCamera.h"
#include "Render/BBDrawCall.h"


BBHorizontalPlane::BBHorizontalPlane()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

void BBHorizontalPlane::init()
{
    float fCoefficient = 2.0f;
    m_pVBO = new BBVertexBufferObject(246);
    for (int i = 0; i <= 40; i++)
    {
        // The most transparent point on the periphery
        m_pVBO->setPosition(i, -20 + i, 0.0f, -20);
        m_pVBO->setNormal(i, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i, 0.847059f, 0.603922f, 0.309804f, 0.01f * fCoefficient);

        m_pVBO->setPosition(i + 41, -20, 0.0f, -20 + i);
        m_pVBO->setNormal(i + 41, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i + 41, 0.847059f, 0.603922f, 0.309804f, 0.01f * fCoefficient);

        m_pVBO->setPosition(i + 82, -20 + i, 0.0f, 20);
        m_pVBO->setNormal(i + 82, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i + 82, 0.847059f, 0.603922f, 0.309804f, 0.01f * fCoefficient);

        m_pVBO->setPosition(i + 123, 20, 0.0f, -20 + i);
        m_pVBO->setNormal(i + 123, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i + 123, 0.847059f, 0.603922f, 0.309804f, 0.01f * fCoefficient);

        // Two midlines
        float alpha = 0.09f - abs(-20 + i) * 0.004f;
        m_pVBO->setPosition(i + 164, -20 + i, 0.0f, 0);
        m_pVBO->setNormal(i + 164, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i + 164, 0.847059f, 0.603922f, 0.309804f, alpha * fCoefficient);
        m_pVBO->setPosition(i + 205, 0, 0.0f, -20 + i);
        m_pVBO->setNormal(i + 205, 0.0f, 1.0f, 0.0f);
        m_pVBO->setColor(i + 205, 0.847059f, 0.603922f, 0.309804f, alpha * fCoefficient);
    }

    m_nIndexCount = 328;
    m_pIndexes = new unsigned short[m_nIndexCount];
    int count = m_nIndexCount / 4;
    for (int i = 0; i < count; i++)
    {
        m_pIndexes[i * 4] = i;
        m_pIndexes[i * 4 + 1] = i + 164;
        m_pIndexes[i * 4 + 2] = i + 82;
        m_pIndexes[i * 4 + 3] = i + 164;
    }
    m_pMaterial->init(BB_PATH_RESOURCE_SHADER(base.vert),
                      BB_PATH_RESOURCE_SHADER(base.frag),
                      m_pIndexes,
                      m_nIndexCount);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

void BBHorizontalPlane::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition().x(), 0, pCamera->getPosition().z());

    // The higher, the lower the grid resolution
    // need to be enlarged, 1 10 100, determined by the digit number of height.
    int height = abs((int)pCamera->getPosition().y());
    // compute the digit number of height
    int num = 1;
    int ratio;
    do
    {
        ratio = pow(10, num);
        num++;
    }
    while (height / ratio != 0);
    modelMatrix.scale(ratio / 10);

    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBHorizontalPlane::draw()
{
//    glEnable(GL_BLEND);
//    glLineWidth(1);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glDrawElements(GL_LINES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
//    glDisable(GL_BLEND);
}
