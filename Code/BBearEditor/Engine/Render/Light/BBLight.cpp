#include "BBLight.h"
#include "Utils/BBUtils.h"
#include "3D/BBIcon.h"
#include "Render/BBCamera.h"
#include "3D/BBLightIndicator.h"
#include "Render/BBRenderPass.h"


BBLight::BBLight(BBScene *pScene)
    : BBLight(pScene, QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{

}

BBLight::BBLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBGameObject(position, rotation, scale)
{
    m_eType = Directional;
    m_pScene = pScene;
    // no need to rotate
    m_pIcon = new BBIcon(position, QVector3D(0, 0, 0), scale);
    // default
    setDiffuseColor(0.976f, 0.804f, 0.678f, 1.0f);
    setHomogeneousPosition(position, 1.0f);

    // m_Setting0[0] : there is a light
    setSetting0(1.0f, 0.0f, 0.0f, 0.0f);
}

BBLight::~BBLight()
{
    BB_SAFE_DELETE(m_pIcon);
    BB_SAFE_DELETE(m_pIndicator);
}

void BBLight::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pIcon->setPosition(position, bUpdateLocalTransform);
    m_pIndicator->setPosition(position, bUpdateLocalTransform);
    setHomogeneousPosition(position, 1.0f);
}

void BBLight::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pIndicator->setRotation(nAngle, axis, bUpdateLocalTransform);
}

void BBLight::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBGameObject::setRotation(rotation, bUpdateLocalTransform);
    m_pIndicator->setRotation(rotation, bUpdateLocalTransform);
}

void BBLight::setVisibility(bool bVisible)
{
    BBGameObject::setVisibility(bVisible);
    m_pIndicator->setVisibility(bVisible);
}

void BBLight::init(const QString &path)
{
    BBGameObject::init(path);
    m_pIcon->init(BB_PATH_RESOURCE_ICON() + path.split('.')[0] + " white.png");
    m_pIndicator->init();
}

void BBLight::render(BBCamera *pCamera)
{
    // face camera
    QVector3D dir = pCamera->getPosition() - pCamera->getViewCenter();
    QQuaternion rot = QQuaternion::fromDirection(dir, QVector3D(0, 1, 0));
    m_pIcon->setRotation(rot, false);

    m_pIcon->render(pCamera);
    m_pIndicator->render(pCamera);
}

void BBLight::insertInRenderQueue(BBRenderQueue *pQueue)
{
    m_pIcon->insertInRenderQueue(pQueue);
}

void BBLight::removeFromRenderQueue(BBRenderQueue *pQueue)
{
    m_pIcon->removeFromRenderQueue(pQueue);
}

bool BBLight::hit(const BBRay &ray, float &fDistance)
{
    return m_pIcon->hit(ray, fDistance);
}

bool BBLight::belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                      const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                      const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                      const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3)
{
    return m_pIcon->belongToSelectionRegion(left1, left2, left3,
                                            top1, top2, top3,
                                            right1, right2, right3,
                                            bottom1, bottom2, bottom3);
}

void BBLight::setRenderPass(BBRenderPass *pRenderPass)
{
    pRenderPass->setVector4(LOCATION_LIGHT_POSITION, m_HomogeneousPosition);
    pRenderPass->setVector4(LOCATION_LIGHT_COLOR, m_Diffuse);
    pRenderPass->setVector4(LOCATION_LIGHT_SETTINGS(0), m_Setting0);
    pRenderPass->setVector4(LOCATION_LIGHT_SETTINGS(1), m_Setting1);
}

void BBLight::setAmbientColor(float r, float g, float b, float a)
{
    m_Ambient[0] = r;
    m_Ambient[1] = g;
    m_Ambient[2] = b;
    m_Ambient[3] = a;
}

void BBLight::setDiffuseColor(float r, float g, float b, float a)
{
    m_Diffuse[0] = r;
    m_Diffuse[1] = g;
    m_Diffuse[2] = b;
    m_Diffuse[3] = a;
}

void BBLight::setSpecularColor(float r, float g, float b, float a)
{
    m_Specular[0] = r;
    m_Specular[1] = g;
    m_Specular[2] = b;
    m_Specular[3] = a;
}

void BBLight::setSetting0(float x, float y, float z, float w)
{
    m_Setting0[0] = x;
    m_Setting0[1] = y;
    m_Setting0[2] = z;
    m_Setting0[3] = w;
}

void BBLight::setSetting1(float x, float y, float z, float w)
{
    m_Setting1[0] = x;
    m_Setting1[1] = y;
    m_Setting1[2] = z;
    m_Setting1[3] = w;
}

void BBLight::setHomogeneousPosition(const QVector3D &value, float w)
{
    m_HomogeneousPosition[0] = value.x();
    m_HomogeneousPosition[1] = value.y();
    m_HomogeneousPosition[2] = value.z();
    m_HomogeneousPosition[3] = w;
}



