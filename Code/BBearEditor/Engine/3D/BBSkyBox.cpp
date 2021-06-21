#include "BBSkyBox.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBCamera.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBTexture.h"
#include "BBUtils.h"

//--------------------
// BBSkyBoxSide
//--------------------

BBSkyBoxSide::BBSkyBoxSide(BBVertexBufferObject *pVBO)
    : BBRenderableObject()
{
    m_pVBO = pVBO;
}

void BBSkyBoxSide::init(const QString &path)
{
    m_pMaterial->init(BB_PATH_RESOURCE_SHADER(texture.vert),
                      BB_PATH_RESOURCE_SHADER(texture.frag));
    BBTexture texture;
    m_pMaterial->getBaseRenderPass()->setSampler2D(NAME_TEXTURE,
                                                   texture.createTexture2DFromBMP(path.toStdString().c_str()));
    m_pMaterial->getBaseRenderPass()->setZTestState(true);
    m_pVBO->setDrawParameter(GL_TRIANGLE_STRIP, 0, 4);

    BBRenderableObject::init();
}

void BBSkyBoxSide::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    BBRenderableObject::render(modelMatrix, pCamera);
}

void BBSkyBoxSide::change(const QString &path)
{

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
    m_pFront->init(path + "front.bmp");
    m_pBack->init(path + "back.bmp");
    m_pLeft->init(path + "left.bmp");
    m_pRight->init(path + "right.bmp");
    m_pTop->init(path + "top.bmp");
    m_pBottom->init(path + "bottom.bmp");
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

void BBSkyBox::change(const QString &path)
{
    m_pFront->change(path + "front");
    m_pBack->change(path + "back");
    m_pLeft->change(path + "left");
    m_pRight->change(path + "right");
    m_pTop->change(path + "top");
    m_pBottom->change(path + "bottom");
}

void BBSkyBox::initFront()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pFront = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initBack()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pBack = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initLeft()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pLeft = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initRight()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pRight = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initTop()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, -0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(1, 0.5f, 0.5f, -0.5f);
    pVertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
    pVertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pTop = new BBSkyBoxSide(pVertexBuffer);
}

void BBSkyBox::initBottom()
{
    BBVertexBufferObject *pVertexBuffer = new BBVertexBufferObject(4);
    pVertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
    pVertexBuffer->setPosition(2, -0.5f, -0.5f, -0.5f);
    pVertexBuffer->setPosition(3, 0.5f, -0.5f, -0.5f);
    pVertexBuffer->setTexcoord(0, 0.0f, 0.0f);
    pVertexBuffer->setTexcoord(1, 1.0f, 0.0f);
    pVertexBuffer->setTexcoord(2, 0.0f, 1.0f);
    pVertexBuffer->setTexcoord(3, 1.0f, 1.0f);
    pVertexBuffer->submitData();
    m_pBottom = new BBSkyBoxSide(pVertexBuffer);
}
