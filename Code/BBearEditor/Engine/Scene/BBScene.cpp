#include "BBScene.h"
#include "Utils/BBUtils.h"
#include "Render/BBFrameBufferObject.h"
#include "Render/BBCamera.h"
#include "3D/BBSkyBox.h"
#include "3D/BBHorizontalPlane.h"
#include "Base/BBGameObject.h"
#include "3D/BBModel.h"
#include "Geometry/BBRay.h"
#include "2D/BBSelectionRegion.h"
#include <cfloat>
#include "3D/BBCoordinateSystem.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBRenderState.h"
#include "Render/Light/BBLight.h"
#include "Render/Light/BBDirectionalLight.h"
#include "Render/Light/BBPointLight.h"
#include "Render/Light/BBSpotLight.h"
#include "IO/BBMaterialFileManager.h"


QString BBScene::m_ColorBufferName = "color";

BBScene::BBScene()
{
    m_RenderingFunc = &BBScene::defaultRender;
    m_fUpdateRate = (float) BB_CONSTANT_UPDATE_RATE / 1000;
    for (int i = 0; i < 3; i++)
    {
        m_pFBO[i] = NULL;
    }
    m_pCamera = NULL;
    m_pSkyBox = NULL;
    m_pHorizontalPlane = NULL;
    m_pSelectionRegion = NULL;
    m_pTransformCoordinateSystem = NULL;
    m_pTiledFullScreenQuad = NULL;

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
    for (int i = 0; i < 3; i++)
    {
        BB_SAFE_DELETE(m_pFBO[i]);
    }
    BB_SAFE_DELETE(m_pCamera);
    BB_SAFE_DELETE(m_pSkyBox);
    BB_SAFE_DELETE(m_pHorizontalPlane);
    BB_SAFE_DELETE(m_pSelectionRegion);
    BB_SAFE_DELETE(m_pTransformCoordinateSystem);
    BB_SAFE_DELETE(m_pTiledFullScreenQuad);
    QList<BBGameObject*> objects = m_Models + m_Lights;
    QList<BBGameObject*>::Iterator itr;
    for (itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        deleteGameObject(pObject);
    }
}

void BBScene::init()
{
    BBGlobalRenderState::init();

    m_pCamera = new BBCamera;
    m_pSkyBox = new BBSkyBox;
    m_pHorizontalPlane = new BBHorizontalPlane();
    m_pSelectionRegion = new BBSelectionRegion();
    m_pTransformCoordinateSystem = new BBTransformCoordinateSystem();
    m_pTiledFullScreenQuad = new BBTiledFullScreenQuad();

    m_pCamera->setViewportSize(800.0f, 600.0f);

    m_pSkyBox->init(QString(BB_PATH_RESOURCE) + "skyboxs/1/");
    m_pHorizontalPlane->init();
    m_pTransformCoordinateSystem->init();
    m_pTiledFullScreenQuad->init();
}

void BBScene::render()
{
    (this->*m_RenderingFunc)();
}

void BBScene::defaultRender()
{
    // 3D camera mode
    m_pCamera->switchTo3D();
    // refresh camera position and direction, update pos and ..., Convenient for subsequent use
    m_pCamera->update(m_fUpdateRate);

    bindFBO();
//    renderShadowMap();
//    //给模型添加高度图信息 用于阴影计算
//    for (QList<GameObject*>::Iterator itr = models.begin(); itr != models.end(); itr++)
//    {
//        Model *model = (Model*)(*itr);
//        model->setShadowMap(mFBO->getBuffer("depth"));
//    }

    m_pSkyBox->render(m_pCamera);
    m_pHorizontalPlane->render(m_pCamera);

    // render dropped model
    QList<BBGameObject*> objects = m_Models + m_Lights;
            //+ audios;
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        pObject->render(m_pCamera);
    }

    m_pTransformCoordinateSystem->render(m_pCamera);

    unbindFBO();

//    if (m_bEnableFullScreenQuad)
//    {
//        m_pFullScreenQuad->setTexture(m_pFBO->getBuffer(m_ColorBufferName));
//        m_pFullScreenQuad->render(m_pCamera);
//    }

    // 2D camera mode
    m_pCamera->switchTo2D();

    m_pSelectionRegion->render();

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

void BBScene::deferredRender()
{
    // 3D camera mode
    m_pCamera->switchTo3D();
    // refresh camera position and direction, update pos and ..., Convenient for subsequent use
    m_pCamera->update(m_fUpdateRate);

    for (int i = 0; i < 2; i++)
    {
        m_pFBO[i]->bind();

        QList<BBGameObject*> objects = m_Models;
        for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
        {
            BBGameObject *pObject = *itr;
            pObject->setCurrentMaterial(BBMaterialFileManager::getDeferredRenderingMaterial(i));
            pObject->render(m_pCamera);
        }

        m_pFBO[i]->unbind();
    }

    m_pTiledFullScreenQuad->setTexture(NAME_TEXTURE(0), m_pFBO[0]->getBuffer(m_ColorBufferName));
    m_pTiledFullScreenQuad->setTexture(NAME_TEXTURE(1), m_pFBO[1]->getBuffer(m_ColorBufferName));
    m_pTiledFullScreenQuad->setTexture(NAME_TEXTURE(2), m_pFBO[2]->getBuffer(m_ColorBufferName));
    m_pTiledFullScreenQuad->render(m_pCamera);

    // 2D camera mode
    m_pCamera->switchTo2D();
}

