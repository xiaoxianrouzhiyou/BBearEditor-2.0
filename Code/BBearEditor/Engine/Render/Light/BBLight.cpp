#include "BBLight.h"
#include "Utils/BBUtils.h"
#include "3D/BBIcon.h"
#include "Render/BBCamera.h"
#include "3D/BBLightIndicator.h"


BBLight* BBLight::m_pMainLight = nullptr;

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
}

BBLight::~BBLight()
{
    BB_SAFE_DELETE(m_pMainLight);
    BB_SAFE_DELETE(m_pIcon);
    BB_SAFE_DELETE(m_pIndicator);
}

void BBLight::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    m_pIcon->setPosition(position, bUpdateLocalTransform);
    m_pIndicator->setPosition(position, bUpdateLocalTransform);
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




//void Circle::init()
//{
//    mVertexBuffer = new VertexBuffer();
//    mVertexBuffer->setSize(48);
//    for (int i = 0; i < mVertexBuffer->mVertexCount; i++)
//    {
//        float c = cosf(0.1309f * i);
//        float s = sinf(0.1309f * i);
//        mVertexBuffer->setPosition(i, c, s, 0.0f);
//        mVertexBuffer->setColor(i, 0.909804f, 0.337255f, 0.333333f);
//    }

//    mIndexCount = 96;
//    mIndexes = new unsigned short[mIndexCount];
//    for (int i = 0; i < 48; i++)
//    {
//        mIndexes[2 * i] = i;
//        mIndexes[2 * i + 1] = i + 1;
//    }
//    mIndexes[95] = 0;

//    mShader.init("../../../../BBearEngine/resources/shaders/base.vert",
//                 "../../../../BBearEngine/resources/shaders/base.frag", mIndexes, mIndexCount);
//}

//void Circle::draw()
//{
//    glEnable(GL_DEPTH_TEST);
//    glLineWidth(2);
//    glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, 0);
//}


///*************************
// * PointLightIndicator
// * ************************/

//PointLightIndicator::PointLightIndicator()
//    : PointLightIndicator(0, 0, 0, 0, 0, 0, 1, 1, 1)
//{

//}

//PointLightIndicator::PointLightIndicator(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
//    : Indicator(px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    mCircle = new Circle(px, py, pz, rx, ry, rz, sx, sy, sz);
//}

//void PointLightIndicator::init()
//{
//    mVertexBuffer = new VertexBuffer();
//    mVertexBuffer->setSize(144);
//    for (int i = 0; i < 48; i++)
//    {
//        float c = cosf(0.1309f * i);
//        float s = sinf(0.1309f * i);

//        mVertexBuffer->setPosition(i, c, 0.0f, s);
//        mVertexBuffer->setNormal(i, c, 0.0f, s);
//        mVertexBuffer->setColor(i, 0.909804f, 0.337255f, 0.333333f);

//        mVertexBuffer->setPosition(i + 48, 0.0f, c, s);
//        mVertexBuffer->setNormal(i + 48, 0.0f, c, s);
//        mVertexBuffer->setColor(i + 48, 0.909804f, 0.337255f, 0.333333f);

//        mVertexBuffer->setPosition(i + 96, c, s, 0.0f);
//        mVertexBuffer->setNormal(i + 96, c, s, 0.0f);
//        mVertexBuffer->setColor(i + 96, 0.909804f, 0.337255f, 0.333333f);
//    }

//    mIndexCount = 288;
//    mIndexes = new unsigned short[mIndexCount];
//    for (int i = 0; i < 48; i++)
//    {
//        mIndexes[2 * i] = i;
//        mIndexes[2 * i + 1] = i + 1;
//        mIndexes[2 * i + 96] = i + 48;
//        mIndexes[2 * i + 97] = i + 49;
//        mIndexes[2 * i + 192] = i + 96;
//        mIndexes[2 * i + 193] = i + 97;
//    }
//    mIndexes[95] = 0;
//    mIndexes[191] = 48;
//    mIndexes[287] = 96;

//    mShader.init("../../../../BBearEngine/resources/shaders/standard.vert",
//                 "../../../../BBearEngine/resources/shaders/diffusewithcamera.frag", mIndexes, mIndexCount);

