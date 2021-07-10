#include "BBCanvas.h"
#include "BBSprite2D.h"


#define IterateSprite2DSet(x) for (QList<BBSprite2D*>::Iterator itr = m_Sprite2DSet.begin(); itr != m_Sprite2DSet.end(); itr++) {x;}


BBCanvas::BBCanvas()
    : BBCanvas(0, 0, 100, 100)
{

}

BBCanvas::BBCanvas(int nCenterX, int nCenterY, int nWidth, int nHeight)
    : BBGameObject(nCenterX, nCenterY, 0, 0, 0, 0, nWidth / 2.0f, nHeight / 2.0f, 0)
{
    m_nHalfWidth = nWidth / 2.0f;
    m_nHalfHeight = nHeight / 2.0f;
}

BBCanvas::~BBCanvas()
{
    IterateSprite2DSet(delete *itr);
}

void BBCanvas::init()
{
    IterateSprite2DSet(init());
}

void BBCanvas::render(BBCamera *pCamera)
{
    IterateSprite2DSet((*itr)->render(pCamera));
}

void BBCanvas::resize(float fWidth, float fHeight)
{
    IterateSprite2DSet((*itr)->resize(fWidth, fHeight));
}

void BBCanvas::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    IterateSprite2DSet((*itr)->setPosition(position, bUpdateLocalTransform));
}

void BBCanvas::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    IterateSprite2DSet((*itr)->setRotation(nAngle, axis, bUpdateLocalTransform));
}

void BBCanvas::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    IterateSprite2DSet((*itr)->setRotation(rotation, bUpdateLocalTransform));
}

void BBCanvas::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    IterateSprite2DSet((*itr)->setScale(scale, bUpdateLocalTransform));
}

void BBCanvas::setActivity(bool bActive)
{
    IterateSprite2DSet((*itr)->setActivity(bActive));
}

void BBCanvas::setVisibility(bool bVisible)
{
    IterateSprite2DSet((*itr)->setVisibility(bVisible));
}

void BBCanvas::addSprite2D(BBSprite2D *pSprite2D)
{
    pSprite2D->bindCanvas(this);
    m_Sprite2DSet.append(pSprite2D);
}