//void SpotLightIndicator::init()
//{
//    mVertexBuffer = new VertexBuffer();
//    mVertexBuffer->setSize(25);
//    for (int i = 0; i < 24; i++)
//    {
//        float c = 0.267949f * cosf(0.261799f * i);
//        float s = 0.267949f * sinf(0.261799f * i);
//        mVertexBuffer->setPosition(i, c, -1.0f, s);
//        mVertexBuffer->setColor(i, 0.909804f, 0.337255f, 0.333333f);
//    }
//    mVertexBuffer->setPosition(24, 0.0f, 0.0f, 0.0f);
//    mVertexBuffer->setColor(24, 0.909804f, 0.337255f, 0.333333f);

//    mIndexCount = 56;
//    mIndexes = new unsigned short[mIndexCount];
//    for (int i = 0; i < 24; i++)
//    {
//        mIndexes[2 * i] = i;
//        mIndexes[2 * i + 1] = i + 1;
//    }
//    mIndexes[47] = 0;
//    for (int i = 0; i < 4; i++)
//    {
//        mIndexes[2 * i + 48] = 24;
//        mIndexes[2 * i + 49] = 6 * i;
//    }

//    mShader.init("../../../../BBearEngine/resources/shaders/base.vert",
//                 "../../../../BBearEngine/resources/shaders/base.frag", mIndexes, mIndexCount);
//}

//void SpotLightIndicator::setSpotAngle(float angle)
//{
//    //angle是cutoff * 2
//    float radius = tan(angle / 2 / 180 * 3.14);
//    for (int i = 0; i < 24; i++)
//    {
//        float c = radius * cosf(0.261799f * i);
//        float s = radius * sinf(0.261799f * i);
//        mVertexBuffer->setPosition(i, c, -1.0f, s);
//    }
//}

//void SpotLightIndicator::draw()
//{
//    glEnable(GL_DEPTH_TEST);
//    glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, 0);
//}


//SpotLight::SpotLight(Scene *scene, float px, float py, float pz,
//                     float rx, float ry, float rz, float sx, float sy, float sz)
//    : PointLight(scene, px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    mIndicator = new SpotLightIndicator(px, py, pz, rx, ry, rz, 1, 1, 1);
//    //初始聚光灯向下照射
//    lightDirection = QVector4D(0.0f, -1.0f, 0.0f, 1.0f);
//    //cutoff的2倍
//    spotAngle = 30.0f;
//    spotLevel = 48;
//    mIntensity = 2;
//    updateValidRange();
//}


//QVector4D SpotLight::getOption2Vector4f()
//{
//    return QVector4D(spotLevel, mIntensity, 0.0f, 0.0f);
//}

//QVector4D SpotLight::getLightDirection()
//{
//    return lightDirection;
//}

//float SpotLight::getSpotAngle()
//{
//    return spotAngle;
//}

//void SpotLight::setSpotAngle(float angle)
//{
//    spotAngle = angle;
//    //指示器变换角度
//    SpotLightIndicator *indicator = (SpotLightIndicator*)mIndicator;
//    indicator->setSpotAngle(angle);
//    mScene->updateSpotLightOption();
//}

//int SpotLight::getSpotLevel()
//{
//    return spotLevel;
//}

//void SpotLight::setSpotLevel(int level)
//{
//    spotLevel = level;
//    mScene->updateSpotLightOption2();
//}

//void SpotLight::setIntensity(int intensity)
//{
//    mIntensity = intensity;
//    mScene->updateSpotLightOption2();
//}

//void SpotLight::setPosition(QVector3D position, bool isUpdateLocalTransform)
//{
//    Light::setPosition(position, isUpdateLocalTransform);
//    mScene->updateSpotLightPosition();
//}

//void SpotLight::setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform)
//{
//    Light::setRotation(angle, axis, isUpdateLocalTransform);
//    mScene->updateSpotLightDirection();
//    setLightDirection();
//}

//void SpotLight::setRotation(QVector3D rotation, bool isUpdateLocalTransform)
//{
//    Light::setRotation(rotation, isUpdateLocalTransform);
//    mScene->updateSpotLightDirection();
//    setLightDirection();
//}

//void SpotLight::setLightDirection()
//{
//    //旋转矩阵
//    QMatrix4x4 matrix;
//    matrix.rotate(mQuaternion);
//    //转化为方向向量 不旋转时方向默认向下
//    lightDirection = matrix * QVector4D(0.0f, -1.0f, 0.0f, 1.0f);
//}

//void SpotLight::setColor(int r, int g, int b)
//{
//    Light::setColor(r, g, b);
//    mScene->updateSpotLightColor();
//}
