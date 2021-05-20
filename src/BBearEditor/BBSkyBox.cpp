#include "BBSkyBox.h"
#include "BBGLBuffers.h"
#include "BBCamera.h"
#include "BBGLShader.h"
#include "BBUtils.h"

//--------------------
// BBSkyBoxSide
//--------------------

BBSkyBoxSide::BBSkyBoxSide(BBGLVertexBuffer *pVertexBuffer)
    : BBRenderableObject()
{
    m_pVertexBuffer = pVertexBuffer;
}

void BBSkyBoxSide::init(const QString &path)
{
    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "texture.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "texture.frag");
    setTexture(path);
}

void BBSkyBoxSide::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBSkyBoxSide::draw()
{
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


//--------------------
// BBSkyBox
//--------------------

BBSkyBox::BBSkyBox()
    : BBGameObject()
{
    initFront();
    initBack();
    initLeft();
    initRight();
    initTop();
    initBottom();
}

BBSkyBox::~BBSkyBox()
{
    BB_SAFE_DELETE(m_pFront);
    BB_SAFE_DELETE(m_pBack);
    BB_SAFE_DELETE(m_pLeft);
    BB_SAFE_DELETE(m_pRight);
    BB_SAFE_DELETE(m_pTop);
    BB_SAFE_DELETE(m_pBottom);
}

void BBSkyBox::init(const QString &path)
{
    m_pFront->init(path + "front");
    m_pBack->init(path + "back");
    m_pLeft->init(path + "left");
    m_pRight->init(path + "right");
    m_pTop->init(path + "top");
    m_pBottom->init(path + "bottom");
}

void BBSkyBox::render(BBCamera *pCamera)
{
    m_pFront->render(pCamera);
    m_pBack->render(pCamera);
    m_pLeft->render(pCamera);
    m_pRight->render(pCamera);
    m_pTop->render(pCamera);
    m_pBottom->render(pCamera);
}

void BBSkyBox::resize(float fWidth, float fHeight)
{
    m_pFront->resize(fWidth, fHeight);
    m_pBack->resize(fWidth, fHeight);
    m_pLeft->resize(fWidth, fHeight);
    m_pRight->resize(fWidth, fHeight);
    m_pTop->resize(fWidth, fHeight);
    m_pBottom->resize(fWidth, fHeight);
}

void BBSkyBox::initFront()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pFront = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initBack()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pBack = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initLeft()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pLeft = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initRight()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pRight = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initTop()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pTop = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initBottom()
{
    BBGLVertexBuffer *pVertexBuffer = new BBGLVertexBuffer(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    m_pBottom = new BBSkyBoxSide(pVertexBuffer);
}

//void SkyElement::change(QString path)
//{
//    setTexture(path);
//}

//void Skybox::change(QString path)
//{
//    front->change(path + "front");
//    back->change(path + "back");
//    left->change(path + "left");
//    right->change(path + "right");
//    top->change(path + "top");
//    bottom->change(path + "bottom");
//}
