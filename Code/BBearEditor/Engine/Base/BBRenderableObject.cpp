#include "BBRenderableObject.h"
#include "Render/BBMaterial.h"
#include "BBUtils.h"
#include "Render/BBCamera.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBGLBuffers.h"


BBRenderableObject::BBRenderableObject()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBRenderableObject::BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_bVisible = true;
    m_pMaterial = new BBMaterial;
    m_pVertexBuffer = NULL;
    m_pIndexes = NULL;
    m_nIndexCount = 0;
    m_nVertexCount = 0;
    m_DefaultColor = QVector3D(1.0f, 1.0f, 1.0f);
}

BBRenderableObject::~BBRenderableObject()
{
    BB_SAFE_DELETE(m_pMaterial);
    BB_SAFE_DELETE(m_pVertexBuffer);
    BB_SAFE_DELETE(m_pIndexes);
}

void BBRenderableObject::render(BBCamera *pCamera)
{
    render(m_ModelMatrix, pCamera);
}

void BBRenderableObject::render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera)
{
    if (m_bVisible)
    {
        m_pVertexBuffer->bind();

        // test
        BBUniformUpdater *pUniformUpdater = m_pMaterial->getUniforms();
        while (pUniformUpdater != nullptr)
        {
            if (pUniformUpdater->getPropertyType() == BBMaterialUniformPropertyType::Matrix4)
            {
                pUniformUpdater->setData(modelMatrix.data());
                break;
            }
            pUniformUpdater = pUniformUpdater->next<BBUniformUpdater>();
        }

        m_pMaterial->bind(pCamera);
        draw();
        m_pVertexBuffer->unbind();
    }
}

void BBRenderableObject::setTexture(const QString &filePath, bool bInvertY)
{
//    m_pShader->setTexture(NAME_TEXTURE, filePath, bInvertY);
}

void BBRenderableObject::setTexture(int nSize)
{
//    m_pShader->setTexture(NAME_TEXTURE, nSize);
}

void BBRenderableObject::setTexture(const GLuint &nTexture)
{
//    m_pShader->setTexture(NAME_TEXTURE, nTexture);
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
