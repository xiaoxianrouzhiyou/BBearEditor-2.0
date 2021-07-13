#include "BBTransformCoordinateSystem.h"
#include "BBCoordinateSystem.h"
#include "BBCoordinateSystem2D.h"
#include "Base/BBGameObjectSet.h"
#include "Scene/BBSceneManager.h"
#include "Render/BBCamera.h"


BBTransformCoordinateSystem::BBTransformCoordinateSystem()
    : BBGameObject()
{
    m_pPositionCoordinateSystem = new BBPositionCoordinateSystem();
    m_pRotationCoordinateSystem = new BBRotationCoordinateSystem();
    m_pScaleCoordinateSystem = new BBScaleCoordinateSystem();

    m_pPositionCoordinateSystem2D = new BBPositionCoordinateSystem2D();
    m_pRotationCoordinateSystem2D = new BBRotationCoordinateSystem2D();
    m_pScaleCoordinateSystem2D = new BBScaleCoordinateSystem2D();

    // default is m_pPositionCoordinateSystem
    m_ModeKey = m_PositionCoordinateSystemModeKey;
    m_pSelectedObject = nullptr;
    m_bTransforming = false;

    switchSpaceMode(Space3D);
}

BBTransformCoordinateSystem::~BBTransformCoordinateSystem()
{
    BB_SAFE_DELETE(m_pPositionCoordinateSystem);
    BB_SAFE_DELETE(m_pRotationCoordinateSystem);
    BB_SAFE_DELETE(m_pScaleCoordinateSystem);
    BB_SAFE_DELETE(m_pPositionCoordinateSystem2D);
    BB_SAFE_DELETE(m_pRotationCoordinateSystem2D);
    BB_SAFE_DELETE(m_pScaleCoordinateSystem2D);
}

void BBTransformCoordinateSystem::init()
{
    m_pPositionCoordinateSystem->init();
    m_pRotationCoordinateSystem->init();
    m_pScaleCoordinateSystem->init();

    m_pPositionCoordinateSystem2D->init();
    m_pRotationCoordinateSystem2D->init();
    m_pScaleCoordinateSystem2D->init();
}

void BBTransformCoordinateSystem::render(BBCamera *pCamera)
{
    if (m_pSelectedObject == NULL)
        return;

    if (m_eSpaceMode == Space3D)
    {
        m_pPositionCoordinateSystem->render(pCamera);
        m_pRotationCoordinateSystem->render(pCamera);
        m_pScaleCoordinateSystem->render(pCamera);
    }
    else
    {
        m_pPositionCoordinateSystem2D->render(pCamera);
        m_pRotationCoordinateSystem2D->render(pCamera);
        m_pScaleCoordinateSystem2D->render(pCamera);
    }
}

void BBTransformCoordinateSystem::resize(float fWidth, float fHeight)
{
    m_pPositionCoordinateSystem2D->resize(fWidth, fHeight);
    m_pRotationCoordinateSystem2D->resize(fWidth, fHeight);
    m_pScaleCoordinateSystem2D->resize(fWidth, fHeight);
}

void BBTransformCoordinateSystem::setSelectedObject(BBGameObject *pObject)
{
    // The selected operation object has not changed, no additional operation is required
    if (m_pSelectedObject == pObject)
    {
        return;
    }
    // Operation object changed

    // Clear multiple options
    for (int i = 0; i < m_SelectedObjects.count(); i++)
    {
        m_SelectedObjects.at(i)->setVisibility(false);
    }
    m_SelectedObjects.clear();

    // single selection
    if (m_pSelectedObject != nullptr)
    {
        // The bounding box and indicator of the original operation object are hidden
        m_pSelectedObject->setVisibility(false);
    }
    if (pObject != nullptr)
    {
        // show bounding box and indicator of now operation object
        pObject->setVisibility(true);

        // UI use 2D Coordinate System
        if (pObject->getClassName() == BB_CLASSNAME_CANVAS)
        {
            switchSpaceMode(Space2D);
        }
        else
        {
            switchSpaceMode(Space3D);
        }
    }
    m_pSelectedObject = pObject;
    setCoordinateSystem(m_ModeKey);
}

