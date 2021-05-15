#include "BBScene.h"
#include "BBUtils.h"
#include "BBCamera.h"
#include "BBSkyBox.h"
#include "BBHorizontalPlane.h"
#include "BBGameObject.h"
#include "BBModel.h"
#include "BBRay.h"

BBScene::BBScene()
{
    m_fUpdateRate = (float) BB_CONSTANT_UPDATE_RATE / 1000;
    m_pCamera = new BBCamera;
    m_pSkyBox = new BBSkyBox;
    m_pHorizontalPlane = new BBHorizontalPlane();

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
    BB_SAFE_DELETE(m_pHorizontalPlane);
    QList<BBGameObject*> objects = m_Models;
//    + directionLights + pointLights + spotLights + audios;
    QList<BBGameObject*>::Iterator itr;
    for (itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        deleteGameObject(pObject);
    }
}

void BBScene::init()
{
    m_pCamera->setViewportSize(800.0f, 600.0f);

    m_pSkyBox->init(QString(BB_PATH_RESOURCE) + "skyboxs/1/");
    // Horizontal reference grid
    m_pHorizontalPlane->init();
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

    m_pHorizontalPlane->render(m_pCamera);

    // render dropped model
//    //渲染灯光的图标 关闭深度测试 最后渲染 出现在最前面
    QList<BBGameObject*> objects = m_Models;
            //+ directionLights + pointLights + spotLights + audios;
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->render(m_pCamera);
    }
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

    QList<BBGameObject*> objects = m_Models;
            //+ directionLights + pointLights + spotLights + audios;
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->resize(width, height);
    }

    m_pSkyBox->resize(width, height);
    m_pHorizontalPlane->resize(width, height);

//    transformCoordinate->resize(width, height);
//    //particle->resize(width, height);

//    mFBO = new FrameBufferObject;
//    mFBO->attachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height);
//    mFBO->attachDepthBuffer("depth", width, height);
//    mFBO->finish();
}

BBModel* BBScene::createModel(const QString filePath, int x, int y, bool bSelect)
{
    BBRay ray = m_pCamera->createRayFromScreen(x, y);
    // ground y=0
    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
    return createModel(filePath, hit, bSelect);
}

BBModel* BBScene::createModel(const QString filePath, QVector3D position, bool bSelect)
{
    BBModel *pModel = NULL;
    if (filePath == "terrain")
    {
//        model->setBaseAttributes(QFileInfo(filePath).baseName(), TerrainClassName, "terrain");
    }
    else
    {
        pModel = new BBModel(position.x(), position.y(), position.z(), 0, 0, 0, 1, 1, 1, BBMeshType::OBJ);
        pModel->setBaseAttributes(QFileInfo(filePath).baseName(), BB_CLASSNAME_MODEL, "model");
    }
    pModel->init(filePath);
    pModel->resize(m_pCamera->getViewportWidth(), m_pCamera->getViewportHeight());
    m_Models.append(pModel);

//    //创建后坐标系操作当前对象
//    if (isSelect)
//    {
//        transformCoordinate->setSelectedObject(model);
//    }
//    //给该模型添加灯光效果
//    QVector3D count = QVector3D(directionLights.count(), pointLights.count(), spotLights.count());
//    model->updateDirectionLightPosition(directionLightPosition, count);
//    model->updateDirectionLightColor(directionLightColor, directionLights.count());
//    model->updatePointLightPosition(pointLightPosition, count);
//    model->updatePointLightColor(pointLightColor, pointLights.count());
//    model->updatePointLightOption(pointLightOption, pointLights.count());
//    model->updateSpotLightPosition(spotLightPosition, count);
//    model->updateSpotLightDirection(spotLightDirection, spotLights.count());
//    model->updateSpotLightColor(spotLightColor, spotLights.count());
//    model->updateSpotLightOption(spotLightOption, spotLights.count());
//    model->updateSpotLightOption2(spotLightOption2, spotLights.count());
//    model->switchFog(fogSwitch);
//    model->setFogColor(fogColor.redF(), fogColor.greenF(), fogColor.blueF());
//    model->setFogOption(fogStart, fogEnd, fogDensity, fogPower);
//    model->setFogMode(fogMode);

    return pModel;
}