//    mCircle->init();
//}

//void PointLightIndicator::render(Camera camera)
//{
//    QMatrix4x4 modelMatrix;
//    modelMatrix.translate(mPosition);
//    modelMatrix.scale(mScale);
//    RenderableObject::render(modelMatrix, camera);
//    //跟着视点变化的轮廓圈
//    modelMatrix.rotate(QQuaternion::fromDirection(camera.pos - mPosition, QVector3D(0, 1, 0)));
//    mCircle->render(modelMatrix, camera);
//}

//void PointLightIndicator::resize(float width, float height)
//{
//    RenderableObject::resize(width, height);
//    mCircle->resize(width, height);
//}

//void PointLightIndicator::draw()
//{
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glLineWidth(2);
//    glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, 0);
//    glDisable(GL_BLEND);
//}

//void PointLightIndicator::setVisible(bool isVisible)
//{
//    RenderableObject::setVisible(isVisible);
//    mCircle->setVisible(isVisible);
//}

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
//    glLineWidth(2);
//    glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, 0);
//}

///*************************
// * Light
// * ************************/

//QString Light::directionLightPositionUniformLocationName = "directionLightPositionUniform";
//QString Light::directionLightColorUniformLocationName = "directionLightColorUniform";
//QString Light::pointLightPositionUniformLocationName = "pointLightPositionUniform";
//QString Light::pointLightColorUniformLocationName = "pointLightColorUniform";
//QString Light::pointLightOptionUniformLocationName = "pointLightOptionUniform";
//QString Light::spotLightPositionUniformLocationName = "spotLightPositionUniform";
//QString Light::spotLightDirectionUniformLocationName = "spotLightDirectionUniform";
//QString Light::spotLightColorUniformLocationName = "spotLightColorUniform";
//QString Light::spotLightOptionUniformLocationName = "spotLightOptionUniform";
//QString Light::spotLightOption2UniformLocationName = "spotLightOption2Uniform";
//QString Light::lightAmbientUniformLocationName = "lightAmbientUniform";
//QString Light::lightSpecularUniformLocationName = "lightSpecularUniform";
//QString Light::lightOptionUniformLocationName = "lightOptionUniform";
//QString Light::lightCountUniformLocationName = "lightCountUniform";

//Light::Light(Scene *scene, float px, float py, float pz,
//             float rx, float ry, float rz, float sx, float sy, float sz)
//    : GameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    //初始化为白光
//    setColor(255, 255, 255);
//}

//void Light::setColor(int r, int g, int b)
//{
//    mColor = QColor(r, g, b);
//    mColorVector4f = QVector4D(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
//}

//DirectionLight::DirectionLight(Scene *scene, float px, float py, float pz,
//                               float rx, float ry, float rz, float sx, float sy, float sz)
//    : Light(scene, px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    mIndicator = new DirectionLightIndicator(px, py, pz, rx, ry, rz, 1, 1, 1);

//    QMatrix4x4 matrix;
//    matrix.rotate(mQuaternion);
//    QVector3D pos = matrix * QVector3D(0, 1, 0);
//    homogeneousPosition = QVector4D(pos, 0);
//}

//void DirectionLight::setHomogeneousPosition()
//{
//    //平行光的旋转矩阵
//    QMatrix4x4 matrix;
//    matrix.rotate(mQuaternion);
//    //转化为位置向量 没有旋转的平行光是向下射的
//    QVector3D pos = matrix * QVector3D(0, 1, 0);
//    //变为齐次坐标为0的四维向量 表示无穷远
//    homogeneousPosition = QVector4D(pos, 0);
//    //更新场景中的灯光
//    mScene->updateDirectionLightPosition();
//}

//QVector4D DirectionLight::getHomogeneousPosition()
//{
//    return homogeneousPosition;
//}

//void DirectionLight::setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform)
//{
//    Light::setRotation(angle, axis, isUpdateLocalTransform);
//    setHomogeneousPosition();
//}

