#include "BBNormalIndicator.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBDrawCall.h"


BBNormalIndicator::BBNormalIndicator()
    : BBRenderableObject()
{
    m_bActive = false;
}

void BBNormalIndicator::init(BBRenderableObject *pObject)
{
    // when set new BBRenderableObject
    clear();

    setPosition(pObject->getPosition());
    setRotation(pObject->getRotation());
    setScale(pObject->getScale());

    BBVertexBufferObject *pObjectVBO = pObject->getVBO();
    int nObjectVertexCount = pObjectVBO->getVertexCount();

    // one normal two vertex
    m_pVBO = new BBVertexBufferObject(nObjectVertexCount * 2);
    for (int i = 0; i < nObjectVertexCount; i++)
    {
        QVector3D objectVertexPosition = pObjectVBO->getPosition(i);
        QVector3D objectVertexNormal = pObjectVBO->getNormal(i);
        objectVertexNormal.normalized();
        m_pVBO->setPosition(i * 2, objectVertexPosition);
        m_pVBO->setPosition(i * 2 + 1, objectVertexPosition + 0.1 * objectVertexNormal / pObject->getScale());
        m_pVBO->setColor(i * 2, 0.514f, 0.686f, 0.608f);
        m_pVBO->setColor(i * 2 + 1, 0.514f, 0.686f, 0.608f);
    }

    m_pCurrentMaterial = new BBMaterial();
    m_pCurrentMaterial->init("base", BB_PATH_RESOURCE_SHADER(base.vert), BB_PATH_RESOURCE_SHADER(base.frag));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_LINES, 0, m_pVBO->getVertexCount());
    appendDrawCall(pDrawCall);
}

void BBNormalIndicator::clear()
{
    BB_PROCESS_ERROR_RETURN(m_pVBO);
    BB_SAFE_DELETE(m_pVBO);
    BB_SAFE_DELETE(m_pEBO);
    BB_SAFE_DELETE(m_pCurrentMaterial);
    BB_SAFE_DELETE(m_pDrawCalls);
}
