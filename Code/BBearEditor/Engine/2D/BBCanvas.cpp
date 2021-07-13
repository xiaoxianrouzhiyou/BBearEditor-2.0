#include "BBCanvas.h"
#include "BBSprite2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBMaterial.h"


#define IterateSprite2DSet(x) for (QList<BBSprite2D*>::Iterator itr = m_Sprite2DSet.begin(); itr != m_Sprite2DSet.end(); itr++) {x;}


BBCanvas::BBCanvas()
    : BBCanvas(0, 0, 500, 500)
{

}

BBCanvas::BBCanvas(int x, int y, int nWidth, int nHeight)
    : BBGameObject(x, y, nWidth, nHeight)
{
//    m_pBoundingBox2D = new BBRectBoundingBox2D(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
}

BBCanvas::~BBCanvas()
{
    IterateSprite2DSet(delete *itr);
//    BB_SAFE_DELETE(m_pBoundingBox2D);
}

void BBCanvas::init()
{
    IterateSprite2DSet(init());
//    m_pBoundingBox2D->init();
}

void BBCanvas::render(BBCamera *pCamera)
{
    IterateSprite2DSet((*itr)->render(pCamera));
//    m_pBoundingBox2D->render(pCamera);
}

void BBCanvas::resize(float fWidth, float fHeight)
{
    IterateSprite2DSet((*itr)->getMaterial()->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f));
//    m_pBoundingBox2D->getMaterial()->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}

/**
 * @brief BBCanvas::setPosition             screen coordinate
 * @param position
 * @param bUpdateLocalTransform
 */
void BBCanvas::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setPosition(position, bUpdateLocalTransform));
//    m_pBoundingBox2D->setPosition(position, bUpdateLocalTransform);
}

void BBCanvas::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setRotation(nAngle, axis, bUpdateLocalTransform));
//    m_pBoundingBox2D->setRotation(nAngle, axis, bUpdateLocalTransform);
}

void BBCanvas::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(rotation, bUpdateLocalTransform);
    IterateSprite2DSet((*itr)->setRotation(rotation, bUpdateLocalTransform));
//    m_pBoundingBox2D->setRotation(rotation, bUpdateLocalTransform);
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

void BBCanvas::addSprite2D(BBSprite2D *pSprite2D)
{
    pSprite2D->bindCanvas(this);
    m_Sprite2DSet.append(pSprite2D);
}
