#include "BBEditViewOpenGLWidget.h"
#include "BBScene.h"
#include "BBCamera.h"
#include <QMouseEvent>
#include "BBUtils.h"
#include "BBGameObject.h"
#include <QMimeData>
#include "BBModel.h"
#include <QDrag>
#include "BBRay.h"

BBEditViewOpenGLWidget::BBEditViewOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    m_bRightPressed = false;
    m_pPreviewObject = NULL;
    setAcceptDrops(true);
    // Mouse events can be captured without pressing
    setMouseTracking(true);
    m_bRegionSelecting = false;

//    //单选模式
//    isMultipleSelect = false;

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
}

void BBEditViewOpenGLWidget::pressESCSlot()
{
//    scene.transformCoordinate->setSelectedObject(nullptr);
}

void BBEditViewOpenGLWidget::pressMoveKeySlot(char key)
{
    // Handling camera movement
    if (!m_bRightPressed)
        return;
//    qDebug() << key << "true";
    m_pScene->getCamera()->move(key, true);
}

void BBEditViewOpenGLWidget::releaseMoveKeySlot(char key)
{
    if (!m_bRightPressed)
        return;
//    qDebug() << key << "false";
    m_pScene->getCamera()->move(key, false);
}

void BBEditViewOpenGLWidget::pressTransformSlot(char key)
{
//    //处理摄像机移动 就不处理坐标轴变换
//    if (isRightPress)
//        return;
//    scene.transformCoordinate->setCoordinateMode(key);
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
        BBRay ray = m_pScene->getCamera()->createRayFromScreen(e->pos().x(), e->pos().y());
//        m_pScene->transformCoordinate->transform(ray);
        if (true/*!scene.transformCoordinate->getIsTransform()*/)
        {
            // move mouse and do not perform transform of gameobject
            // perform selection operation
            if (m_bRegionSelecting)
            {
                // show selection region, and select gameobjects
                m_pScene->setSelectionRegionVisibility(true);
                m_pScene->setSelectionRegion(m_SelectionRegionStartingPoint, e->pos());
//                regionSelectObjects(scene.setSelectionRegion(selectionRegionStart, e->pos()));
            }
            else
            {
                // do not perform selection operation
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
//        Ray ray = scene.camera.createRayFromScreen(e->pos().x(), e->pos().y());
//        scene.transformCoordinate->setRay(ray);
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
        //鼠标释放时 如果是变换操作结束的释放 不需要拾取对象
        //如果是框选操作结束的释放 也不需要拾取对象
//        if (!scene.transformCoordinate->getIsTransform() && !m_bRegionSelecting)
//        {
//            //3D拾取 选择场景中的对象
//            GameObject *object = scene.pickObject(ray);
//            //发送信号 在层级视图 属性栏等 改变为拾取对象相关的
//            if (isMultipleSelect)
//            {
//                //多选
//                //如果对象非空 增加到多选对象中(或者减去 当已经被选中)
//                if (object)
//                {
//                    updateMultipleSelectObjects(object);
//                }
//            }
//            else
//            {
//                //单选
//                pickObject(object);
//            }
//        }
//        //不再移动坐标轴的时候 上次鼠标位置清空
//        //关闭正在进行变换的开关 下次鼠标释放操作可以进行对象拾取
//        scene.transformCoordinate->stopTransform();
//        //移动坐标轴时 鼠标离开了坐标轴 释放的时候不更新射线 被选中的坐标轴不能恢复原来的颜色
//        scene.transformCoordinate->setRay(ray);

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
        QString fileName;
        dataStream >> fileName;
        if (fileName == "terrain")
        {
//            //创建地形
//            prepareObject = scene.createModel(fileName, event->pos().x(), event->pos().y());
        }
        else
        {
            // Create a temporary object to show drag effect
            // no need to create the corresponding item in the hierarchical tree
            m_pPreviewObject = m_pScene->createModel(BB_PATH_RESOURCE_MESH + fileName, event->pos().x(), event->pos().y());
        }
//        //移除坐标轴选中状态
//        scene.transformCoordinate->setSelectedObject(nullptr);
        event->accept();
    }
//    else if ((data = event->mimeData()->data("light")) != nullptr)
//    {
//        event->accept();
//    }
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
    if (m_pPreviewObject)
    {
        BBRay ray = m_pScene->getCamera()->createRayFromScreen(event->pos().x(), event->pos().y());
        m_pPreviewObject->setPosition(ray.computeIntersectWithXOZPlane(0));
    }
    event->accept();
}

void BBEditViewOpenGLWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    if (m_pPreviewObject)
    {
        // no longer show pre-created object
        m_pScene->deleteGameObject(m_pPreviewObject);
        //设为空 用于下次计算
        m_pPreviewObject = NULL;
    }
    event->accept();
}

