#include "BBMainWindow.h"
#include "ui_BBMainWindow.h"
#include "Utils/BBUtils.h"
#include <QKeyEvent>
#include "Scene/BBScene.h"
#include "Tools/FBX2BBear/BBFBX2BBear.h"
#include "FileSystem/BBFileSystemManager.h"
#include "FileSystem/BBFileListWidget.h"


BBMainWindow::BBMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pUi(new Ui::BBMainWindow)
{
    m_pUi->setupUi(this);

    setWindowLayout();
    setMenu();
    setGameObjectDockWidget();
    setPreview();
    setConnect();

    m_pUi->buttonRootHierarchy->click();
}

BBMainWindow::~BBMainWindow()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBMainWindow::createProject()
{
    m_pUi->dockProject->createProject();
}

void BBMainWindow::openProject()
{
    m_pUi->dockProject->openProject();
}

void BBMainWindow::showGlobalSettingsProperty()
{
    m_pUi->treeHierarchy->setCurrentItem(NULL);
    m_pUi->propertyManager->showGlobalSettingsProperty(m_pUi->openGLWidget->getScene());
}

void BBMainWindow::showOfflineRendererProperty()
{
    m_pUi->propertyManager->showOfflineRendererProperty(m_pUi->offlineOpenGLWidget->getOfflineRenderer());
}

void BBMainWindow::switchGameObjectPage(int nIndex)
{
    m_pUi->stackedWidget->setCurrentIndex(nIndex);
}

void BBMainWindow::useToolFBX2BBear()
{
    BBFBX2BBear::loadFBXFile(m_pUi->dockProject->getFileSystemManager()->getFileListWidget()->currentItem());
}

void BBMainWindow::setWindowLayout()
{
    // Allow nesting of docks
    setDockNestingEnabled(true);

    // dock layout
    addDockWidget(Qt::LeftDockWidgetArea, m_pUi->dockGameObject);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockConsole, Qt::Vertical);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockEditview, Qt::Horizontal);
    splitDockWidget(m_pUi->dockConsole, m_pUi->dockProject, Qt::Horizontal);
    addDockWidget(Qt::RightDockWidgetArea, m_pUi->dockHierarchy);
    splitDockWidget(m_pUi->dockHierarchy, m_pUi->dockInspector, Qt::Vertical);
    splitDockWidget(m_pUi->dockInspector, m_pUi->dockPreview, Qt::Vertical);

    // resize
    m_pUi->dockGameObjectContents->updateSizeHint(QSize(250, 900));
    m_pUi->dockEditviewContents->updateSizeHint(QSize(1440, 900));
    m_pUi->dockHierarchyContents->updateSizeHint(QSize(640, 300));
    m_pUi->dockInspectorContents->updateSizeHint(QSize(640, 600));
//    m_pUi->dockPreviewContents->updateSizeHint(QSize(480, 300));

    m_pUi->dockPreview->setVisible(false);
}

void BBMainWindow::setMenu()
{
    QMenu *pMenuTools = m_pUi->menuTools;
    QAction *pActionFBX2BBear = new QAction(tr("FBX2BBear"));
    pMenuTools->addAction(pActionFBX2BBear);
    pMenuTools->addSeparator();
    pMenuTools->addAction(new QAction(tr("Options...")));

    QObject::connect(pActionFBX2BBear, SIGNAL(triggered()), this, SLOT(useToolFBX2BBear()));
}

void BBMainWindow::setGameObjectDockWidget()
{
    QButtonGroup *pGroup = new QButtonGroup;
    pGroup->addButton(m_pUi->buttonBase, 0);
    pGroup->addButton(m_pUi->buttonLight, 1);
    pGroup->addButton(m_pUi->buttonParticle, 2);
    pGroup->addButton(m_pUi->buttonCustom, 3);
    pGroup->addButton(m_pUi->buttonOther, 4);
    QObject::connect(pGroup, SIGNAL(buttonClicked(int)), this, SLOT(switchGameObjectPage(int)));

    // Load contents for list
    m_pUi->listBaseGameObject->loadListItems(BB_PATH_BASEOBJECTLIST);
    m_pUi->listBaseGameObject->setMimeType(BB_MIMETYPE_BASEOBJECT);
    m_pUi->listLightGameObject->loadListItems(BB_PATH_LIGHTOBJECTLIST);
    m_pUi->listLightGameObject->setMimeType(BB_MIMETYPE_LIGHTOBJECT);
}

void BBMainWindow::setPreview()
{
    m_pUi->propertyManager->bindPreviewOpenGLWidget(m_pUi->previewOpenGLWidget);
    m_pUi->dockProject->bindPreviewOpenGLWidget(m_pUi->previewOpenGLWidget);
}