void BBScene::deleteGameObject(BBGameObject *pObject)
{
    if (pObject->getClassName() == BB_CLASSNAME_MODEL)
    {
        m_Models.removeOne(pObject);
    }
//    else if (object->getClassName() == TerrainClassName)
//    {
//        models.removeOne(object);
//    }
//    else if (object->getClassName() == DirectionLightClassName)
//    {
//        directionLights.removeOne(object);
//        updateDirectionLightPosition();
//        updateDirectionLightColor();
//    }
//    else if (object->getClassName() == PointLightClassName)
//    {
//        pointLights.removeOne(object);
//        updatePointLightPosition();
//        updatePointLightColor();
//        updatePointLightOption();
//    }
//    else if (object->getClassName() == SpotLightClassName)
//    {
//        spotLights.removeOne(object);
//        updateSpotLightPosition();
//        updateSpotLightDirection();
//        updateSpotLightColor();
//        updateSpotLightOption();
//        updateSpotLightOption2();
//    }
//    else if (object->getClassName() == AudioClassName)
//    {
//        audios.removeOne(object);
//    }
//    transformCoordinate->setSelectedObject(nullptr);

    BB_SAFE_DELETE(pObject);
}




//void Scene::changeSkybox(QString path)
//{
//    skybox->change(path);
//}

//void Scene::renderShadowMap()
//{
//    //离屏渲染
//    mFBO->bind();
//    //只绘制背面 否则会产生自身阴影
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
//    //高度图 阴影
//    QVector3D lightPos(5, 5, -5);
//    QMatrix4x4 lightView;
//    lightView.lookAt(lightPos, QVector3D(5, 0, -5), QVector3D(0, 0, -1));
//    QMatrix4x4 lightProjection;
//    lightProjection.ortho(-10, 10, -10, 10, -10, 10);
//    //第一盏平行光的方向
//    if (directionLights.size() > 0)
//    {
//        lightView.rotate(-directionLights[0]->getRotation().z(), 0, 0, 1);
//        lightView.rotate(-directionLights[0]->getRotation().x(), 1, 0, 0);
//        lightView.rotate(-directionLights[0]->getRotation().y(), 0, 1, 0);
//        if ((directionLights[0]->getQuaternion() * QVector3D(0, -1, 0)).y() < -0.01)
//        {
//            //从下射到上方的光 不要阴影
//            for (QList<GameObject*>::Iterator itr = models.begin(); itr != models.end(); itr++)
//            {
//                Model *model = (Model*)(*itr);
//                model->renderBuffer(lightView, lightProjection, lightPos);
//            }
//        }
//    }
//    glDisable(GL_CULL_FACE);
//    mFBO->unbind();
//}

//Model *Scene::createModelForPreview(QString filePath, float distFactor)
//{
//    Model *model = new Model;
//    model->setBaseAttributes(QFileInfo(filePath).baseName(), ModelClassName, "model");
//    model->init(filePath);
//    model->resize(camera.viewportWidth, camera.viewportHeight);
//    models.append(model);
//    model->lookAtSelf(camera.pos, camera.viewCenter, distFactor);
//    //不显示包围盒
//    model->setVisible(false);
//    return model;
//}

//GameObject *Scene::pickObject(Ray ray, bool isSelect)
//{
//    float distance = FLT_MAX;
//    GameObject *selectedObject = nullptr;
//    //遍历模型 灯光
//    QList<GameObject*> objects = models + directionLights + pointLights + spotLights + audios;
//    QList<GameObject*>::Iterator itr;
//    for (itr = objects.begin(); itr != objects.end(); itr++)
//    {
//        GameObject *object = *itr;
//        float d;
//        if (object->hit(ray, d))
//        {
//            if (d < distance)
//            {
//                distance = d;
//                selectedObject = object;
//            }
//        }
//    }
//    //if (selectedObject != nullptr)
//    if (isSelect)
//    {
//        //拾取并用坐标系将其选中 有时候只需要拾取 不需要选中
//        transformCoordinate->setSelectedObject(selectedObject);
//    }
//    return selectedObject;
//}

