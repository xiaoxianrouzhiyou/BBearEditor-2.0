#include "BBSpriteObject2D.h"
#include "2D/BBSprite2D.h"
#include "Geometry/BBBoundingBox2D.h"


BBSpriteObject2D::BBSpriteObject2D(int x, int y, int nWidth, int nHeight)
    : BBGameObject(x, y, nWidth, nHeight)
{
    m_pSprite2D = new BBSprite2D(x, y, nWidth, nHeight);
    m_pAABBBoundingBox2D = new BBAABBBoundingBox2D(x, y, nWidth, nHeight);
}

BBSpriteObject2D::~BBSpriteObject2D()
{
    BB_SAFE_DELETE(m_pSprite2D);
    BB_SAFE_DELETE(m_pAABBBoundingBox2D);
}

void BBSpriteObject2D::init()
{
    m_pSprite2D->init();
    m_pAABBBoundingBox2D->init();
}

void BBSpriteObject2D::render(BBCanvas *pCanvas)
{
    m_pSprite2D->render(pCanvas);
    m_pAABBBoundingBox2D->render(pCanvas);
}

void BBSpriteObject2D::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    m_pSprite2D->setPosition(position, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setPosition(position, bUpdateLocalTransform);
}
