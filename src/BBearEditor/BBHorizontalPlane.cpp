#include "BBHorizontalPlane.h"
#include "BBGLBuffers.h"
#include "BBGLShader.h"
#include "BBUtils.h"
#include "BBCamera.h"

BBHorizontalPlane::BBHorizontalPlane()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

void BBHorizontalPlane::init()
{
    m_pVertexBuffer = new BBGLVertexBuffer(246);
    for (int i = 0; i <= 40; i++)
    {
        // The most transparent point on the periphery
        m_pVertexBuffer->setPosition(i, -20 + i, 0.0f, -20);
        m_pVertexBuffer->setNormal(i, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i, 0.847059f, 0.603922f, 0.309804f, 0.01f);

        m_pVertexBuffer->setPosition(i + 41, -20, 0.0f, -20 + i);
        m_pVertexBuffer->setNormal(i + 41, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i + 41, 0.847059f, 0.603922f, 0.309804f, 0.01f);

        m_pVertexBuffer->setPosition(i + 82, -20 + i, 0.0f, 20);
        m_pVertexBuffer->setNormal(i + 82, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i + 82, 0.847059f, 0.603922f, 0.309804f, 0.01f);

        m_pVertexBuffer->setPosition(i + 123, 20, 0.0f, -20 + i);
        m_pVertexBuffer->setNormal(i + 123, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i + 123, 0.847059f, 0.603922f, 0.309804f, 0.01f);

        // Two midlines
        float alpha = 0.09f - abs(-20 + i) * 0.004f;
        m_pVertexBuffer->setPosition(i + 164, -20 + i, 0.0f, 0);
        m_pVertexBuffer->setNormal(i + 164, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i + 164, 0.847059f, 0.603922f, 0.309804f, alpha);
        m_pVertexBuffer->setPosition(i + 205, 0, 0.0f, -20 + i);
        m_pVertexBuffer->setNormal(i + 205, 0.0f, 1.0f, 0.0f);
        m_pVertexBuffer->setColor(i + 205, 0.847059f, 0.603922f, 0.309804f, alpha);
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

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "base.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "base.frag",
                    m_pIndexes,
                    m_nIndexCount);
}

void BBHorizontalPlane::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition().x(), 0, pCamera->getPosition().z());
//    //越高 网格分辨率越低 需要放大 放大倍数为1 10 100 由高度的位数决定
//    int height = abs((int)camera.pos.y());
//    //求高度的位数
//    int num = 1;
//    int ratio;
//    do
//    {
//        ratio = pow(10, num);
//        num++;
//    }
//    while (height / ratio != 0);
//    modelMatrix.scale(ratio / 10);
    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBHorizontalPlane::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDrawElements(GL_LINES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
    glDisable(GL_BLEND);
}
