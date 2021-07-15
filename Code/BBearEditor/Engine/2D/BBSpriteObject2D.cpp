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
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pSprite2D->setPosition(position, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setPosition(position, bUpdateLocalTransform);
}

void BBSpriteObject2D::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pSprite2D->setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setRotation(nAngle, axis, bUpdateLocalTransform);
}

void BBSpriteObject2D::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(rotation, bUpdateLocalTransform);
    m_pSprite2D->setRotation(rotation, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setRotation(rotation, bUpdateLocalTransform);
}

void BBSpriteObject2D::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBGameObject::setScale(scale, bUpdateLocalTransform);
    m_pSprite2D->setScale(scale, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setScale(scale, bUpdateLocalTransform);
}
