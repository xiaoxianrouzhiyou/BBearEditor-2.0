#include "BBMesh.h"
#include "Render/BBMaterial.h"
#include "Utils/BBUtils.h"
#include "Geometry/BBBoundingBox.h"
#include <cfloat>
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BufferObject/BBElementBufferObject.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/Texture/BBTexture.h"
#include "Math/BBMath.h"


BBMesh::BBMesh()
    : BBMesh(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBMesh::BBMesh(float px, float py, float pz,
               float rx, float ry, float rz,
               float sx, float sy, float sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    setClassName(BB_CLASSNAME_MESH);
    m_eDrawPrimitiveType = GL_QUADS;
    m_pEBO2 = nullptr;
    m_pIndexes2 = nullptr;
    m_nIndexCount2 = 0;
}

void BBMesh::init(const QString &path, BBBoundingBox3D *&pOutBoundingBox)
{
    QList<QVector4D> positions;
    load(path, positions);
    m_pVBO->computeTangent(m_pIndexes, m_nIndexCount);

    // create bounding box
    pOutBoundingBox = new BBAABBBoundingBox3D(m_Position.x(), m_Position.y(), m_Position.z(),
                                              m_Rotation.x(), m_Rotation.y(), m_Rotation.z(),
                                              m_Scale.x(), m_Scale.y(), m_Scale.z(),
                                              positions);
    pOutBoundingBox->init();

    m_pCurrentMaterial->initMultiPass("standard", BB_PATH_RESOURCE_SHADER(standard.vert), BB_PATH_RESOURCE_SHADER(standard.frag));
    m_pCurrentMaterial->getAdditiveRenderPass()->setBlendState(true);
    // default
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);
    BBTexture texture;
    m_pCurrentMaterial->setSampler2D(LOCATION_TEXTURE(0), texture.createTexture2D());
    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_TRIANGLES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);

    if (m_nIndexCount2 > 0)
    {
        m_pEBO2 = new BBElementBufferObject(m_nIndexCount2);
        m_pEBO2->submitData(m_pIndexes2, m_nIndexCount2);

        BBDrawCall *pDrawCall2 = new BBDrawCall;
        pDrawCall2->setMaterial(m_pCurrentMaterial);
        pDrawCall2->setVBO(m_pVBO);
        pDrawCall2->setEBO(m_pEBO2, GL_QUADS, m_nIndexCount2, 0);
        appendDrawCall(pDrawCall2);
    }
}

void BBMesh::init(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount, BBBoundingBox3D *&pOutBoundingBox)
{
    m_pVBO = pVBO;
    // create bounding box
    pOutBoundingBox = new BBAABBBoundingBox3D(m_Position.x(), m_Position.y(), m_Position.z(),
                                              m_Rotation.x(), m_Rotation.y(), m_Rotation.z(),
                                              m_Scale.x(), m_Scale.y(), m_Scale.z(),
                                              pVBO->getPositions());
    pOutBoundingBox->init();

    m_pCurrentMaterial->init("base", BB_PATH_RESOURCE_SHADER(base.vert), BB_PATH_RESOURCE_SHADER(base.frag));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, eDrawPrimitiveType, nDrawStartIndex, nDrawCount);
    appendDrawCall(pDrawCall);
}

bool BBMesh::hit(const BBRay &ray, float &fDistance)
{
    QVector3D intersection;
    bool bResult = false;
    fDistance = FLT_MAX;
    for (int i = 0; i < m_nIndexCount; i += 3)
    {
        if (ray.computeIntersectWithTriangle(m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i]),
                                             m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i + 1]),
                                             m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i + 2]),
                                             intersection))
        {
            float temp = ray.computeIntersectDistance(intersection);
            if (temp < fDistance)
                fDistance = temp;
            bResult = true;
        }
    }
    return bResult;
}

bool BBMesh::hit(const BBRay &ray, float fMinDistance, float fMaxDistance, BBHitInfo &hitInfo)
{
    QVector3D intersectionNormal;
    QVector3D intersectionPos;
    float fIntersectionU;
    float fIntersectionV;
    float fNearestIntersectionU;
    float fNearestIntersectionV;
    hitInfo.m_fDistance = fMaxDistance;
    int nNearestStartIndex = -1;
    bool bResult = false;

    for (int i = 0; i < m_nIndexCount; i += 3)
    {
        if (ray.computeIntersectWithTriangle(m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i]),
                                             m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i + 1]),
                                             m_ModelMatrix * m_pVBO->getPosition(m_pIndexes[i + 2]),
                                             intersectionNormal, intersectionPos, fIntersectionU, fIntersectionV))
        {
            float temp = ray.computeIntersectDistance(intersectionPos);
            if (temp < hitInfo.m_fDistance && temp > fMinDistance)
            {
                // Record the information of the nearest intersection point and face
                hitInfo.m_Position = intersectionPos;
                hitInfo.m_Normal = intersectionNormal;
                fNearestIntersectionU = fIntersectionU;
                fNearestIntersectionV = fIntersectionV;
                hitInfo.m_fDistance = temp;
                nNearestStartIndex = i;
                bResult = true;
            }
        }
    }

    if (bResult)
    {
        hitInfo.m_Texcoords = lerp(m_pVBO->getTexcoord(m_pIndexes[nNearestStartIndex]),
                                   m_pVBO->getTexcoord(m_pIndexes[nNearestStartIndex + 1]),
                                   m_pVBO->getTexcoord(m_pIndexes[nNearestStartIndex + 2]),
                                   fNearestIntersectionU, fNearestIntersectionV);
    }

    return bResult;
}


//void BBMesh::draw()
//{
//    if (mMeshType == MeshType::fbx)
//    {
//        //动画
//        mFbx->update();
//        mVertexPositions = mFbx->getVertexPositions();
//        for (int i = 0; i < mVertexCount; i++)
//        {
//            mVertexBuffer->setPosition(i, mVertexPositions.at(i));
//            mVertexBuffer->setColor(i, mColor);
//        }
//    }
//}

//void Mesh::loadFbx(QString path)
//{
//    mFbx = new FBX(path);
//    mFbx->load();
//    mIndexes = mFbx->getIndexes();
//    mIndexCount = mFbx->getIndexCount();
//    //计算包围盒的数据
//    mVertexPositions = mFbx->getVertexPositions();
//    //顶点数据
//    QList<QVector2D> texcoords = mFbx->getVertexTexcoords();
//    QList<QVector4D> normals = mFbx->getVertexNormals();
//    //shader buffer
//    mVertexCount = mVertexPositions.count();
//    mVertexBuffer = new VertexBuffer();
//    mVertexBuffer->setSize(mVertexCount);
//    for (int i = 0; i < mVertexCount; i++)
//    {
//        mVertexBuffer->setPosition(i, mVertexPositions.at(i));
//        mVertexBuffer->setTexcoord(i, texcoords.at(i));
//        mVertexBuffer->setNormal(i, normals.at(i));
//        mVertexBuffer->setColor(i, mColor);
//    }
//    mFbx->closeLoad();
//}
