#include "BBModel.h"
#include "BBStaticMesh.h"
#include "Utils/BBUtils.h"
#include "Geometry/BBBoundingBox.h"
#include <cfloat>


using namespace std;

BBModel::BBModel(const BBMeshType &eType)
    : BBModel(0, 0, 0, 0, 0, 0, 1, 1, 1, eType)
{

}

BBModel::BBModel(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale, const BBMeshType &eType)
    : BBModel(position.x(), position.y(), position.z(),
              rotation.x(), rotation.y(), rotation.z(),
                 scale.x(),    scale.y(),    scale.z(), eType)
{

}

BBModel::BBModel(float px, float py, float pz,
                 float rx, float ry, float rz,
                 float sx, float sy, float sz, const BBMeshType &eType)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    setClassName(BB_CLASSNAME_MODEL);
    if (eType == BBMeshType::OBJ)
        m_pMesh = new BBOBJMesh(px, py, pz, rx, ry, rz, sx, sy, sz);
    m_pBoundingBox = NULL;
}

BBModel::~BBModel()
{
    BB_SAFE_DELETE(m_pMesh);
    BB_SAFE_DELETE(m_pBoundingBox);
}

void BBModel::init(const QString &path)
{
    BBGameObject::init(path);
    m_pMesh->init(path, m_pBoundingBox);
}

void BBModel::render(BBCamera *pCamera)
{
    if (m_bActive)
    {
        m_pMesh->render(pCamera);
        m_pBoundingBox->render(pCamera);
    }
}

void BBModel::setCurrentMaterial(BBMaterial *pMaterial)
{
    m_pMesh->setCurrentMaterial(pMaterial);
}

void BBModel::restoreMaterial()
{
    m_pMesh->restoreMaterial();
}

void BBModel::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pMesh->setPosition(position, bUpdateLocalTransform);
    m_pBoundingBox->setPosition(position, bUpdateLocalTransform);
}

void BBModel::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pMesh->setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pBoundingBox->setRotation(nAngle, axis, bUpdateLocalTransform);
}

void BBModel::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(rotation, bUpdateLocalTransform);
    m_pMesh->setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBox->setRotation(rotation, bUpdateLocalTransform);
}

void BBModel::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBGameObject::setScale(scale, bUpdateLocalTransform);
    m_pMesh->setScale(scale, bUpdateLocalTransform);
    m_pBoundingBox->setScale(scale, bUpdateLocalTransform);
}

void BBModel::setActivity(bool bActive)
{
    // Visibility and does not exist
    BBGameObject::setActivity(bActive);
    m_pMesh->setActivity(bActive);
    m_pBoundingBox->setActivity(bActive);
    setVisibility(bActive);
}

void BBModel::setVisibility(bool bVisible)
{
    // Visibility and exist
    BBGameObject::setVisibility(bVisible);
    m_pBoundingBox->setVisibility(bVisible);
}

bool BBModel::hit(const BBRay &ray, float &fDistance)
{
    // use bounding box for rough collision detection at first
    if (m_pBoundingBox->hit(ray, fDistance))
    {
        // After hitting the bounding box, judge whether it hits the mesh
        if (m_pMesh->hit(ray, fDistance))
        {
            return true;
        }
    }
    return false;
}

bool BBModel::belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                      const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                      const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                      const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3)
{
    // Eliminate objects whose the center point of the bounding box is on the outside
    QVector3D center = getModelMatrix() * m_pBoundingBox->getCenter();
    if (center.distanceToPlane(left1, left2, left3) < 0)
        return false;
    if (center.distanceToPlane(top1, top2, top3) < 0)
        return false;
    if (center.distanceToPlane(right1, right2, right3) < 0)
        return false;
    if (center.distanceToPlane(bottom1, bottom2, bottom3) < 0)
        return false;
    // If the center point of the bounding box is inside,
    // further calculate whether each vertex of the bounding box is inside
    return m_pBoundingBox->belongToSelectionRegion(left1, left2, left3,
                                                   top1, top2, top3,
                                                   right1, right2, right3,
                                                   bottom1, bottom2, bottom3);
}

void BBModel::showCloseUp(QVector3D &outPosition, QVector3D &outViewCenter, float fDistFactor)
{
    // The original bounding box is centered at the zero point and needs to be transformed
    outViewCenter = m_ModelMatrix * m_pBoundingBox->getCenter();
    QVector3D boxHalfLength = m_pBoundingBox->getHalfLength() * m_Scale;
    // Use the maximum half-length of the bounding box to become a cube bounding box
    float nSize = max(abs(boxHalfLength.x()), abs(boxHalfLength.y()));
    nSize = max(nSize, abs(boxHalfLength.z()));
    outPosition = outViewCenter + fDistFactor * nSize * QVector3D(1, 1, 1);
}