void BBMainWindow::setConnect()
{
    // global event about pressing Control key
    QObject::connect(this, SIGNAL(pressMultipleSelectionKey(bool)),
                     m_pUi->openGLWidget, SLOT(pressMultipleSelectionKey(bool)));
    // The handling of key events in the EditView in OpenGL
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressMoveKey(char)),
                     m_pUi->openGLWidget, SLOT(pressMoveKey(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(releaseMoveKey(char)),
                     m_pUi->openGLWidget, SLOT(releaseMoveKey(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressTransform(char)),
                     m_pUi->openGLWidget, SLOT(pressTransform(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressESC()),
                     m_pUi->openGLWidget, SLOT(pressESC()));
    // update title when switching scene
    QObject::connect(m_pUi->openGLWidget, SIGNAL(updateEditViewTitle()),
                     m_pUi->dockEditview, SLOT(setTitleBarText()));
    // save scene
    QObject::connect(m_pUi->dockEditview, SIGNAL(saveCurrentScene()),
                     m_pUi->dockProject, SLOT(saveScene()));
    // Drag into the model, and add item in the Hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(addGameObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(addGameObject(BBGameObject*)));
    // copy object in the scene, need to insert map and perform some operations in hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(addGameObject(BBGameObject*, QTreeWidgetItem*)),
                     m_pUi->treeHierarchy, SLOT(addGameObject(BBGameObject*, QTreeWidgetItem*)));
    // select item in treeHierarchy, use coordinate system to select corresponding object in OpenGL view
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(setCoordinateSystemSelectedObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(setCoordinateSystemSelectedObject(BBGameObject*)));
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(setCoordinateSystemSelectedObjects(QList<BBGameObject*>, BBGameObjectSet*)),
                     m_pUi->openGLWidget, SLOT(setCoordinateSystemSelectedObjects(QList<BBGameObject*>, BBGameObjectSet*)));
    // pick object in OpenGL view, select corresponding item in treeHierarchy
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(selectPickedItem(BBGameObject*)));
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObjects(QList<BBGameObject*>)),
                     m_pUi->treeHierarchy, SLOT(selectPickedItems(QList<BBGameObject*>)));
    // press control to perform multiple selection in OpenGL view, also perform multiple selection in Hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(updateMultipleSelectedObjects(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(updateMultipleSelectedItems(BBGameObject*)));
    // click item in treeHierarchy, and then show property in inspector
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(showGameObjectProperty(BBGameObject*)),
                     m_pUi->propertyManager, SLOT(showGameObjectProperty(BBGameObject*)));
    // change transform in propertyManager, update coordinate system in OpenGL view
    QObject::connect(m_pUi->propertyManager, SIGNAL(coordinateSystemUpdated()),
                     m_pUi->openGLWidget, SLOT(updateCoordinateSystem()));
    // perform transform in OpenGL view, change transform in propertyManager
    QObject::connect(m_pUi->openGLWidget, SIGNAL(updateTransformInPropertyManager(BBGameObject*, char)),
                     m_pUi->propertyManager, SLOT(updateTransform(BBGameObject*, char)));
    // Drag files or prefabs into the hierarchical view, create object in the scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(createModel(QString)),
                     m_pUi->openGLWidget, SLOT(createModelAtOrigin(QString)));
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(createLight(QString)),
                     m_pUi->openGLWidget, SLOT(createLightAtOrigin(QString)));
    // perform operation in treeHierarchy, delete corresponding object in the scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(deleteGameObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(deleteGameObject(BBGameObject*)));
    // copy tree item in treeHierarchy, and copy game object in scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(copyGameObject(BBGameObject*, QTreeWidgetItem*)),
                     m_pUi->openGLWidget, SLOT(copyGameObject(BBGameObject*, QTreeWidgetItem*)));
    // double item, show close-up of corresponding gameobject
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(lookAtGameObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(lookAtGameObject(BBGameObject*)));
    // Select multiple items in the hierarchy tree, the property manager shows the properties of the set
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(showGameObjectSetProperty(BBGameObject*, QList<BBGameObject*>)),
                     m_pUi->propertyManager, SLOT(showGameObjectSetProperty(BBGameObject*, QList<BBGameObject*>)));
    // in the file list, need to show file's property, and remove selected item in the treeHierarchy
    QObject::connect(m_pUi->dockProject, SIGNAL(removeCurrentItemInHierarchyTree()),
                     m_pUi->treeHierarchy, SLOT(removeCurrentItem()));
    QObject::connect(m_pUi->listBaseGameObject, SIGNAL(removeCurrentItemInFileList()),
                     m_pUi->dockProject, SLOT(removeCurrentItem()));
    QObject::connect(m_pUi->listLightGameObject, SIGNAL(removeCurrentItemInFileList()),
                     m_pUi->dockProject, SLOT(removeCurrentItem()));
    // press scene button, show global settings in the property manager
    QObject::connect(m_pUi->buttonRootHierarchy, SIGNAL(clicked()),
                     this, SLOT(showGlobalSettingsProperty()));
    QObject::connect(m_pUi->buttonRootInspector, SIGNAL(clicked()),
                     this, SLOT(showOfflineRendererProperty()));
    // click material file, and show
    QObject::connect(m_pUi->dockProject, SIGNAL(showMaterialPreview(QString)),
                     m_pUi->previewOpenGLWidget, SLOT(showMaterialPreview(QString)));
    QObject::connect(m_pUi->dockProject, SIGNAL(showMaterialInPropertyManager(QString)),
                     m_pUi->propertyManager, SLOT(showMaterialProperty(QString)));
    QObject::connect(m_pUi->dockProject, SIGNAL(removeMaterialPreview()),
                     m_pUi->previewOpenGLWidget, SLOT(removeMaterialPreview()));
    // after baking, show new items in the file list
    QObject::connect(m_pUi->propertyManager, SIGNAL(fileListUpdated()),
                     m_pUi->dockProject, SLOT(updateAll()));