//GameObject *Scene::createLight(QString fileName, int x, int y, bool isSelect)
//{
//    Ray ray = camera.createRayFromScreen(x, y);
//    //地面y=0
//    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
//    return createLight(fileName, hit, isSelect);
//}

//GameObject *Scene::createLight(QString fileName, QVector3D position, bool isSelect)
//{
//    Light *light;
//    if (fileName == "directional light.png")
//    {
//        light = new DirectionLight(this, position.x(), position.y(), position.z(), 45, 45, 0, 1, 1, 1);
//        light->setBaseAttributes(fileName.split('.')[0], DirectionLightClassName, "light2");
//        directionLights.append(light);
//        //如果构造函数中调用 light还没加入list 灯光访问不到
//        updateDirectionLightPosition();
//        //增加了新的光 颜色也有一份新颜色
//        updateDirectionLightColor();
//    }
//    else if (fileName == "point light.png")
//    {
//        light = new PointLight(this, position.x(), position.y(), position.z(), 0, 0, 0, 1, 1, 1);
//        light->setBaseAttributes(fileName.split('.')[0], PointLightClassName, "light2");
//        pointLights.append(light);
//        updatePointLightPosition();
//        updatePointLightColor();
//        updatePointLightOption();
//    }
//    else if (fileName == "spot light.png")
//    {
//        light = new SpotLight(this, position.x(), position.y(), position.z(), 0, 0, 0, 1, 1, 1);
//        light->setBaseAttributes(fileName.split('.')[0], SpotLightClassName, "light2");
//        spotLights.append(light);
//        updateSpotLightPosition();
//        updateSpotLightDirection();
//        updateSpotLightColor();
//        updateSpotLightOption();
//        updateSpotLightOption2();
//    }
//    light->init(fileName);
//    light->resize(camera.viewportWidth, camera.viewportHeight);
//    if (isSelect)
//    {
//        transformCoordinate->setSelectedObject(light);
//    }
//    return light;
//}

//Audio *Scene::createAudio(QString filePath, int x, int y, bool isSelect)
//{
//    Ray ray = camera.createRayFromScreen(x, y);
//    //地面y=0
//    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
//    return createAudio(filePath, hit, isSelect);
//}

//Audio *Scene::createAudio(QString filePath, QVector3D position, bool isSelect)
//{
//    Audio *audio;
//    audio = new Audio(position.x(), position.y(), position.z());
//    audio->setBaseAttributes(QFileInfo(filePath).baseName(), AudioClassName, "audio2");
//    audios.append(audio);
//    audio->init();
//    audio->resize(camera.viewportWidth, camera.viewportHeight);
//    if (isSelect)
//    {
//        transformCoordinate->setSelectedObject(audio);
//    }
//    return audio;
//}

//void Scene::lookAtGameObject(GameObject *object)
//{
//    if (object != nullptr)
//    {
//        transformCoordinate->setSelectedObject(object);
//        camera.lookAt(object);
//    }
//}


