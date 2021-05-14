#include "BBScene.h"
#include "BBUtils.h"
#include "BBCamera.h"
#include "BBSkyBox.h"

BBScene::BBScene()
{
    m_fUpdateRate = (float) BB_CONSTANT_UPDATE_RATE / 1000;
    m_pCamera = new BBCamera;
    m_pSkyBox = new BBSkyBox;

//    horizontalPlane = new HorizontalPlane();
//    selectionRegion = new SelectionRegion();
//    transformCoordinate = new TransformCoordinate();
//    particle = new Particle();
//    fogSwitch = false;
//    fogColor = QColor(128, 128, 128);
//    fogStart = 0.1f;
//    fogEnd = 100.0f;
//    //1表示线性雾 2表示指数雾 3表示指数雾x
//    fogMode = 0;
//    fogDensity = 10;
//    fogPower = 2.0f;
}

BBScene::~BBScene()
{
    BB_SAFE_DELETE(m_pCamera);
    BB_SAFE_DELETE(m_pSkyBox);
//    QList<GameObject*> objects = models + directionLights + pointLights + spotLights + audios;
//    QList<GameObject*>::Iterator itr;
//    for (itr = objects.begin(); itr != objects.end(); itr++)
//    {
//        GameObject *object = *itr;
//        deleteGameObject(object);
//    }
}

void BBScene::init()
{
    m_pCamera->setViewportSize(800.0f, 600.0f);

    m_pSkyBox->init(QString(BB_PATH_RESOURCE) + "skyboxs/1/");
//    //水平面
//    horizontalPlane->init();
//    //模型坐标系
//    transformCoordinate->init();
//    //粒子
//    particle->init();

//    //2D
//    sprite.setImage("../../../../BBearEngine/resources/textures/1158133.png");
//    //sprite.setImage("../../../../BBearEngine/resources/textures/face.jpg");
//    sprite.setRect(0.0f, 0.0f, 100.0f, 100.0f);
}

void BBScene::render()
{
    // 3D camera mode
    m_pCamera->switchTo3D();
    // refresh camera position and direction, update pos and ..., Convenient for subsequent use
    m_pCamera->update(m_fUpdateRate);

//    //执行脚本的update接口
//    for (QList<GameObject*>::Iterator itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->onUpdateLua();
//    }

//    renderShadowMap();
//    //给模型添加高度图信息 用于阴影计算
//    for (QList<GameObject*>::Iterator itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->setShadowMap(mFBO->getBuffer("depth"));
//    }

    // Render the skybox at first
    m_pSkyBox->render(m_pCamera);

    // test
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-5, -5, -10);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(5, -5, -10);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(5, 5, -10);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-5, 5, -10);
    glEnd();

//    //渲染水平面网格
//    horizontalPlane->render(camera);
//    //渲染拖出来的模型
//    //渲染灯光的图标 关闭深度测试 最后渲染 出现在最前面
//    QList<GameObject*> objects = models + directionLights + pointLights + spotLights + audios;
//    for (QList<GameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
//    {
//        GameObject *object = *itr;
//        object->render(camera);
//    }
//    //模型坐标系
//    transformCoordinate->render(camera);
//    //粒子
//    //particle->update();
//    //particle->render(camera);

    // 2D camera mode
    m_pCamera->switchTo2D();
//    //渲染框选区域
//    selectionRegion->render();
//    //sprite.draw();

//    //实心？   会导致选框绘制不出
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, mFBO->getBuffer("depth"));

//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0f, 0.0f);
//    glVertex3f(-50, -50, 0);
//    glTexCoord2f(1.0f, 0.0f);
//    glVertex3f(50, -50, 0);
//    glTexCoord2f(1.0f, 1.0f);
//    glVertex3f(50, 50, 0);
//    glTexCoord2f(0.0f, 1.0f);
//    glVertex3f(-50, 50, 0);
//    glEnd();
}

void BBScene::resize(float width, float height)
{
    // 3D camera, resize
    m_pCamera->setViewportSize(width, height);
//    //shader投影矩阵改变
//    QList<GameObject*> objects = models + directionLights + pointLights + spotLights + audios;
//    QList<GameObject*>::Iterator itr;
//    for (itr = objects.begin(); itr != objects.end(); itr++)
//    {
//        GameObject *object = *itr;
//        object->resize(width, height);
//    }

    m_pSkyBox->resize(width, height);
//    horizontalPlane->resize(width, height);
//    transformCoordinate->resize(width, height);
//    //particle->resize(width, height);

//    mFBO = new FrameBufferObject;
//    mFBO->attachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height);
//    mFBO->attachDepthBuffer("depth", width, height);
//    mFBO->finish();
}