void BBScene::resize(float width, float height)
{
    // 3D camera, resize
    m_pCamera->setViewportSize(width, height);

    for (int i = 0; i < 3; i++)
    {
        if (!m_pFBO[i])
            BB_SAFE_DELETE(m_pFBO[i]);
        m_pFBO[i] = new BBFrameBufferObject;
        m_pFBO[i]->attachColorBuffer(m_ColorBufferName, GL_COLOR_ATTACHMENT0, width, height);
        m_pFBO[i]->attachDepthBuffer("depth", width, height);
        m_pFBO[i]->finish();
    }
}

void BBScene::setSkyBox(const QString &path)
{
    m_pSkyBox->change(path);
}

void BBScene::enableSkyBox(bool bEnable)
{
    m_pSkyBox->setVisibility(bEnable);
}

void BBScene::enableHorizontalPlane(bool bEnable)
{
    m_pHorizontalPlane->setVisibility(bEnable);
}

BBModel* BBScene::createModel(const QString &filePath, int x, int y)
{
    BBRay ray = m_pCamera->createRayFromScreen(x, y);
    // ground y=0
    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
    return createModel(filePath, hit);
}

BBModel* BBScene::createModel(const QString &filePath,
                              const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
{
    BBModel *pModel = NULL;
    if (filePath == "terrain")
    {
//        model->setBaseAttributes(QFileInfo(filePath).baseName(), TerrainClassName, "terrain");
    }
    else
    {
        pModel = new BBModel(position, rotation, scale, BBMeshType::OBJ);
        pModel->setBaseAttributes(QFileInfo(filePath).baseName(), BB_CLASSNAME_MODEL, "model");
    }
    pModel->init(filePath);
    m_Models.append(pModel);

    return pModel;
}

BBModel* BBScene::createModel(const BBSerializer::BBGameObject &gameObject)
{
    std::string filePath = gameObject.filepath();
    BBSerializer::BBVector3f position = gameObject.position();
    BBSerializer::BBVector3f rotation = gameObject.rotation();
    BBSerializer::BBVector3f scale = gameObject.scale();
    // create model with default settings
    BBModel *pModel = createModel(QString::fromStdString(filePath),
                                  QVector3D(position.x(), position.y(), position.z()),
                                  QVector3D(rotation.x(), rotation.y(), rotation.z()),
                                  QVector3D(scale.x(), scale.y(), scale.z()));
    // load extra info from the scene file
    pModel->setName(QString::fromStdString(gameObject.name()));

    return pModel;
}

BBModel* BBScene::createModelForPreview(const QString &filePath, float fDistFactor)
{
    BBModel *pModel = createModel(filePath);
    QVector3D position;
    QVector3D viewCenter;
    pModel->showCloseUp(position, viewCenter, fDistFactor);
    m_pCamera->setPosition(position);
    m_pCamera->setViewCenter(viewCenter);
    // do not show bounding box
    pModel->setVisibility(false);
    return pModel;
}

BBLight* BBScene::createLight(const QString &fileName, int x, int y, bool bSelect)
{
    BBRay ray = m_pCamera->createRayFromScreen(x, y);
    // ground y=0
    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
    return createLight(fileName, hit, QVector3D(0, 0, 0), bSelect);
}

BBLight* BBScene::createLight(const QString &fileName, const QVector3D &position, const QVector3D &rotation, bool bSelect)
{
    BBLight *pLight;
    if (fileName == BB_FILENAME_DIRECTIONALLIGHT)
    {
        pLight = new BBDirectionalLight(this, position, QVector3D(0, -30, -30));
        pLight->setBaseAttributes(fileName.split('.')[0], BB_CLASSNAME_DIRECTIONAL_LIGHT, "light2");
    }
    else if (fileName == BB_FILENAME_POINTLIGHT)
    {
        pLight = new BBPointLight(this, position);
        pLight->setBaseAttributes(fileName.split('.')[0], BB_CLASSNAME_POINT_LIGHT, "light2");
    }
    else if (fileName == BB_FILENAME_SPOTLIGHT)
    {
        pLight = new BBSpotLight(this, position, rotation);
        pLight->setBaseAttributes(fileName.split('.')[0], BB_CLASSNAME_SPOT_LIGHT, "light2");
    }
    pLight->init(fileName);
    m_Lights.append(pLight);
    if (bSelect)
    {
        m_pTransformCoordinateSystem->setSelectedObject(pLight);
    }
    return pLight;
}

BBGameObject* BBScene::pickObject(const BBRay &ray, bool bSelect)
{
    float fDistance = FLT_MAX;
    BBGameObject *pSelectedObject = nullptr;
    // traverse models and lights
    QList<BBGameObject*> objects = m_Models + m_Lights;
            //audios;
    QList<BBGameObject*>::Iterator itr;
    for (itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        float d;
        if (pObject->hit(ray, d))
        {
            if (d < fDistance)
            {
                fDistance = d;
                pSelectedObject = pObject;
            }
        }
    }
//    // if (pSelectedObject != nullptr)
//    if (bSelect)
//    {
//        //拾取并用坐标系将其选中 有时候只需要拾取 不需要选中
//        transformCoordinate->setSelectedObject(selectedObject);
//    }
    return pSelectedObject;
}

void BBScene::lookAtGameObject(BBGameObject *pGameObject)
{
    BB_PROCESS_ERROR_RETURN(pGameObject);

    m_pTransformCoordinateSystem->setSelectedObject(pGameObject);
    m_pCamera->lookAt(pGameObject);
}

void BBScene::deleteGameObject(BBGameObject *pGameObject)
{
    if (pGameObject->getClassName() == BB_CLASSNAME_MODEL)
    {
        m_Models.removeOne(pGameObject);
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_DIRECTIONAL_LIGHT
             || pGameObject->getClassName() == BB_CLASSNAME_POINT_LIGHT
             || pGameObject->getClassName() == BB_CLASSNAME_SPOT_LIGHT)
    {
        m_Lights.removeOne(pGameObject);
    }
//    else if (object->getClassName() == TerrainClassName)
//    {
//        models.removeOne(object);
//    }
//    else if (object->getClassName() == AudioClassName)
//    {
//        audios.removeOne(object);
//    }
//    transformCoordinate->setSelectedObject(nullptr);
}

void BBScene::setSelectionRegionVisibility(bool bVisible)
{
    m_pSelectionRegion->setVisibility(bVisible);
}

QList<BBGameObject*> BBScene::getSelectedObjects(QPoint start, QPoint end)
{
    QList<BBGameObject*> result;
    // end cannot exceed the viewport
    if (end.x() < 0)
        end.setX(0);
    else if (end.x() > m_pCamera->getViewportWidth())
        end.setX(m_pCamera->getViewportWidth());
    if (end.y() < 0)
        end.setY(0);
    else if (end.y() > m_pCamera->getViewportHeight())
        end.setY(m_pCamera->getViewportHeight());
    // compute the coordinates of the bottom-left corner, get the smaller x and the larger y
    int x = start.x() < end.x() ? start.x() : end.x();
    int y = start.y() > end.y() ? start.y() : end.y();
    int w = abs(start.x() - end.x());
    int h = abs(start.y() - end.y());
    // 3D ray of the 4 vertexes in the selection region
    BBRay topLeft = m_pCamera->createRayFromScreen(x, y - h);
    BBRay topRight = m_pCamera->createRayFromScreen(x + w, y - h);
    BBRay bottomRight = m_pCamera->createRayFromScreen(x + w, y);
    BBRay bottomLeft = m_pCamera->createRayFromScreen(x, y);

    // The top-left corner is origin, transform origin into the center
    y = -y;
    x = x - m_pCamera->getViewportWidth() / 2;
    y = y + m_pCamera->getViewportHeight() / 2;
    m_pSelectionRegion->setRect(x, y, w, h);

    // Two rays form a plane
    // 4 planes, object in the middle of top bottom left right planes is selected
    QList<BBGameObject*> objects = m_Models;// + directionLights + pointLights + spotLights + audios;
    for (int i = 0; i < objects.count(); i++)
    {
        // whether the bounding box of object is placed in the middle of 4 planes
        if (objects.at(i)->belongToSelectionRegion(
                    topLeft.getNearPoint(), bottomLeft.getNearPoint(), topLeft.getFarPoint(),
                    topLeft.getNearPoint(), topLeft.getFarPoint(), topRight.getNearPoint(),
                    topRight.getNearPoint(), topRight.getFarPoint(), bottomRight.getNearPoint(),
                    bottomLeft.getNearPoint(), bottomRight.getNearPoint(), bottomLeft.getFarPoint()))
        {
            result.append(objects.at(i));
        }
    }

    // return all objects in the selection region
    return result;
}

void BBScene::clear()
{
    QList<BBGameObject*> objects = m_Models;
            //+ directionLights + pointLights + spotLights + audios;
    for (QList<BBGameObject*>::Iterator itr = objects.begin(); itr != objects.end(); itr++)
    {
        BBGameObject *pObject = *itr;
        deleteGameObject(pObject);
    }
}

void BBScene::bindFBO()
{
//    if (m_bEnableFBO && m_pFBO)
//    {
//        m_pFBO->bind();
//    }
}

void BBScene::unbindFBO()
{
//    if (m_bEnableFBO && m_pFBO)
//    {
//        m_pFBO->unbind();
//    }
}




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

