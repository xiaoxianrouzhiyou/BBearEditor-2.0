#include "BBEditViewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Render/BBCamera.h"
#include <QMouseEvent>
#include "Utils/BBUtils.h"
#include "Base/BBGameObject.h"
#include <QMimeData>
#include "3D/Model/BBModel.h"
#include <QDrag>
#include "Geometry/BBRay.h"
#include "CoordinateSystem/BBTransformCoordinateSystem.h"
#include <QTreeWidgetItem>
#include "Scene/BBSceneManager.h"
#include "Render/Light/BBLight.h"
#include "2D/BBCanvas.h"
#include "2D/BBSprite2D.h"


BBEditViewOpenGLWidget::BBEditViewOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    BBSceneManager::bindEditViewOpenGLWidget(this);

    m_bRightPressed = false;
    m_pCurrentCanvas = nullptr;
    m_pPreviewObject = nullptr;
    setAcceptDrops(true);
    // Mouse events can be captured without pressing
    setMouseTracking(true);
    m_bRegionSelecting = false;
    // Single selection mode
    m_bMultipleSelecting = false;

    setRenderThread();

//    //右下角的浏览视图
//    QHBoxLayout *l = new QHBoxLayout(this);
//    l->setMargin(0);
//    mPreview = new BaseOpenGLWidget(this);
//    mPreview->setMinimumSize(150, 100);
//    l->addWidget(mPreview, 1, Qt::AlignBottom | Qt::AlignRight);
//    mPreview->hide();
}

BBEditViewOpenGLWidget::~BBEditViewOpenGLWidget()
{
    BB_SAFE_DELETE(m_pPreviewObject);
    m_pRenderThread->quit();
    m_pRenderThread->wait();
    BB_SAFE_DELETE(m_pRenderThread);
}

BBGameObject* BBEditViewOpenGLWidget::createModel(const BBSerializer::BBGameObject &gameObject)
{
    BBGameObject *pResult = m_pScene->createModel(gameObject);
    addGameObject(pResult);
    return pResult;
}

void BBEditViewOpenGLWidget::setRenderThread()
{
    m_pRenderThread = new QThread(this);
    m_pRenderTimer = new QTimer();
    m_pRenderTimer->setInterval(BB_CONSTANT_UPDATE_RATE);
    m_pRenderTimer->moveToThread(m_pRenderThread);

    QObject::connect(m_pRenderThread, SIGNAL(started()), m_pRenderTimer, SLOT(start()));
    QObject::connect(m_pRenderTimer, SIGNAL(timeout()), this, SLOT(update()), Qt::DirectConnection);
    QObject::connect(m_pRenderThread, SIGNAL(finished()), m_pRenderTimer, SLOT(deleteLater()));

    m_pRenderThread->start();
}

void BBEditViewOpenGLWidget::pressESC()
{
    setCoordinateSystemSelectedObject(nullptr);
}

void BBEditViewOpenGLWidget::pressMoveKey(char key)
{
    // Handling camera movement
    if (!m_bRightPressed)
        return;
//    qDebug() << key << "true";
    m_pScene->getCamera()->move(key, true);
}

void BBEditViewOpenGLWidget::releaseMoveKey(char key)
{
    if (!m_bRightPressed)
        return;
//    qDebug() << key << "false";
    m_pScene->getCamera()->move(key, false);
}

void BBEditViewOpenGLWidget::pressTransform(char key)
{
    // If the camera is processed, the coordinate system transform is not processed
    if (m_bRightPressed)
        return;
    m_pScene->getTransformCoordinateSystem()->setCoordinateSystem(key);
}

void BBEditViewOpenGLWidget::setCoordinateSystemSelectedObject(BBGameObject *pGameObject)
{
    m_pScene->getTransformCoordinateSystem()->setSelectedObject(pGameObject);
}

void BBEditViewOpenGLWidget::setCoordinateSystemSelectedObjects(const QList<BBGameObject*> &gameObjects, BBGameObjectSet *pSet)
{
    m_pScene->getTransformCoordinateSystem()->setSelectedObjects(gameObjects, pSet);
}

void BBEditViewOpenGLWidget::pressMultipleSelectionKey(bool bPressed)
{
    // switch single or multiple selection mode
    m_bMultipleSelecting = bPressed;
}

