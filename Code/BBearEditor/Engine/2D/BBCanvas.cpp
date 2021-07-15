#include "BBCanvas.h"
#include "BBSprite2D.h"
#include "Geometry/BBBoundingBox2D.h"
#include "Render/BBMaterial.h"
#include "Scene/BBSceneManager.h"


#define IterateSprite2DSet(x) for (QList<BBSprite2D*>::Iterator itr = m_Sprite2DSet.begin(); itr != m_Sprite2DSet.end(); itr++) {x;}

BBCanvas::BBCanvas(int x, int y, int nWidth, int nHeight)
    : BBGameObject(x, y, nWidth, nHeight)
{
    m_pAABBBoundingBox2D = new BBAABBBoundingBox2D(x, y, nWidth / 2.0, nHeight / 2.0);
}

BBCanvas::~BBCanvas()
{
    BB_SAFE_DELETE(m_pAABBBoundingBox2D);
    IterateSprite2DSet(delete *itr);
}

void BBCanvas::init()
{
    m_pAABBBoundingBox2D->init();
    IterateSprite2DSet(init());
}

void BBCanvas::render(BBCamera *pCamera)
{
    m_pAABBBoundingBox2D->render(pCamera);
    IterateSprite2DSet((*itr)->render(pCamera));
}

void BBCanvas::resize(float fWidth, float fHeight)
{
    m_pAABBBoundingBox2D->getMaterial()->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}

/**
 * @brief BBCanvas::setPosition             screen coordinate
 * @param position
 * @param bUpdateLocalTransform
 */
void BBCanvas::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setPosition(position, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setPosition(position, bUpdateLocalTransform));
}

void BBCanvas::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    BBGameObject::setRotation(QVector3D(0, 0, m_Rotation.z()));
    m_pAABBBoundingBox2D->setRotation(nAngle, axis, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setRotation(nAngle, axis, bUpdateLocalTransform));
}

void BBCanvas::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(rotation, bUpdateLocalTransform);
    m_pAABBBoundingBox2D->setRotation(rotation, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setRotation(rotation, bUpdateLocalTransform));
}

void BBCanvas::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBGameObject::setScale(scale, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setScale(scale, bUpdateLocalTransform));
//    m_pBoundingBox2D->setScale(scale, bUpdateLocalTransform);
}

void BBCanvas::setActivity(bool bActive)
{
    IterateSprite2DSet((*itr)->setActivity(bActive));
//    m_pBoundingBox2D->setActivity(bActive);
    setVisibility(bActive);
}

void BBCanvas::setVisibility(bool bVisible)
{
//    m_pBoundingBox2D->setVisibility(bVisible);
}

bool BBCanvas::hit(int x, int y)
{
    return m_pAABBBoundingBox2D->hit(x, y);
}

void BBCanvas::addSprite2D(BBSprite2D *pSprite2D)
{
    m_Sprite2DSet.append(pSprite2D);
}