//    //????????????????????????
//    QObject::connect(ui->buttonRootInspector, SIGNAL(clicked()), this, SLOT(switchInspectorRootButton()));
//    //??????????????????????????? ???????????????????????????
//    QObject::connect(ui->treeHierarchy, SIGNAL(updateNameInInspector(GameObject*, QString)),
//                     ui->propertyManager, SLOT(renameGameObject(GameObject*, QString)));
//    //???????????????????????? ????????????????????????????????????
//    QObject::connect(ui->propertyManager, SIGNAL(renameGameObjectInHierarchySignal(GameObject*)),
//                     ui->treeHierarchy, SLOT(renameItemName(GameObject*)));
//    //??????????????????????????? ???????????????????????????
//    QObject::connect(ui->propertyManager, SIGNAL(changeActivationSignal(GameObject*, bool)),
//                     ui->treeHierarchy, SLOT(changeGameObjectActivation(GameObject*, bool)));
//    //??????????????????????????? ???????????????????????????
//    QObject::connect(ui->treeHierarchy, SIGNAL(changeButtonActiveCheckStateInInspector(GameObject*, bool)),
//                     ui->propertyManager, SLOT(changeButtonActiveCheckState(GameObject*, bool)));

//    //???????????????????????? ????????????????????????
//    QObject::connect(ui->treeHierarchy, SIGNAL(createLight(QString)),
//                     ui->openGLWidget, SLOT(createLightDependParent(QString)));
//    //??????????????????????????????????????? ?????? ???????????????????????? ???????????????????????????
//    QObject::connect(ui->propertyManager, SIGNAL(setSelectedObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));

//    //??????????????????????????? ????????????????????????
//    QObject::connect(ui->openGLWidget, SIGNAL(regionSelectObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));
//    //????????????????????? ???????????????????????????????????? ???????????? ?????? ???????????????
//    QObject::connect(ui->listFile, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //???????????? ?????? ???????????????
//    QObject::connect(ui->treeProject, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //???????????? ?????? ?????????????????? ?????????????????? ???????????????????????????
//    QObject::connect(ui->dockEditview, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->treeHierarchy, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listBaseGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listLightGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    //???????????? ????????????????????????????????????
//    QObject::connect(ui->listFile, SIGNAL(showMaterialProperty(QString)),
//                     ui->propertyManager, SLOT(showMaterialProperty(QString)));
//    QObject::connect(ui->listFile, SIGNAL(showFbxProperty(QString)),
//                     ui->propertyManager, SLOT(showFbxProperty(QString)));
//    //???????????????
//    QObject::connect(ui->listFile, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    QObject::connect(ui->treeProject, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    //???????????? ???????????????????????????????????????
//    QObject::connect(ui->propertyManager, SIGNAL(updateMaterialPreviewInOtherWidgetSignal(QString)),
//                     ui->listFile, SLOT(updateMaterialFileIcon(QString)));
//    //???????????? ????????????????????? ????????? ??????????????????????????????
//    QObject::connect(ui->openGLWidget, SIGNAL(updateMaterialProperty(Model*)),
//                     ui->propertyManager, SLOT(updateMaterialFactory(Model*)));

}

void BBMainWindow::keyPressEvent(QKeyEvent *e)
{
//    QMainWindow::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Control:
        pressMultipleSelectionKey(true);
        break;
    default:
        break;
    }
}

void BBMainWindow::keyReleaseEvent(QKeyEvent *e)
{
//    QMainWindow::keyReleaseEvent(e);

    switch (e->key()) {
    case Qt::Key_Control:
        pressMultipleSelectionKey(false);
        break;
    default:
        break;
    }
}