void BBEditViewOpenGLWidget::updateCoordinateSystem()
{
    m_pScene->getTransformCoordinateSystem()->update();
}

void BBEditViewOpenGLWidget::createModelAtOrigin(const QString &filePath)
{
    // Send signal to treeHierarchy
    addGameObject(m_pScene->createModel(filePath));
}

void BBEditViewOpenGLWidget::createLightAtOrigin(const QString &fileName)
{
    addGameObject(m_pScene->createLight(fileName));
}

void BBEditViewOpenGLWidget::deleteGameObject(BBGameObject *pGameObject)
{
    m_pScene->deleteGameObject(pGameObject);
}

void BBEditViewOpenGLWidget::copyGameObject(BBGameObject *pSourceObject, QTreeWidgetItem *pTranscriptItem)
{
    BBGameObject *pTranscriptObject = NULL;
    if (pSourceObject->getClassName() == BB_CLASSNAME_MODEL)
    {
        pTranscriptObject = m_pScene->createModel(pSourceObject->getFilePath(), pSourceObject->getPosition());
    }
//    else if (sourceObject->getClassName() == DirectionLightClassName
//             || sourceObject->getClassName() == PointLightClassName
//             || sourceObject->getClassName() == SpotLightClassName)
//    {
//        //拷贝一个灯光对象
//        transcriptObject = scene.createLight(sourceObject->getFilePath(), position);
//    }
    else
    {

    }
    BB_PROCESS_ERROR_RETURN(pTranscriptObject);
    pTranscriptObject->setName(pTranscriptItem->text(0));
    pTranscriptObject->setActivity(pSourceObject->getActivity());
    // then, handle in HierarchyTreeWidget
    addGameObject(pTranscriptObject, pTranscriptItem);
}

void BBEditViewOpenGLWidget::lookAtGameObject(BBGameObject *pGameObject)
{
    m_pScene->lookAtGameObject(pGameObject);
}

void BBEditViewOpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        m_bRightPressed = true;
        m_OriginalMousePos = e->globalPos();
        setCursor(Qt::BlankCursor);
    }
    else if (e->button() == Qt::LeftButton)
    {
        // Start dragging for selection area, and record the starting point
        m_SelectionRegionStartingPoint = e->pos();
    }
}

void BBEditViewOpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::RightButton)
    {
        QPoint currentPos = e->globalPos();
        float deltaX = currentPos.x() - m_OriginalMousePos.x();
        float deltaY = currentPos.y() - m_OriginalMousePos.y();
        // rotate camera
        // Otherwise, the farther the current moves, the larger the delta
        QCursor::setPos(m_OriginalMousePos);
        //qDebug() << "rotate camera";
        float angleRotatedByUp = deltaX / 1000;
        float angleRotatedByRight = deltaY / 1000;
        m_pScene->getCamera()->yaw(-angleRotatedByUp);
        m_pScene->getCamera()->pitch(-angleRotatedByRight);
    }
    else if (e->buttons() & Qt::LeftButton)
    {
        if (m_bRegionSelecting)
        {
            // do not perform transform of gameobject
            // show selection region, and select gameobjects
            m_pScene->setSelectionRegionVisibility(true);
            pickObjects(m_pScene->getSelectedObjects(m_SelectionRegionStartingPoint, e->pos()));
        }
        else
        {
            // do not perform selection operation
            BBRay ray = m_pScene->getCamera()->createRayFromScreen(e->pos().x(), e->pos().y());
            if (m_pScene->getTransformCoordinateSystem()->mouseMoveEvent(e->pos().x(), e->pos().y(), ray, true))
            {
                // update value in property manager
                updateTransformInPropertyManager(m_pScene->getTransformCoordinateSystem()->getSelectedObject(),
                                                 m_pScene->getTransformCoordinateSystem()->getTransformModeKey());
            }
            else
            {
                // if also do not perform transform, determine whether turn on m_bRegionSelecting
                // If the mouse moves only a small distance, it is not considered as selection operation
                QPoint delta = e->pos() - m_SelectionRegionStartingPoint;
                static int nThreshold = 49;
                if ((delta.x() * delta.x() + delta.y() * delta.y()) > nThreshold)
                {
                    m_bRegionSelecting = true;
                }
            }
        }
    }
    else
    {
        // move mouse without press mouse
        BBRay ray = m_pScene->getCamera()->createRayFromScreen(e->pos().x(), e->pos().y());
        m_pScene->getTransformCoordinateSystem()->mouseMoveEvent(e->pos().x(), e->pos().y(), ray, false);
    }
}

void BBEditViewOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        // end camera movement
        m_bRightPressed = false;
        QCursor::setPos(m_OriginalMousePos);
        setCursor(Qt::ArrowCursor);
        m_pScene->getCamera()->resetMove();
    }
    else if (e->button() == Qt::LeftButton)
    {
        BBRay ray = m_pScene->getCamera()->createRayFromScreen(e->pos().x(), e->pos().y());
        // if it is the release at the end of the transform and selection operation
        // there is no need to pick object
        if (!m_pScene->getTransformCoordinateSystem()->isTransforming() && !m_bRegionSelecting)
        {
            // 3D pick objects
            BBGameObject *pObject = m_pScene->pickObject(ray);
            // send signals, show related imformation in Hierarchy tree and inspector
            if (m_bMultipleSelecting)
            {
                // If the object is not NULL
                // add it to the multi-selection objects (or subtract it when it is already selected)
                if (pObject)
                {
                    updateMultipleSelectedObjects(pObject);
                }
            }
            else
            {
                // Single selection
                pickObject(pObject);
            }
        }
        // When the coordinate system is no longer moved, reset m_LastMousePos
        // stop m_bTransforming, can pick object in the next mouse release
        m_pScene->getTransformCoordinateSystem()->stopTransform();

        // When the coordinate system is moved, the mouse leaves the coordinate system
        // If the ray is not updated when released, the selected coordinate axis cannot be restored to its original color
        m_pScene->getTransformCoordinateSystem()->mouseMoveEvent(e->pos().x(), e->pos().y(), ray, false);

        // Exit selection mode
        m_pScene->setSelectionRegionVisibility(false);
        m_bRegionSelecting = false;
    }
}

void BBEditViewOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    m_pScene->getCamera()->setMoveSpeed(event->delta() > 0 ? 1 : -1);
}

void BBEditViewOpenGLWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QByteArray data;
    if ((data = event->mimeData()->data(BB_MIMETYPE_BASEOBJECT)) != nullptr)
    {
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        m_DragType.clear();
        dataStream >> m_DragType;
        if (m_DragType == "terrain")
        {
//            //创建地形
//            prepareObject = scene.createModel(fileName, event->pos().x(), event->pos().y());
        }
        else if (m_DragType == BB_CLASSNAME_CANVAS)
        {
            m_pPreviewObject = m_pScene->createCanvas(event->pos().x(), event->pos().y());
        }
        else if (m_DragType == BB_CLASSNAME_SPRITE2D)
        {

        }
        else
        {
            // Create a temporary object to show drag effect
            // no need to create the corresponding item in the hierarchical tree
            m_pPreviewObject = m_pScene->createModel(BB_PATH_RESOURCE_MESH() + m_DragType, event->pos().x(), event->pos().y());
        }
        // Remove the selected state of the coordinate system
        setCoordinateSystemSelectedObject(NULL);
        event->accept();
    }
    else if ((data = event->mimeData()->data(BB_MIMETYPE_LIGHTOBJECT)) != nullptr)
    {
        event->accept();
    }
//    else if ((data = event->mimeData()->data(FileList::getMimeType())) != nullptr)
//    {
//        //拖入资源文件
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString filePath;
//        dataStream >> filePath;
//        QString suffix = filePath.mid(filePath.lastIndexOf('.') + 1);
//        if (FileList::meshSuffixs.contains(suffix))
//        {
//            //拖入obj模型文件 创建预览模型 无需在层级视图内创建对应的对象
//            prepareObject = scene.createModel(filePath, event->pos().x(), event->pos().y());
//            //移除坐标轴选中状态
//            scene.transformCoordinate->setSelectedObject(nullptr);
//            event->accept();
//        }
//        else if (suffix == "mtl")
//        {
//            //拖入材质
//            event->accept();
//        }
//        else if (FileList::textureSuffixs.contains(suffix))
//        {
//            event->accept();
//        }
//        else if (FileList::audioSuffixs.contains(suffix))
//        {
//            event->accept();
//        }
//        else if (FileList::scriptSuffixs.contains(suffix))
//        {
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
//    }
    else
    {
        event->ignore();
    }
}

void BBEditViewOpenGLWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    if (m_pPreviewObject)
    {
        if (m_pPreviewObject->getClassName() == BB_CLASSNAME_MODEL)
        {
            BBRay ray = m_pScene->getCamera()->createRayFromScreen(event->pos().x(), event->pos().y());
            m_pPreviewObject->setPosition(ray.computeIntersectWithXOZPlane(0));
        }
        else if (m_pPreviewObject->getClassName() == BB_CLASSNAME_CANVAS)
        {
            m_pPreviewObject->setScreenCoordinateWithSwitchingOriginalPoint(event->pos().x(), event->pos().y());
        }
    }
    else if (m_DragType == BB_CLASSNAME_SPRITE2D)
    {
        m_pCurrentCanvas = nullptr;
        if (!m_pScene->hitCanvas(event->pos().x(), event->pos().y(), m_pCurrentCanvas))
        {
            event->ignore();
        }
    }
}

void BBEditViewOpenGLWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    if (m_pPreviewObject)
    {
        // no longer show pre-created object
        m_pScene->deleteGameObject(m_pPreviewObject);
        m_pPreviewObject = nullptr;
    }

    event->accept();
}

void BBEditViewOpenGLWidget::dropEvent(QDropEvent *event)
{
    QByteArray data;
    if ((data = event->mimeData()->data(BB_MIMETYPE_BASEOBJECT)) != nullptr)
    {
        if (m_pPreviewObject)
        {
            // Show item in hierarchical tree
            // need the item so that set local coordinate
            addGameObject(m_pPreviewObject);
            // Set to empty for the next calculation
            m_pPreviewObject = nullptr;
        }
        else if (m_pCurrentCanvas)
        {
            BBSprite2D *pSprite2D = m_pScene->createSprite2D(m_pCurrentCanvas, event->pos().x(), event->pos().y());
            addGameObject(pSprite2D);
            // need to make Sprite2D be the child of canvas
            BBSceneManager::addSprite2DForCanvas(m_pCurrentCanvas, pSprite2D);
            // Set to empty for the next calculation
            m_pCurrentCanvas = nullptr;
        }
        event->accept();
        setFocus();
    }
    else if ((data = event->mimeData()->data(BB_MIMETYPE_LIGHTOBJECT)) != nullptr)
    {
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        QString fileName;
        dataStream >> fileName;
        BBGameObject *pLight = m_pScene->createLight(fileName, event->pos().x(), event->pos().y());
        addGameObject(pLight);
        event->accept();
        setFocus();
    }
//    else if ((data = event->mimeData()->data(FileList::getMimeType())) != nullptr)
//    {
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString filePath;
//        dataStream >> filePath;
//        QString suffix = filePath.mid(filePath.lastIndexOf('.') + 1);
//        if (FileList::meshSuffixs.contains(suffix))
//        {
//            if (prepareObject)
//            {
//                //落下后 预创建对象成为正式创建的对象 坐标轴选中它
//                scene.transformCoordinate->setSelectedObject(prepareObject);
//                //在层级视图中显示结点
//                addGameObjectSignal(prepareObject);
//                //设为空 用于下次计算
//                prepareObject = NULL;
//                event->accept();
//            }
//            else
//            {
//                event->ignore();
//            }
//        }
//        else if (suffix == "mtl")
//        {
//            //材质文件
//            //拾取落下位置的对象 赋予材质
//            Model *model = scene.pickModel(event->pos().x(), event->pos().y());
//            if (scene.setModelMaterial(model, filePath))
//            {
//                //修改对象属性栏的材质属性
//                updateMaterialProperty(model);
//                event->accept();
//            }
//            else
//            {
//                event->ignore();
//            }
//        }
//        else if (FileList::audioSuffixs.contains(suffix))
//        {
//            Audio *audio = scene.createAudio(filePath, event->pos().x(), event->pos().y());
//            addGameObjectSignal(audio);
//            event->accept();
//        }
//        else
//        {
//            //拖入是其他文件
//            qDebug() << "test";
//            event->accept();
//        }
//    }
    else
    {
        event->ignore();
    }
}
