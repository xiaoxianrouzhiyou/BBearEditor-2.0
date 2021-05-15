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
    m_pShader = new BBGLShader;
    m_pVertexBuffer = NULL;
    m_pIndexes = NULL;
    m_nIndexCount = 0;
}

BBRenderableObject::~BBRenderableObject()
{
    BB_SAFE_DELETE(m_pShader);
    BB_SAFE_DELETE(m_pVertexBuffer);
    BB_SAFE_DELETE(m_pIndexes);
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

void BBRenderableObject::setTexture(const QString filePath, const bool bInvertY)
{
    m_pShader->setTexture(NAME_TEXTURE, filePath, bInvertY);
}

void BBRenderableObject::setTexture(const int nSize)
{
    m_pShader->setTexture(NAME_TEXTURE, nSize);
}

void BBRenderableObject::setTexture(const GLuint nTexture)
{
    m_pShader->setTexture(NAME_TEXTURE, nTexture);
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
