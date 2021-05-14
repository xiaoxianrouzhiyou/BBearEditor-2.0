#include "BBSkyBox.h"
#include "BBGLBuffers.h"
#include "BBCamera.h"
#include "BBGLShader.h"

//--------------------
// BBSkyBoxSide
//--------------------

BBSkyBoxSide::BBSkyBoxSide(BBGLVertexBuffer *pVertexBuffer)
    : BBRenderableObject()
{
    m_pVertexBuffer = pVertexBuffer;
}

void BBSkyBoxSide::init(const QString path)
{
    m_pShader->init("../BBearEditor/resources/shaders/base.vert",
                    "../BBearEditor/resources/shaders/base.frag");
//    setTexture(path);
}

void BBSkyBoxSide::render(BBCamera *pCamera)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pCamera->getPosition());
    BBRenderableObject::render(pCamera);
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
{

}


//void SkyElement::change(QString path)
//{
//    setTexture(path);
//}

//Skybox::Skybox()
//{
//    initFront();
//    initBack();
//    initLeft();
//    initRight();
//    initTop();
//    initBottom();
//}

//void Skybox::init(QString path)
//{
//    front->init(path + "front");
//    back->init(path + "back");
//    left->init(path + "left");
//    right->init(path + "right");
//    top->init(path + "top");
//    bottom->init(path + "bottom");
//}

//void Skybox::render(Camera camera)
//{
//    front->render(camera);
//    back->render(camera);
//    left->render(camera);
//    right->render(camera);
//    top->render(camera);
//    bottom->render(camera);
//}

//void Skybox::resize(float width, float height)
//{
//    front->resize(width, height);
//    back->resize(width, height);
//    left->resize(width, height);
//    right->resize(width, height);
//    top->resize(width, height);
//    bottom->resize(width, height);
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

//void Skybox::initFront()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, -0.5f, -0.5f, -0.5f);
//    vertexBuffer->setPosition(1, 0.5f, -0.5f, -0.5f);
//    vertexBuffer->setPosition(2, -0.5f, 0.5f, -0.5f);
//    vertexBuffer->setPosition(3, 0.5f, 0.5f, -0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    front = new SkyElement(vertexBuffer);
//}

//void Skybox::initBack()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, 0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(1, -0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(2, 0.5f, 0.5f, 0.5f);
//    vertexBuffer->setPosition(3, -0.5f, 0.5f, 0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    back = new SkyElement(vertexBuffer);
//}

//void Skybox::initLeft()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(1, -0.5f, -0.5f, -0.5f);
//    vertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
//    vertexBuffer->setPosition(3, -0.5f, 0.5f, -0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    left = new SkyElement(vertexBuffer);
//}

//void Skybox::initRight()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, 0.5f, -0.5f, -0.5f);
//    vertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(2, 0.5f, 0.5f, -0.5f);
//    vertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    right = new SkyElement(vertexBuffer);
//}

//void Skybox::initTop()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, -0.5f, 0.5f, -0.5f);
//    vertexBuffer->setPosition(1, 0.5f, 0.5f, -0.5f);
//    vertexBuffer->setPosition(2, -0.5f, 0.5f, 0.5f);
//    vertexBuffer->setPosition(3, 0.5f, 0.5f, 0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    top = new SkyElement(vertexBuffer);
//}

//void Skybox::initBottom()
//{
//    VertexBuffer *vertexBuffer = new VertexBuffer();
//    vertexBuffer->setSize(4);
//    vertexBuffer->setPosition(0, -0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(1, 0.5f, -0.5f, 0.5f);
//    vertexBuffer->setPosition(2, -0.5f, -0.5f, -0.5f);
//    vertexBuffer->setPosition(3, 0.5f, -0.5f, -0.5f);
//    vertexBuffer->setTexcoord(0, 0.0f, 0.0f);
//    vertexBuffer->setTexcoord(1, 1.0f, 0.0f);
//    vertexBuffer->setTexcoord(2, 0.0f, 1.0f);
//    vertexBuffer->setTexcoord(3, 1.0f, 1.0f);
//    bottom = new SkyElement(vertexBuffer);
//}

