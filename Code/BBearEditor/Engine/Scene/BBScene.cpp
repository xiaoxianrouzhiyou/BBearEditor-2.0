#include "BBScene.h"
#include "Utils/BBUtils.h"
#include "Render/BufferObject/BBFrameBufferObject.h"
#include "Render/BBCamera.h"
#include "3D/BBSkyBox.h"
#include "3D/BBHorizontalPlane.h"
#include "Base/BBGameObject.h"
#include "3D/BBModel.h"
#include "Geometry/BBRay.h"
#include "2D/BBSelectionRegion.h"
#include <cfloat>
#include "CoordinateSystem/BBTransformCoordinateSystem.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBRenderState.h"
#include "Render/BBDrawCall.h"
#include "Lighting/GameObject/BBLight.h"
#include "Lighting/GameObject/BBDirectionalLight.h"
#include "Lighting/GameObject/BBPointLight.h"
#include "Lighting/GameObject/BBSpotLight.h"
#include "Scene/BBRendererManager.h"
#include "2D/BBCanvas.h"
#include "2D/BBSpriteObject2D.h"
#include "ParticleSystem/BBParticleSystem.h"
#include "RayTracing/BBRayTracker.h"
#include "Render/Texture/BBTexture.h"
#include "Render/BBRenderQueue.h"
#include "3D/BBNormalIndicator.h"


BBScene::BBScene()
{
    m_RenderingFunc = &BBScene::defaultRendering;
    m_pRenderQueue = nullptr;
    m_fUpdateRate = (float) BB_CONSTANT_UPDATE_RATE / 1000;
    for (int i = 0; i < 3; i++)
    {
        m_pFBO[i] = nullptr;
        m_pFullScreenQuad[i] = nullptr;
    }
    m_pCamera = nullptr;
    m_pSkyBox = nullptr;
    m_pHorizontalPlane = nullptr;
    m_pSelectionRegion = nullptr;
    m_pTransformCoordinateSystem = nullptr;
    m_pTiledFullScreenQuad = nullptr;
    m_pNormalIndicator = nullptr;
    m_pFixedSizeFBO = nullptr;
}

BBScene::~BBScene()
{
    for (int i = 0; i < 3; i++)
    {
        BB_SAFE_DELETE(m_pFBO[i]);
        BB_SAFE_DELETE(m_pFullScreenQuad[i]);
    }
    BB_SAFE_DELETE(m_pCamera);
    BB_SAFE_DELETE(m_pRenderQueue);
    BB_SAFE_DELETE(m_pSkyBox);
    BB_SAFE_DELETE(m_pHorizontalPlane);
    BB_SAFE_DELETE(m_pSelectionRegion);
    BB_SAFE_DELETE(m_pTransformCoordinateSystem);
    BB_SAFE_DELETE(m_pTiledFullScreenQuad);
    BB_SAFE_DELETE(m_pNormalIndicator);
    BB_SAFE_DELETE(m_pFixedSizeFBO);
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

    m_pCamera = new BBCamera();
    m_pRenderQueue = new BBRenderQueue(m_pCamera);

    m_pSkyBox = new BBSkyBox();
    m_pHorizontalPlane = new BBHorizontalPlane();
    m_pSelectionRegion = new BBSelectionRegion();
    m_pTransformCoordinateSystem = new BBTransformCoordinateSystem();
    m_pTiledFullScreenQuad = new BBTiledFullScreenQuad();
    m_pNormalIndicator = new BBNormalIndicator();

    m_pCamera->setViewportSize(800.0f, 600.0f);

    m_pSkyBox->init(QString(BB_PATH_RESOURCE) + "skyboxs/3/");
    m_pHorizontalPlane->init();
    m_pTransformCoordinateSystem->init();

    m_pTiledFullScreenQuad->init();
    for (int i = 0; i < 3; i++)
    {
        m_pFullScreenQuad[i] = new BBFullScreenQuad();
        m_pFullScreenQuad[i]->init();
    }

    // init environment FBO
    m_pFixedSizeFBO = new BBFrameBufferObject();
    m_pFixedSizeFBO->attachColorBuffer(FBO_COLOR_BUFFER_NAME(0), GL_COLOR_ATTACHMENT0, BBSkyBox::m_nEnvironmentMapSize, BBSkyBox::m_nEnvironmentMapSize, GL_RGBA32F);
    m_pFixedSizeFBO->attachDepthBuffer(FBO_DEPTH_BUFFER_NAME, BBSkyBox::m_nEnvironmentMapSize, BBSkyBox::m_nEnvironmentMapSize);
    m_pFixedSizeFBO->finish();
}

