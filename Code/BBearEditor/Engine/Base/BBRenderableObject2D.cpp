#include "BBRenderableObject2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBMaterial.h"
#include "Scene/BBRendererManager.h"
#include "Render/BBDrawCall.h"


BBRenderableObject2D::BBRenderableObject2D()
    : BBRenderableObject2D(0, 0, 100, 100)
{

}

BBRenderableObject2D::BBRenderableObject2D(int x, int y, int nWidth, int nHeight)
    : BBRenderableObject(x, y, nWidth, nHeight)
{

}

BBRenderableObject2D::~BBRenderableObject2D()
{

}

void BBRenderableObject2D::init()
{
    m_pCurrentMaterial = BBRendererManager::createUIMaterial();
    BBRenderableObject::init();
}

void BBRenderableObject2D::render(BBCamera *pCamera)
{
    BBRenderableObject::render(pCamera);
}

void BBRenderableObject2D::render(BBCanvas *pCanvas)
{
    if (m_bActive && m_bVisible)
    {
        m_pDrawCalls->uiRendering(pCanvas);
    }
}