void BBTransformCoordinateSystem::setSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet)
{
    // The operation object has not changed, no additional operations are required
    if (m_SelectedObjects == gameObjects)
        return;

    int count = m_SelectedObjects.count();
    for (int i = 0; i < count; i++)
    {
        m_SelectedObjects.at(i)->setVisibility(false);
    }

    count = gameObjects.count();
    for (int i = 0; i < count; i++)
    {
        gameObjects.at(i)->setVisibility(true);
    }
    m_SelectedObjects = gameObjects;
    // select center of gravity
    m_pSelectedObject = pSet;
    setCoordinateSystem(m_ModeKey);
}

/**
 * @brief BBTransformCoordinateSystem::mouseMoveEvent
 * @param x                                                     original point is the center of screen
 * @param y
 * @param ray
 * @param bMousePressed
 * @return
 */
bool BBTransformCoordinateSystem::mouseMoveEvent(int x, int y, const BBRay &ray, bool bMousePressed)
{
    m_bTransforming = false;

    if (m_eSpaceMode == Space3D)
    {
        if (m_ModeKey == m_PositionCoordinateSystemModeKey)
        {
            m_bTransforming = m_pPositionCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
        }
        else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
        {
            m_bTransforming = m_pRotationCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
        }
        else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
        {
            m_bTransforming = m_pScaleCoordinateSystem->mouseMoveEvent(ray, bMousePressed);
        }
    }
    else
    {
        BBSceneManager::getCamera()->switchCoordinate(x, y);
        if (m_ModeKey == m_PositionCoordinateSystemModeKey)
        {
            m_bTransforming = m_pPositionCoordinateSystem2D->mouseMoveEvent(x, y, bMousePressed);
        }
        else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
        {
            m_bTransforming = m_pRotationCoordinateSystem2D->mouseMoveEvent(x, y, bMousePressed);
        }
        else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
        {
            m_bTransforming = m_pScaleCoordinateSystem2D->mouseMoveEvent(x, y, bMousePressed);
        }
    }

    // if false, other mouse events will be processed
    return m_bTransforming;
}

void BBTransformCoordinateSystem::setCoordinateSystem(char modeKey)
{
    m_ModeKey = modeKey;

    if (m_eSpaceMode == Space3D)
    {
        if (m_ModeKey == m_PositionCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem->setSelectedObject(m_pSelectedObject);
            m_pRotationCoordinateSystem->setSelectedObject(nullptr);
            m_pScaleCoordinateSystem->setSelectedObject(nullptr);
        }
        else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem->setSelectedObject(nullptr);
            m_pRotationCoordinateSystem->setSelectedObject(m_pSelectedObject);
            m_pScaleCoordinateSystem->setSelectedObject(nullptr);
        }
        else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem->setSelectedObject(nullptr);
            m_pRotationCoordinateSystem->setSelectedObject(nullptr);
            m_pScaleCoordinateSystem->setSelectedObject(m_pSelectedObject);
        }
    }
    else
    {
        if (m_ModeKey == m_PositionCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem2D->setSelectedObject(m_pSelectedObject);
            m_pRotationCoordinateSystem2D->setSelectedObject(nullptr);
            m_pScaleCoordinateSystem2D->setSelectedObject(nullptr);
        }
        else if (m_ModeKey == m_RotationCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem2D->setSelectedObject(nullptr);
            m_pRotationCoordinateSystem2D->setSelectedObject(m_pSelectedObject);
            m_pScaleCoordinateSystem2D->setSelectedObject(nullptr);
        }
        else if (m_ModeKey == m_ScaleCoordinateSystemModeKey)
        {
            m_pPositionCoordinateSystem2D->setSelectedObject(nullptr);
            m_pRotationCoordinateSystem2D->setSelectedObject(nullptr);
            m_pScaleCoordinateSystem2D->setSelectedObject(m_pSelectedObject);
        }
    }
}

void BBTransformCoordinateSystem::stopTransform()
{
    m_pPositionCoordinateSystem->stopTransform();
    m_pRotationCoordinateSystem->stopTransform();
    m_pScaleCoordinateSystem->stopTransform();

    m_pPositionCoordinateSystem2D->stopTransform();
    m_pRotationCoordinateSystem2D->stopTransform();
    m_pScaleCoordinateSystem2D->stopTransform();

    m_bTransforming = false;
}

void BBTransformCoordinateSystem::update()
{
    setCoordinateSystem(m_ModeKey);
}

void BBTransformCoordinateSystem::switchSpaceMode(const BBCoordinateSystemSpaceMode &eMode)
{
    m_eSpaceMode = eMode;
    if (m_eSpaceMode == Space3D)
    {

    }
    else
    {

    }
}