//QList<GameObject*> Scene::setSelectionRegion(QPoint start, QPoint end)
//{
//    QList<GameObject*> result;
//    //end不能超出视口
//    if (end.x() < 0)
//        end.setX(0);
//    else if (end.x() > camera.viewportWidth)
//        end.setX(camera.viewportWidth);
//    if (end.y() < 0)
//        end.setY(0);
//    else if (end.y() > camera.viewportHeight)
//        end.setY(camera.viewportHeight);
//    //求左下角坐标 取较小的x 较大的y
//    int x = start.x() < end.x() ? start.x() : end.x();
//    int y = start.y() > end.y() ? start.y() : end.y();
//    int w = abs(start.x() - end.x());
//    int h = abs(start.y() - end.y());
//    //选区四个顶点对应的3D射线
//    Ray topLeft = camera.createRayFromScreen(x, y - h);
//    Ray topRight = camera.createRayFromScreen(x + w, y - h);
//    Ray bottomRight = camera.createRayFromScreen(x + w, y);
//    Ray bottomLeft = camera.createRayFromScreen(x, y);
//    //左上角为原点的坐标 变换为中心为原点的坐标
//    y = -y;
//    x = x - camera.viewportWidth / 2;
//    y = y + camera.viewportHeight / 2;
//    //设定选区位置
//    selectionRegion->setRect(x, y, w, h);
//    //计算2D框选区对应的3D包围盒
//    //两条射线组成一个平面 包围盒在上下左右平面之中的被选中
//    QList<GameObject*> objects = models + directionLights + pointLights + spotLights + audios;
//    int count = objects.count();
//    for (int i = 0; i < count; i++)
//    {
//        //计算对象的包围盒是否位于选区的上下左右四个平面之中
//        if (objects.at(i)->belongToSelectionRegion(
//                    topLeft.getNearPoint(), bottomLeft.getNearPoint(), topLeft.getFarPoint(),
//                    topLeft.getNearPoint(), topLeft.getFarPoint(), topRight.getNearPoint(),
//                    topRight.getNearPoint(), topRight.getFarPoint(), bottomRight.getNearPoint(),
//                    bottomLeft.getNearPoint(), bottomRight.getNearPoint(), bottomLeft.getFarPoint()))
//        {
//            result.append(objects.at(i));
//        }
//    }
//    //返回选区中的所有对象
//    return result;
//}

//void Scene::setSelectionRegionVisible(bool isVisible)
//{
//    selectionRegion->setVisible(isVisible);
//}

//Model *Scene::pickModel(int x, int y)
//{
//    //产生射线
//    Ray ray = camera.createRayFromScreen(x, y);
//    //拾取Model 灯光不拾取
//    float distance = FLT_MAX;
//    Model *model = nullptr;
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        GameObject *object = *itr;
//        float d;
//        if (object->hit(ray, d))
//        {
//            if (d < distance)
//            {
//                distance = d;
//                model = (Model*)object;
//            }
//        }
//    }
//    return model;
//}

//bool Scene::setModelMaterial(Model *model, QString mtlPath)
//{
//    if (model)
//    {
//        //模型设置材质
//        Material *material = Material::mtlMap.value(mtlPath);
//        model->setMaterial(material);
//        return true;
//    }
//    else
//    {
//        //没有合法模型
//        return false;
//    }
//}

//void Scene::updateDirectionLightPosition()
//{
//    //平行光的个数
//    int count = directionLights.count();
//    directionLightPosition = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        DirectionLight *light = (DirectionLight*)directionLights.at(i);
//        directionLightPosition[i] = light->getHomogeneousPosition();
//    }
//    //场景中所有受灯光影响的对象 shader的平行光都需要更新
//    //判断是否开启灯光渲染？ 目前设所有模型都渲染灯光
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateDirectionLightPosition(directionLightPosition, QVector3D(count, pointLights.count(), spotLights.count()));
//    }
//}

//void Scene::updateDirectionLightColor()
//{
//    //平行光的个数
//    int count = directionLights.count();
//    directionLightColor = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        DirectionLight *light = (DirectionLight*)directionLights.at(i);
//        directionLightColor[i] = light->getColorVector4f();
//    }
//    //场景中所有受灯光影响的对象 shader的光颜色都需要更新
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateDirectionLightColor(directionLightColor, count);
//    }
//}