void BBScene::render()
{
    (this->*m_RenderingFunc)();
}

void BBScene::defaultRendering()
{
    // 3D camera mode
    m_pCamera->switchTo3D();
    // refresh camera position and direction, update pos and ..., Convenient for subsequent use
    m_pCamera->update(m_fUpdateRate);

    writeViewSpaceFBO(0);
    writeShadowMap(1);

    m_pSkyBox->render(m_pCamera);
    m_pHorizontalPlane->render(m_pCamera);

    // BBGameObject
    m_pRenderQueue->render();

    m_pNormalIndicator->render(m_pCamera);

    for (QList<BBGameObject*>::Iterator itr = m_Lights.begin(); itr != m_Lights.end(); itr++)
    {
        ((BBLight*)(*itr))->render(m_pCamera);
    }

    for (QList<BBGameObject*>::Iterator itr = m_Canvases.begin(); itr != m_Canvases.end(); itr++)
    {
        ((BBCanvas*)(*itr))->render();
    }

    for (QList<BBGameObject*>::Iterator itr = m_ParticleSystems.begin(); itr != m_ParticleSystems.end(); itr++)
    {
        ((BBParticleSystem*)(*itr))->render(m_pCamera);
    }

    m_pTransformCoordinateSystem->render(m_pCamera);

    // test
//    BBMaterial *pMaterial = new BBMaterial();
//    pMaterial->init("texture", BB_PATH_RESOURCE_SHADER(texture.vert), BB_PATH_RESOURCE_SHADER(texture.frag));
//    pMaterial->setSampler2D(LOCATION_TEXTURE(0), m_pFixedSizeFBO->getBuffer(FBO_COLOR_BUFFER_NAME(0)));
//    m_pFullScreenQuad[0]->setCurrentMaterial(pMaterial);
//    m_pFullScreenQuad[0]->render(m_pCamera);

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

void BBScene::deferredRendering()
{
    m_pCamera->switchTo3D();
    m_pCamera->update(m_fUpdateRate);

    writeShadowMap(2);

    m_pFBO[0]->bind();
    m_pSkyBox->render(m_pCamera);
    // BBGameObject
    m_pRenderQueue->render();
    m_pFBO[0]->unbind();

    m_pFullScreenQuad[0]->render(m_pCamera);
}

void BBScene::deferredRenderingAndPostProcessing()
{
    m_pCamera->switchTo3D();
    m_pCamera->update(m_fUpdateRate);

    writeShadowMap(2);

    m_pFBO[0]->bind();
    m_pSkyBox->render(m_pCamera);
    // BBGameObject
    m_pRenderQueue->render();
    m_pFBO[0]->unbind();

    // Post processing
    m_pFBO[1]->bind();
    m_pFullScreenQuad[0]->render(m_pCamera);
    m_pFBO[1]->unbind();

    m_pFullScreenQuad[1]->render(m_pCamera);
}

void BBScene::resize(float width, float height)
{
    // 3D camera, resize
    m_pCamera->setViewportSize(width, height);

//    m_pTiledFullScreenQuad->setTiledAABB(width, height);

    for (int i = 0; i < 3; i++)
    {
        if (!m_pFBO[i])
            BB_SAFE_DELETE(m_pFBO[i]);
        m_pFBO[i] = new BBFrameBufferObject;
        m_pFBO[i]->attachColorBuffer(FBO_COLOR_BUFFER_NAME(0), GL_COLOR_ATTACHMENT0, width, height, GL_RGBA32F);
        m_pFBO[i]->attachColorBuffer(FBO_COLOR_BUFFER_NAME(1), GL_COLOR_ATTACHMENT1, width, height, GL_RGBA32F);
        m_pFBO[i]->attachColorBuffer(FBO_COLOR_BUFFER_NAME(2), GL_COLOR_ATTACHMENT2, width, height, GL_RGBA32F);
        m_pFBO[i]->attachColorBuffer(FBO_COLOR_BUFFER_NAME(3), GL_COLOR_ATTACHMENT3, width, height, GL_RGBA32F);
        m_pFBO[i]->attachDepthBuffer(FBO_DEPTH_BUFFER_NAME, width, height);
        m_pFBO[i]->finish();
    }

    for (QList<BBGameObject*>::Iterator itr = m_Canvases.begin(); itr != m_Canvases.end(); itr++)
    {
        ((BBCanvas*)(*itr))->resize(width, height);
    }

    // used for computing lighting
    writeSkyBoxCubeMap();
}

GLuint BBScene::getColorFBO(int nFBOIndex, int nAttachmentIndex)
{
    return m_pFBO[nFBOIndex]->getBuffer(FBO_COLOR_BUFFER_NAME() + QString::number(nAttachmentIndex));
}

GLuint BBScene::getDepthFBO(int nFBOIndex)
{
    return m_pFBO[nFBOIndex]->getBuffer(FBO_DEPTH_BUFFER_NAME);
}

void BBScene::setSkyBox(const QString &path)
{
    m_pSkyBox->changeResource(path);
}

void BBScene::changeSkyBoxAlgorithm(int nAlgorithmIndex)
{
    m_pSkyBox->changeAlgorithm(nAlgorithmIndex);
}

void BBScene::enableSkyBox(bool bEnable)
{
    m_pSkyBox->setVisibility(bEnable);
}

void BBScene::enableHorizontalPlane(bool bEnable)
{
    m_pHorizontalPlane->setVisibility(bEnable);
}

BBModel* BBScene::createModel(const QString &userData, int x, int y)
{
    BBRay ray = m_pCamera->createRayFromScreen(x, y);
    // ground y=0
    QVector3D hit = ray.computeIntersectWithXOZPlane(0);
    return createModel(userData, hit);
}

BBModel* BBScene::createModel(const QString &userData,
                              const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
{
    BBModel *pModel = nullptr;
    if (userData == BB_CLASSNAME_TERRAIN)
    {
        pModel = new BBModel(position, rotation, scale, BBMeshType::TERRAIN);
        pModel->setBaseAttributes(QFileInfo(userData).baseName(), BB_CLASSNAME_TERRAIN, BB_CLASSNAME_TERRAIN);
        pModel->init(BB_PATH_RESOURCE_PICTURE(height.png));
    }
    else
    {
        pModel = new BBModel(position, rotation, scale, BBMeshType::OBJ);
        pModel->setBaseAttributes(QFileInfo(userData).baseName(), BB_CLASSNAME_MODEL, BB_CLASSNAME_MODEL);
        pModel->init(userData);
    }
    pModel->insertInRenderQueue(m_pRenderQueue);
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
    BBLight *pLight = nullptr;
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

    // open light for materials
    m_pTiledFullScreenQuad->openLight();

    return pLight;
}

BBCanvas* BBScene::createCanvas(int x, int y, bool bSelect)
{
    // change original point
    m_pCamera->switchCoordinate(x, y);
    BBCanvas *pCanvas = new BBCanvas(x, y);
    pCanvas->setBaseAttributes(BB_CLASSNAME_CANVAS, BB_CLASSNAME_CANVAS, "ground");
    pCanvas->init();
    pCanvas->resize(m_pCamera->getViewportWidth(), m_pCamera->getViewportHeight());
    m_Canvases.append(pCanvas);
    return pCanvas;
}

BBSpriteObject2D* BBScene::createSpriteObject2D(BBCanvas *pCanvas, int x, int y, bool bSelect)
{
    m_pCamera->switchCoordinate(x, y);
    BBSpriteObject2D *pSpriteObject2D = new BBSpriteObject2D(x, y);
    pSpriteObject2D->setBaseAttributes(BB_CLASSNAME_SPRITEOBJECT2D, BB_CLASSNAME_SPRITEOBJECT2D, "ground");
    pSpriteObject2D->init();
    pCanvas->addSpriteObject2D(pSpriteObject2D);
    return pSpriteObject2D;
}

BBParticleSystem* BBScene::createParticleSystem(int x, int y, bool bSelect)
{
    BBRay ray = m_pCamera->createRayFromScreen(x, y);
    // ground y=0
    QVector3D hit = ray.computeIntersectWithXOZPlane(0);

    BBParticleSystem *pParticleSystem = new BBParticleSystem(hit);
    pParticleSystem->setBaseAttributes(BB_CLASSNAME_PARTICLE, BB_CLASSNAME_PARTICLE, "particle white");
    pParticleSystem->init();

    m_ParticleSystems.append(pParticleSystem);

    return pParticleSystem;
}

bool BBScene::hitCanvas(int x, int y, BBCanvas *&pOutCanvas)
{
    m_pCamera->switchCoordinate(x, y);
    pOutCanvas = nullptr;
    for (QList<BBGameObject*>::Iterator itr = m_Canvases.begin(); itr != m_Canvases.end(); itr++)
    {
        if ((*itr)->hit(x, y))
        {
            pOutCanvas = (BBCanvas*)(*itr);
            return true;
        }
    }
    return false;
}

BBGameObject* BBScene::pickObject(const QList<BBGameObject*> &alternativeObjects, const BBRay &ray, bool bSelect)
{
    float fDistance = FLT_MAX;
    BBGameObject *pSelectedObject = nullptr;
    // traverse models and lights
    QList<BBGameObject*> objects = alternativeObjects;
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
//    if (!bSelect)
//    {
//        m_pTransformCoordinateSystem->setSelectedObject(nullptr);
//    }

    return pSelectedObject;
}

BBGameObject* BBScene::pickObjectInAllObjects(const BBRay &ray, bool bSelect)
{
    return pickObject(m_Models + m_Lights, ray, bSelect);
}

BBGameObject* BBScene::pickObjectInModels(const BBRay &ray, bool bSelect)
{
    return pickObject(m_Models, ray, bSelect);
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
        ((BBModel*)pGameObject)->removeFromRenderQueue(m_pRenderQueue);
        m_Models.removeOne(pGameObject);
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_DIRECTIONAL_LIGHT
             || pGameObject->getClassName() == BB_CLASSNAME_POINT_LIGHT
             || pGameObject->getClassName() == BB_CLASSNAME_SPOT_LIGHT)
    {
        ((BBLight*)pGameObject)->removeFromRenderQueue(m_pRenderQueue);

        m_Lights.removeOne(pGameObject);

        if (m_Lights.count() == 0)
        {
            m_pTiledFullScreenQuad->closeLight();
        }
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_CANVAS)
    {
        m_Canvases.removeOne(pGameObject);
    }
    else if (pGameObject->getClassName() == BB_CLASSNAME_PARTICLE)
    {
        m_ParticleSystems.removeOne(pGameObject);
    }
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
    BBFrustum frustum(m_pCamera, x, y - h, w, h);

    // Two rays form a plane
    // 4 planes, object in the middle of top bottom left right planes is selected
    QList<BBGameObject*> objects = m_Models + m_Lights;
    for (int i = 0; i < objects.count(); i++)
    {
        // whether the bounding box of object is placed in the middle of 4 planes
        if (objects.at(i)->belongToSelectionRegion(frustum))
        {
            result.append(objects.at(i));
        }
    }

    // The top-left corner is origin, transform origin into the center
    y = -y;
    x = x - m_pCamera->getViewportWidth() / 2;
    y = y + m_pCamera->getViewportHeight() / 2;
    m_pSelectionRegion->setRect(x, y, w, h);

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

void BBScene::setFullScreenQuadTexture(const std::string &uniformName, GLuint textureName)
{
    m_pTiledFullScreenQuad->setTexture(uniformName, textureName);
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

void BBScene::writeSkyBoxCubeMap()
{
    m_pFixedSizeFBO->bind();
    m_pSkyBox->writeCubeMap(m_pCamera);
    m_pFixedSizeFBO->unbind();
}

void BBScene::writeViewSpaceFBO(int nIndex)
{
    BBDrawCall::switchRenderingSettings(1);
    m_pFBO[nIndex]->bind();

    m_pSkyBox->render(m_pCamera);
    // BBGameObject
    m_pRenderQueue->render();

    m_pFBO[nIndex]->unbind();
    BBDrawCall::switchRenderingSettings(0);
}

void BBScene::writeShadowMap(int nIndex)
{
    BBDrawCall::switchRenderingSettings(2);
    m_pFBO[nIndex]->bind();
    // BBGameObject
    for (QList<BBGameObject*>::Iterator it = m_Models.begin(); it != m_Models.end(); it++)
    {
        ((BBModel*)(*it))->renderToShadowMap(m_pCamera);
    }

    m_pFBO[nIndex]->unbind();
    BBDrawCall::switchRenderingSettings(0);
}

