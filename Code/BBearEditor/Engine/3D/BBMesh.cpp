#include "BBMesh.h"
#include "Render/BBGLShader.h"
#include "BBUtils.h"
#include "Geometry/BBBoundingBox.h"
#include <cfloat>
#include "Render/BBGLBuffers.h"

BBMesh::BBMesh()
    : BBMesh(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBMesh::BBMesh(float px, float py, float pz,
               float rx, float ry, float rz,
               float sx, float sy, float sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{

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

    // test
    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "base.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "base.frag", m_pIndexes, m_nIndexCount);
//    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "standard.vert",
//                    QString(BB_PATH_RESOURCE_SHADER) + "standard.frag", m_pIndexes, m_nIndexCount);
//    mShader.setVector4f(Light::lightAmbientUniformLocationName, 1.0f, 1.0f, 1.0f, 1.0f);
//    //环境光无需考虑光的位置 而漫反射光需要 齐次坐标为0 无穷远处
//    QVector4D *lightPosition = new QVector4D[1];
//    lightPosition[0] = QVector4D(5.0f, 5.0f, 1.0f, 0.0f);
//    updateDirectionLightPosition(lightPosition, QVector3D(1, 0, 0));
//    mShader.setVector4f(Light::directionLightColorUniformLocationName, 1.0f, 1.0f, 1.0f, 1.0f);
//    mShader.setVector4f(Light::lightSpecularUniformLocationName, 1.0f, 1.0f, 1.0f, 1.0f);
//    mShader.setVector4f(Light::lightOptionUniformLocationName, 32.0f, 0.0f, 0.0f, 0.0f);
//    setDefaultMaterial();
}

bool BBMesh::hit(const BBRay &ray, float &fDistance)
{
    QVector3D intersection;
    bool bResult = false;
    fDistance = FLT_MAX;
    for (int i = 0; i < m_nIndexCount; i += 3)
    {
        if (ray.computeIntersectWithTriangle(m_ModelMatrix * m_pVertexBuffer->getPosition(m_pIndexes[i]),
                                             m_ModelMatrix * m_pVertexBuffer->getPosition(m_pIndexes[i + 1]),
                                             m_ModelMatrix * m_pVertexBuffer->getPosition(m_pIndexes[i + 2]),
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

void BBMesh::draw()
{
//    if (mIsDepthTest)
//        glEnable(GL_DEPTH_TEST);
//    else
//        glDisable(GL_DEPTH_TEST);

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
    glDrawElements(GL_TRIANGLES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
}

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

//void Mesh::loadTerrain()
//{
//    mIndexes = mTerrain->getIndexes();
//    mIndexCount = mTerrain->getIndexCount();
//    //计算包围盒的数据
//    mVertexPositions = mTerrain->getVertexPositions();
//    //顶点数据
//    QList<QVector2D> texcoords = mTerrain->getVertexTexcoords();
//    QList<QVector4D> normals = mTerrain->getVertexNormals();
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
//    mTerrain->closeLoad();
//}

//void Mesh::setColor(QVector3D color)
//{
//    mColor = color;
//    for (int i = 0; i < mVertexCount; i++)
//        mVertexBuffer->setColor(i, mColor);
//}

//void Mesh::setIsDepthTest(bool isDepthTest)
//{
//    mIsDepthTest = isDepthTest;
//}



//void Mesh::setMaterial(Material *material)
//{
//    mShader.init(material->getVertexShaderPath(), material->getFragmentShaderPath(), mIndexes, mIndexCount);
//    setAmbientMaterial(material->getAmbientColor());
//    setDiffuseMaterial(material->getDiffuseColor());
//    setSpecularMaterial(material->getSpecularColor());
//    if (!material->getDiffuseMapPath().isEmpty())
//    {
//        setTexture(material->getDiffuseMapPath());
//    }
//    int triggerSpecular;
//    if (material->getSmoothness() < 5)
//    {
//        triggerSpecular = 0;
//    }
//    else
//    {
//        triggerSpecular = 1;
//    }
//    mShader.setVector4f(Light::lightOptionUniformLocationName, material->getSmoothness(), 0.0f, 0.0f, triggerSpecular);
//}

//void Mesh::setDefaultMaterial()
//{
//    setAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);
//    setDiffuseMaterial(0.6f, 0.6f, 0.6f, 1.0f);
//    setSpecularMaterial(0.9f, 0.9f, 0.9f, 1.0f);
//    setTexture(DefaultTexturePath);
//}

//void Mesh::updateDirectionLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mShader.setVector4f(Light::lightCountUniformLocationName, count.x(), count.y(), count.z(), 0);
//    mShader.setVector4fArray(Light::directionLightPositionUniformLocationName, lightPosition, count.x());
//}

//void Mesh::updateDirectionLightColor(QVector4D *lightColor, int count)
//{
//    mShader.setVector4fArray(Light::directionLightColorUniformLocationName, lightColor, count);
//}

//void Mesh::updatePointLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mShader.setVector4f(Light::lightCountUniformLocationName, count.x(), count.y(), count.z(), 0);
//    mShader.setVector4fArray(Light::pointLightPositionUniformLocationName, lightPosition, count.y());
//}

//void Mesh::updatePointLightColor(QVector4D *lightColor, int count)
//{
//    mShader.setVector4fArray(Light::pointLightColorUniformLocationName, lightColor, count);
//}

//void Mesh::updatePointLightOption(QVector4D *option, int count)
//{
//    mShader.setVector4fArray(Light::pointLightOptionUniformLocationName, option, count);
//}

//void Mesh::updateSpotLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mShader.setVector4f(Light::lightCountUniformLocationName, count.x(), count.y(), count.z(), 0);
//    mShader.setVector4fArray(Light::spotLightPositionUniformLocationName, lightPosition, count.z());
//}

//void Mesh::updateSpotLightDirection(QVector4D *lightDirection, int count)
//{
//    mShader.setVector4fArray(Light::spotLightDirectionUniformLocationName, lightDirection, count);
//}

//void Mesh::updateSpotLightColor(QVector4D *lightColor, int count)
//{
//    mShader.setVector4fArray(Light::spotLightColorUniformLocationName, lightColor, count);
//}

//void Mesh::updateSpotLightOption(QVector4D *option, int count)
//{
//    mShader.setVector4fArray(Light::spotLightOptionUniformLocationName, option, count);
//}

//void Mesh::updateSpotLightOption2(QVector4D *option, int count)
//{
//    mShader.setVector4fArray(Light::spotLightOption2UniformLocationName, option, count);
//}

//void Mesh::switchFog(bool b)
//{
//    mShader.setBool("fogSwitchUniform", b);
//}

//void Mesh::setFogColor(float r, float g, float b)
//{
//    mShader.setVector4f("fogColorUniform", r, g, b, 1.0f);
//}

//void Mesh::setFogOption(float start, float end, int density, float power)
//{
//    mShader.setVector4f("fogOptionUniform", start, end, density, power);
//}

//void Mesh::setFogMode(int mode)
//{
//    mShader.setVector4f("fogModeUniform", mode, 0.0f, 0.0f, 0.0f);
//}


//void Mesh::renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos)
//{
//    mShader.setMatrix("lightViewUniform", viewMatrix);
//    mShader.setMatrix("lightProjectionUniform", projectionMatrix);
//    mShader.render([this]()->void { draw();}, mModelMatrix, viewMatrix, projectionMatrix, cameraPos, mVertexBuffer);
//}

//void Mesh::setShadowMap(GLuint map)
//{
//    mShader.setTexture("shadowMapUniform", map);
//}

//void Mesh::changeTerrainHeightMap(QString path)
//{
//    if (mTerrain)
//    {
//        mTerrain->changeHeightMap(path);
//        loadTerrain();
//    }
//}
