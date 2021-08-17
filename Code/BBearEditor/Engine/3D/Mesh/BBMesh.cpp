#include "BBMesh.h"
#include "Render/BBMaterial.h"
#include "Utils/BBUtils.h"
#include "Geometry/BBBoundingBox.h"
#include <cfloat>
#include "Render/BBVertexBufferObject.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Render/Texture/BBTexture.h"


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
}

void BBMesh::init(const QString &path, BBBoundingBox3D *&pOutBoundingBox)
{
    QList<QVector4D> positions;
    load(path, positions);
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

    m_pVBO->computeTangent(m_pIndexes, m_nIndexCount);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, m_eDrawPrimitiveType, m_nIndexCount, 0);
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