void BBEditViewOpenGLWidget::dropEvent(QDropEvent *event)
{
    QByteArray data;
    if ((data = event->mimeData()->data(BB_MIMETYPE_BASEOBJECT)) != nullptr)
    {
//        //落下后 预创建对象成为正式创建的对象 坐标轴选中它
//        scene.transformCoordinate->setSelectedObject(prepareObject);
        // Set the position of the drop point
        BBRay ray = m_pScene->getCamera()->createRayFromScreen(event->pos().x(), event->pos().y());
        m_pPreviewObject->setPosition(ray.computeIntersectWithXOZPlane(0));
        // Show item in hierarchical tree
        // need the item so that set local coordinate
        addGameObjectSignal(m_pPreviewObject);
        // Set to empty for the next calculation
        m_pPreviewObject = NULL;
        event->accept();
    }
//    else if ((data = event->mimeData()->data("light")) != nullptr)
//    {
//        QDataStream dataStream(&data, QIODevice::ReadOnly);
//        QString fileName;
//        dataStream >> fileName;
//        //创建灯光
//        GameObject *object = scene.createLight(fileName, event->pos().x(), event->pos().y());
//        //在层级视图中显示结点
//        addGameObjectSignal(object);
//        event->accept();
//    }
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

//void OpenGLWidget::paintGL()
//{
//    BaseOpenGLWidget::paintGL();
//    //如果正在进行变换 修改右侧属性栏中的数据
//    //重新渲染才会改变transform
//    if (scene.transformCoordinate->getIsTransform())
//    {
//        //transform被修改 发送信号给属性栏
//        updateTransform(scene.transformCoordinate->getSelectedObject(),
//                        scene.transformCoordinate->getTransformModeKey());
//    }
//}











//BaseOpenGLWidget *OpenGLWidget::getPreview()
//{
//    return mPreview;
//}

//void OpenGLWidget::multipleSelectKey(bool isPress)
//{
//    //设置单选 多选模式
//    isMultipleSelect = isPress;
//}

//void OpenGLWidget::lookAtGameObjectSlot(GameObject *gameObject)
//{
//    scene.lookAtGameObject(gameObject);
//}

//void OpenGLWidget::setCoordinateSelectedObject(GameObject *gameObject)
//{
//    scene.transformCoordinate->setSelectedObject(gameObject);
//}

//void OpenGLWidget::setCoordinateSelectedObjects(QList<GameObject*> gameObjects, CenterPoint *center)
//{
//    scene.transformCoordinate->setSelectedObjects(gameObjects, center);
//}

//void OpenGLWidget::deleteSceneGameObject(GameObject *gameObject)
//{
//    scene.deleteGameObject(gameObject);
//}

//void OpenGLWidget::updateCoordinate()
//{
//    scene.transformCoordinate->update();
//}

//void OpenGLWidget::createModelDependParent(QString filePath)
//{
//    //在原点创建模型 并在层级视图中加入对应结点
//    addGameObjectSignal(scene.createModel(filePath));
//}

//void OpenGLWidget::createLightDependParent(QString fileName)
//{
//    addGameObjectSignal(scene.createLight(fileName));
//}

//void OpenGLWidget::copyGameObject(GameObject *sourceObject, QTreeWidgetItem* transcript, QVector3D position)
//{
//    //创建一个文件名 类型 图标 可见性和源对象一样的对象
//    GameObject *transcriptObject = NULL;
//    if (sourceObject->getClassName() == ModelClassName)
//    {
//        //拷贝一个模型对象
//        transcriptObject = scene.createModel(sourceObject->getFilePath(), position);
//    }
//    else if (sourceObject->getClassName() == DirectionLightClassName
//             || sourceObject->getClassName() == PointLightClassName
//             || sourceObject->getClassName() == SpotLightClassName)
//    {
//        //拷贝一个灯光对象
//        transcriptObject = scene.createLight(sourceObject->getFilePath(), position);
//    }
//    else
//    {

//    }
//    //名字使用副本的名字
//    transcriptObject->setName(transcript->text(0));
//    //可见性与源对象一样
//    transcriptObject->setActive(sourceObject->getActive());
//    //拷贝的新对象与拷贝的新树节点插入映射
//    copyGameObjectInsertMap(transcript, transcriptObject);
//}

//void OpenGLWidget::onKeyPress(QKeyEvent *e)
//{
//    scene.onKeyPress(e);
//}




