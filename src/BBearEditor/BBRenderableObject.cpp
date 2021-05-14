#include "BBRenderableObject.h"
#include "BBGLShader.h"
#include "BBUtils.h"
#include "BBCamera.h"
#include "BBGLBuffers.h"

BBRenderableObject::BBRenderableObject()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBRenderableObject::BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_bVisible = true;
    m_pVertexBuffer = NULL;
}

BBRenderableObject::~BBRenderableObject()
{
    BB_SAFE_DELETE(m_pShader);
    BB_SAFE_DELETE(m_pVertexBuffer);
}

void BBRenderableObject::render(BBCamera *pCamera)
{
    if (m_bVisible)
        m_pShader->render([this]()->void { draw(); },
                          m_ModelMatrix,
                          pCamera->getViewMatrix(),
                          pCamera->getPosition(),
                          m_pVertexBuffer);
}

void BBRenderableObject::render(QMatrix4x4 modelMatrix, BBCamera *pCamera)
{
    if (m_bVisible)
        m_pShader->render([this]()->void { draw(); },
                          modelMatrix,
                          pCamera->getViewMatrix(),
                          pCamera->getPosition(),
                          m_pVertexBuffer);
}

void BBRenderableObject::resize(float fWidth, float fHeight)
{
    m_pShader->resize(fWidth, fHeight);
}

void BBRenderableObject::draw()
{

}

//void RenderableObject::setAmbientMaterial(float r, float g, float b, float a)
//{
//    mShader.setVector4f("ambientMaterialUniform", r, g, b, a);
//}

//void RenderableObject::setAmbientMaterial(QColor color)
//{
//    setAmbientMaterial(color.redF(), color.greenF(), color.blueF(), color.alphaF());
//}

//void RenderableObject::setDiffuseMaterial(float r, float g, float b, float a)
//{
//    mShader.setVector4f("diffuseMaterialUniform", r, g, b, a);
//}

//void RenderableObject::setDiffuseMaterial(QColor color)
//{
//    setDiffuseMaterial(color.redF(), color.greenF(), color.blueF(), color.alphaF());
//}

//void RenderableObject::setSpecularMaterial(float r, float g, float b, float a)
//{
//    mShader.setVector4f("specularMaterialUniform", r, g, b, a);
//}

//void RenderableObject::setSpecularMaterial(QColor color)
//{
//    setSpecularMaterial(color.redF(), color.greenF(), color.blueF(), color.alphaF());
//}

//void RenderableObject::setTexture(QString imagePath, bool invertY)
//{
//    mShader.setTexture("textureUniform", imagePath, invertY);
//}

//void RenderableObject::setTexture(int size)
//{
//    mShader.setTexture("textureUniform", size);
//}

//void RenderableObject::setTexture(GLuint texture)
//{
//    mShader.setTexture("textureUniform", texture);
//}