//void Scene::updatePointLightPosition()
//{
//    int count = pointLights.count();
//    pointLightPosition = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        PointLight *light = (PointLight*)pointLights.at(i);
//        pointLightPosition[i] = QVector4D(light->getPosition(), 1.0f);
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updatePointLightPosition(pointLightPosition,
//                                        QVector3D(directionLights.count(), count, spotLights.count()));
//    }
//}

//void Scene::updatePointLightColor()
//{
//    int count = pointLights.count();
//    pointLightColor = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        PointLight *light = (PointLight*)pointLights.at(i);
//        pointLightColor[i] = light->getColorVector4f();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updatePointLightColor(pointLightColor, count);
//    }
//}

//void Scene::updatePointLightOption()
//{
//    int count = pointLights.count();
//    pointLightOption = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        PointLight *light = (PointLight*)pointLights.at(i);
//        pointLightOption[i] = light->getOptionVector4f();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updatePointLightOption(pointLightOption, count);
//    }
//}

//void Scene::updateSpotLightPosition()
//{
//    int count = spotLights.count();
//    spotLightPosition = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        SpotLight *light = (SpotLight*)spotLights.at(i);
//        spotLightPosition[i] = QVector4D(light->getPosition(), 1.0f);
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateSpotLightPosition(spotLightPosition,
//                                       QVector3D(directionLights.count(), pointLights.count(), count));
//    }
//}

//void Scene::updateSpotLightDirection()
//{
//    int count = spotLights.count();
//    spotLightDirection = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        SpotLight *light = (SpotLight*)spotLights.at(i);
//        spotLightDirection[i] = light->getLightDirection();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateSpotLightDirection(spotLightDirection, count);
//    }
//}

//void Scene::updateSpotLightColor()
//{
//    int count = spotLights.count();
//    spotLightColor = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        SpotLight *light = (SpotLight*)spotLights.at(i);
//        spotLightColor[i] = light->getColorVector4f();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateSpotLightColor(spotLightColor, count);
//    }
//}

//void Scene::updateSpotLightOption()
//{
//    int count = spotLights.count();
//    spotLightOption = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        SpotLight *light = (SpotLight*)spotLights.at(i);
//        spotLightOption[i] = light->getOptionVector4f();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateSpotLightOption(spotLightOption, count);
//    }
//}

//void Scene::updateSpotLightOption2()
//{
//    int count = spotLights.count();
//    spotLightOption2 = new QVector4D[count];
//    for (int i = 0; i < count; i++)
//    {
//        SpotLight *light = (SpotLight*)spotLights.at(i);
//        spotLightOption2[i] = light->getOption2Vector4f();
//    }
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->updateSpotLightOption2(spotLightOption2, count);
//    }
//}

//void Scene::switchFog(bool b)
//{
//    fogSwitch = b;
//    //场景中所有模型开关雾
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->switchFog(b);
//    }
//}

//float Scene::getFogSwitch()
//{
//    return fogSwitch;
//}

//void Scene::setFogColor(float r, float g, float b)
//{
//    fogColor = QColor::fromRgbF(r, g, b);
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->setFogColor(r, g, b);
//    }
//}

//QColor Scene::getFogColor()
//{
//    return fogColor;
//}

//void Scene::setFogOption(float start, float end, int density, float power)
//{
//    fogStart = start;
//    fogEnd = end;
//    fogDensity = density;
//    fogPower = power;
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->setFogOption(fogStart, fogEnd, fogDensity, fogPower);
//    }
//}

//float Scene::getFogStart()
//{
//    return fogStart;
//}

//float Scene::getFogEnd()
//{
//    return fogEnd;
//}

//int Scene::getFogDensity()
//{
//    return fogDensity;
//}

//float Scene::getFogPower()
//{
//    return fogPower;
//}

//void Scene::setFogMode(int mode)
//{
//    fogMode = mode;
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->setFogMode(fogMode);
//    }
//}

//int Scene::getFogMode()
//{
//    return fogMode;
//}

//void Scene::play()
//{
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        //骨骼动画
//        model->play();
//        //开启脚本调用开关
//        model->onLua();
//    }
//}

//void Scene::stop()
//{
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->stop();
//        model->offLua();
//    }
//}

//void Scene::onKeyPress(QKeyEvent *e)
//{
//    QList<GameObject*>::Iterator itr;
//    for (itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->onKeyDownLua(e);
//    }
//}