//void DirectionLight::setRotation(QVector3D rotation, bool isUpdateLocalTransform)
//{
//    Light::setRotation(rotation, isUpdateLocalTransform);
//    setHomogeneousPosition();
//}

//void DirectionLight::setColor(int r, int g, int b)
//{
//    mColor = QColor(r, g, b);
//    //直射光的颜色需要调节hsl 否则对场景颜色的影响太大
//    //mColorVector4f为调节hsl后的 对场景产生影响 mColor为调节之前的 颜色按钮还是指定颜色
//    QColor color = QColor::fromHslF(mColor.hueF(), mColor.saturationF(), 0.85f);
//    mColorVector4f = QVector4D(color.redF(), color.greenF(), color.blueF(), 1.0f);
//    //更新场景渲染
//    mScene->updateDirectionLightColor();
//}


///*************************
// * PointLight
// * ************************/


//float PointLight::attenuationThreshold = 0.2f;

//PointLight::PointLight(Scene *scene)
//    : PointLight(scene, 0, 0, 0, 0, 0, 0, 1, 1, 1)
//{

//}

//PointLight::PointLight(Scene *scene, float px, float py, float pz,
//                       float rx, float ry, float rz, float sx, float sy, float sz)
//    : Light(scene, px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    mIndicator = new PointLightIndicator(px, py, pz, rx, ry, rz, 1, 1, 1);
//    constantFactor = 2.0f;
//    linearFactor = 0.5f;
//    quadricFactor = 0.2f;
//    mIntensity = 1.0f;
//    updateValidRange();
//}

//void PointLight::setConstantFactor(float value)
//{
//    constantFactor = value;
//    mScene->updatePointLightOption();
//    updateValidRange();
//}

//float PointLight::getConstantFactor()
//{
//    return constantFactor;
//}

//void PointLight::setLinearFactor(float value)
//{
//    linearFactor = value;
//    mScene->updatePointLightOption();
//    updateValidRange();
//}

//float PointLight::getLinearFactor()
//{
//    return linearFactor;
//}

//void PointLight::setQuadricFactor(float value)
//{
//    quadricFactor = value;
//    mScene->updatePointLightOption();
//    updateValidRange();
//}

//float PointLight::getQuadricFactor()
//{
//    return quadricFactor;
//}

//QVector4D PointLight::getOptionVector4f()
//{
//    return QVector4D(constantFactor, linearFactor, quadricFactor, mIntensity);
//}

//void PointLight::updateValidRange()
//{
//    //当衰减度小于阈值时 认为在范围外 指示器需要指示出范围
//    float range = (1 / attenuationThreshold - constantFactor) / (linearFactor + quadricFactor * quadricFactor);
//    if (range < 0)
//        range = 0;
//    mIndicator->setScale(QVector3D(range, range, range));
//}

//int PointLight::getIntensity()
//{
//    return mIntensity;
//}

//void PointLight::setIntensity(int intensity)
//{
//    mIntensity = intensity;
//    mScene->updatePointLightOption();
//}

//void PointLight::setPosition(QVector3D position, bool isUpdateLocalTransform)
//{
//    Light::setPosition(position, isUpdateLocalTransform);
//    mScene->updatePointLightPosition();
//}

//void PointLight::setColor(int r, int g, int b)
//{
//    Light::setColor(r, g, b);
//    mScene->updatePointLightColor();
//}


///*************************
// * SpotLight
// * ************************/


//SpotLight::SpotLight(Scene *scene)
//    : SpotLight(scene, 0, 0, 0, 0, 0, 0, 1, 1, 1)
//{

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

//void SpotLight::setConstantFactor(float value)
//{
//    constantFactor = value;
//    mScene->updateSpotLightOption();
//    updateValidRange();
//}

//void SpotLight::setLinearFactor(float value)
//{
//    linearFactor = value;
//    mScene->updateSpotLightOption();
//    updateValidRange();
//}

//void SpotLight::setQuadricFactor(float value)
//{
//    quadricFactor = value;
//    mScene->updateSpotLightOption();
//    updateValidRange();
//}

//QVector4D SpotLight::getOptionVector4f()
//{
//    return QVector4D(constantFactor, linearFactor, quadricFactor, spotAngle);
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
