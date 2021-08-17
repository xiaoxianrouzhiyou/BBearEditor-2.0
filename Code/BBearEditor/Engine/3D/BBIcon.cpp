#include "BBIcon.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/Texture/BBTexture.h"
#include "Render/BBDrawCall.h"


BBIcon::BBIcon()
    : BBIcon(QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{

}

BBIcon::BBIcon(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBRenderableObject(position, rotation, scale)
{
    m_pBoundingBox = new BBRectBoundingBox3D(0.0f, 0.0f, 0.0f, 0.25f, 0.25f, 0.0f);
    m_pBoundingBox->setPosition(position, false);
}

BBIcon::~BBIcon()
{
    BB_SAFE_DELETE(m_pBoundingBox);
}

void BBIcon::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBRenderableObject::setPosition(position, bUpdateLocalTransform);
    m_pBoundingBox->setPosition(position, bUpdateLocalTransform);
}

void BBIcon::setRotation(const QQuaternion &quaternion, bool bUpdateLocalTransform)
{
    BBRenderableObject::setRotation(quaternion, bUpdateLocalTransform);
    m_pBoundingBox->setRotation(quaternion, bUpdateLocalTransform);
}

void BBIcon::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBRenderableObject::setScale(scale, bUpdateLocalTransform);
    m_pBoundingBox->setScale(scale, bUpdateLocalTransform);
}

void BBIcon::init(const QString &path)
{
    m_pVBO = new BBVertexBufferObject(4);
    m_pVBO->setPosition(0, 0.25f, 0.25f, 0);
    m_pVBO->setPosition(1, -0.25f, 0.25f, 0);
    m_pVBO->setPosition(2, -0.25f, -0.25f, 0);
    m_pVBO->setPosition(3, 0.25f, -0.25f, 0);
    m_pVBO->setTexcoord(0, 1.0f, 1.0f);
    m_pVBO->setTexcoord(1, 0.0f, 1.0f);
    m_pVBO->setTexcoord(2, 0.0f, 0.0f);
    m_pVBO->setTexcoord(3, 1.0f, 0.0f);
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setColor(i, 1.0f, 1.0f, 1.0f);
    }

    m_pCurrentMaterial->init("texture",
                             BB_PATH_RESOURCE_SHADER(texture.vert),
                             BB_PATH_RESOURCE_SHADER(texture.frag));
    BBTexture texture;
    m_pCurrentMaterial->getBaseRenderPass()->setSampler2D(LOCATION_TEXTURE(0), texture.createTexture2D(path), path);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 4);
    appendDrawCall(pDrawCall);
}

bool BBIcon::hit(const BBRay &ray, float &fDistance)
{
    return m_pBoundingBox->hit(ray, fDistance);
}

bool BBIcon::belongToSelectionRegion(const BBFrustum &frustum)
{
    // Eliminate objects whose the center point of the bounding box is on the outside
    QVector3D center = getModelMatrix() * m_pBoundingBox->getCenter();
    return frustum.contain(center);
}
